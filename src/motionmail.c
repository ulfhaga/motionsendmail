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

static volatile uint globalCounter = 0;
static volatile bool detection = false;
static volatile bool lastDetection = false;
static volatile uint emailCounter = 0;

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

void alarmInterrupt();

int main(int argc, char **argv)
{
  logger(INFO, "Raspberry Pi alarm program started");
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
  led1blink();

  // Setup interrupt  
  wiringPiISR(DHTPIN, INT_EDGE_BOTH, &alarmInterrupt);

  // Wait for interrupts
  while (true)
  {
    delay(20 * 60 * 1000);
    if (emailCounter > 200)
    {
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
    logger(INFO, "Alarm interrupt rising");
    logger(DEBUG, "Set led on!");
    led1ON();
    logger(INFO, "Sending email!");


    char *arpdata;
    arpdata = arp_detect();

    /* funkar
    arpdata = calloc(512,sizeof(char));
    if (snprintf(arpdata,14+1,"%s", "Dummy ARP data") >= 14)
    {
       logger(ERROR, "Not enough space!");
    }
     */
    
    sendmail(arpdata);
    logger(INFO, "free start");
    free(arpdata);
    
    logger(INFO, "free end");
    arpdata = NULL;

    emailCounter++;
    lastDetection = true;
  }

  if ((detection == false) && (lastDetection == true))
  {
    logger(INFO, "Alarm interrupt falling");
    logger(DEBUG, "Set led off!");
    led1OFF();
    lastDetection = false;
  }
  logger(INFO, "Alarm interrupt handling ends");
};
