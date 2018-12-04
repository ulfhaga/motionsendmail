
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "arpscanner.h"

int main(int argc, char **argv)
{
  logger(INFO, "Unit test");
  char *arpdata;
  arpdata = arp_detect();
  printf("Result:\n%s");

}
