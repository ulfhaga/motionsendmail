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
#include "log.h"
#include "motion.h"
#define MAXTIMINGS  1
#define DHTPIN    29            /* (wPi:29) (phy pin:16) (GPIO-29 */

#define ON 1
#define OFF 0


bool motion_detect()
{
  bool status = false;
  int laststate = OFF;

  laststate = digitalRead(DHTPIN);

  if (laststate == ON)
  {
    status = true;
  }

  return status;
}
