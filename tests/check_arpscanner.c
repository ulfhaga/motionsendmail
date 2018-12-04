#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "arpscanner.h"
#include "mailalarm.h"

void test_arp_scanner();
void test_send_mail();

int main(int argc, char **argv)
{
  test_send_mail();
  test_arp_scanner();
  return 0;
}

void test_send_mail()
{
  char message[] = "123456789";
  sendmail(message);
}

void test_arp_scanner()
{
  printf("Test of arpscanner\n");
  char *arpdata;
  arpdata = arp_detect();
  if (strlen(arpdata) > 0)
  {
    if (strncmp("Interface:", arpdata, 10) == 0)
    {
      printf("Successful\n");
    } else
    {
      printf("Result:\n%s", arpdata);
      printf("Failed\n");
    }
#ifdef DEBUG
    printf("Result:\n%s", arpdata);
#endif
  } else
  {
    printf("Failed\n");
  }
}
