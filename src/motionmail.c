#include <wiringPi.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lirc/lirc_client.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <sys/reboot.h>
#include <stdbool.h>

#include "motion.h"
#include "mailalarm.h"
#include "leds.h"
#include "log.h"
#include "arpscanner.h"
#include "motionmail.h"
#include "takepicture.h"

static volatile uint globalCounter = 0;
static volatile bool detection = false;
static volatile bool lastDetection = false;
static volatile uint emailCounter = 0;

#define  ARP_SIZE  1024

/**
 * Send an email then an alarm occurs.
 * 
 * Notes:
 * gpio edge 29 falling 
 * gpio edge 29 rising
 * http://wiringpi.com/reference/priority-interrupts-and-threads/
 * https://projects.drogon.net/raspberry-pi/wiringpi/functions/
 * https://github.com/WiringPi/WiringPi/blob/master/examples/isr.c
 */

int main(int argc, char **argv)
{
  logger(INFO, "Raspberry Pi alarm program started");

#ifdef DEBUG
  //intf("INFO: Debug run\n");
  logger(INFO,"Debug run");
#else
  logger(INFO,"Release run");
#endif

 // pid_t pid = getpid();
 // storePid(pid);
  
  int status = 1;

  // Use  wiringPi pin 
  if (wiringPiSetup() == -1)
  {
    logger(ERROR, "Wiring Pi Setup failed");
    exit(1);
  }

  pinMode(DHTPIN, INPUT);

  // Blink for init
  blinkInit();
  led1OFF();
  led1blink();


  // Setup interrupt  
  wiringPiISR(DHTPIN, INT_EDGE_BOTH, &alarmInterrupt);

  // Wait for interrupts
  while (true)
  {
    delay(20 * 60 * 1000);
    if (emailCounter > 200)
    {
      logger(INFO, "EmailCounter > 200. Program ends");
      break;
    }
  }

  logger(INFO, "Raspberry Pi alarm program ended");
  return (status);
}


void alarmInterrupt()
{
  ++globalCounter;
  logger(INFO, "Alarm interrupt handling starts");

  detection = motion_detect();

  if ((detection == true) && (lastDetection == false))
  {
    bool sendMail = true;
    char *arpdata;
    int number_of_hosts;

    lastDetection = true;
    logger(INFO, "Alarm interrupt rising");
    logger(DBG, "Set led on!");
    led1ON();

    arp_detection();
    number_of_hosts = arp_hosts();
    arpdata = calloc(ARP_SIZE, sizeof(char));

    for (int i = 1; i <= number_of_hosts; i++)
    {
      ARP_DATA *result;
      char line[ARP_DATA_MAX_SIZE + 100];
      result = arp_parse(i);
      if (result != NULL)
      {
        snprintf(line, ARP_DATA_IP_SIZE + 4, "IP:%s", result->IP);
        strcat(arpdata, line);

        if (strcmp(result->IP, "192.168.1.126") == 0)
        {
          sendMail = false;
          logger(INFO, "IP 192.168.1.126 found");
        }

        snprintf(line, ARP_DATA_MAC_SIZE + 6, " MAC:%s", result->MAC);
        strcat(arpdata, line);

        snprintf(line, ARP_DATA_DEVICE_SIZE + 9, " Name:%s !", result->device);
        strcat(arpdata, line);

        strcat(arpdata, "<br />");
        strcat(arpdata, "\n");

        free(result);
        result = NULL;
      }
    }

    if (sendMail)
    {
      char ref_last_picture[] = "<a href=\"http://188.149.157.235/picture/latest.jpg\">Sista bilden</a> <br>";
      size_t len = strlen(ref_last_picture) + strlen(arpdata) ;
      char *mail_message = calloc(len + 1, sizeof(char));
      strcpy(mail_message, ref_last_picture);
      strcat(mail_message, arpdata);
      sendmail(mail_message);
      free (mail_message);
    }
    
    logger(INFO, "free start");
    free(arpdata);
    logger(INFO, "free end");
    arpdata = NULL;
    arp_cleanup();
    emailCounter++;

    // Take a picture
    take_picture();
  }

  if ((detection == false) && (lastDetection == true))
  {
    logger(INFO, "Alarm interrupt falling");
    logger(DBG, "Set led off!");
    led1OFF();
    lastDetection = false;
  }
  logger(INFO, "Alarm interrupt handling ends");
};
