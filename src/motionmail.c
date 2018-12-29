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
#include <unistd.h>
#include <sys/reboot.h>
#include <stdbool.h>

#include "motion.h"
#include "mailalarm.h"
#include "leds.h"
#include "log.h"
#include "arpscanner.h"
#include "motionmail.h"


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
  printf("Debug run\n");
#else
  printf("Release run\n");
#endif

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



alarmInterrupt();
return 0;

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

//  if ((detection == true) && (lastDetection == false))
  {
    lastDetection = true;
    logger(INFO, "Alarm interrupt rising");
    logger(DBG, "Set led on!");
    led1ON();



    char *arpdata;

    arp_detection();



    int number_of_hosts;
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

      snprintf(line, ARP_DATA_MAC_SIZE + 6, " MAC:%s", result->MAC);
      strcat(arpdata, line);

      snprintf(line, ARP_DATA_DEVICE_SIZE + 9, " Name:%s !", result->device);
      strcat(arpdata, line);

      strcat(arpdata, "<br />");
      strcat(arpdata, "\n");
      // logger(INFO, "***********************************");
      //   logger(INFO, arpdata);
      free(result);
      result = NULL;
      }
    }

    sendmail(arpdata);
    logger(INFO, "free start");
    free(arpdata);
    logger(INFO, "free end");
    arpdata = NULL;
    arp_cleanup();

    /*
       char *xxxxxx = arp_detect();
       free (xxxxxx);
     */

    // funkar

    /*    arpdata = calloc(512,sizeof(char));
       if (snprintf(arpdata,14+1,"%s", result->IP) < 7)
       {
       logger(ERROR, "Not enough space!");
       }
     */

//    sendmail(arpdata);
    // char new_arpdata[501];
    // strncpy(new_arpdata, arpdata, 500);
    // logger(INFO, "free start");
    // free(arpdata);
    //  logger(INFO, "free end");
    // arpdata = NULL;
    // logger(INFO, "Sending email!");
    // sendmail(new_arpdata);
    emailCounter++;

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
