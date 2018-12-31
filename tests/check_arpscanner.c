#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "arpscanner.h"
#include "mailalarm.h"
#include "motionmail.h"


void test_arp_scanner();
void test_send_mail();
void test_arp_scanner_get_line();
void test_point_next_line();
void test_parse_line_arp_data();
void test_arp_parse();
void test_arp_hosts();


int main(int argc, char **argv)
{
#ifdef DEBUG
  printf("Debug run\n");
#else
  printf("Release run\n");
#endif
  // test_send_mail();
  test_arp_scanner();
  test_arp_scanner_get_line();
  test_point_next_line();
  test_parse_line_arp_data();
  test_arp_parse();
  test_arp_hosts();
  return 0;
}

void test_send_mail()
{
  char message[] = "123456789";
  sendmail(message);
}


void test_arp_hosts()
{
  int number_of_hosts;
  arp_detection();

  number_of_hosts = arp_hosts();
  if (number_of_hosts > 0)
  {
    printf("Successful number of hosts %i \n", number_of_hosts);
  }
  else
  {
    printf("Failed number of hosts. %i\n", number_of_hosts);
  }
}


void test_parse_line_arp_data()
{
  ARP_DATA *arp_data;
  const char *buf =
    "192.168.1.201 20:d5:bf:0b:f3:3f Samsung Eletronics Co., Ltd";
  arp_data = parse_line_arp_data(buf);


  if (strcmp(arp_data->IP, "192.168.1.201") == 0)
  {
    printf("Successful IP\n");
  }
  else
  {
    printf("Failed IP. Result:%s\n", arp_data->IP);
  }



  if (strcmp(arp_data->MAC, "20:d5:bf:0b:f3:3f") == 0)
  {
    printf("Successful MAC\n");
  }
  else
  {
    printf("Failed MAC. Result:%s\n", arp_data->MAC);
  }


  if (strcmp(arp_data->device, "Samsung Eletronics Co., Ltd") == 0)
  {
    printf("Successful device\n");
  }
  else
  {
    printf("Failed device. Result:%s\n", arp_data->device);
  }

}

void test_point_next_line()
{
  char message[] = "123456789\nabc";
  char *line;

  line = point_next_line(message);
  if (strcmp(line, "abc") == 0)
  {
    printf("Successful\n");
  }
  else
  {
    printf("Result:\n%s", line);
    printf("Failed\n");
  }
}

void test_arp_scanner()
{
  printf("Test of arpscanner\n");
  char *arpdata;
  arp_detection();
  arpdata = get_arpdata();
  
  if (strlen(arpdata) > 0)
  {
    if (strncmp("Interface:", arpdata, 10) == 0)
    {
      printf("Successful\n");
    }
    else
    {
      printf("Result:\n%s", arpdata);
      printf("Failed\n");
    }
#ifdef NDEBUG
    printf("Result:\n%s", arpdata);
#endif
  }
  else
  {
    printf("Failed\n");
  }
  free(arpdata);
}

void test_arp_scanner_get_line()
{
  char message1[] = "123456789\n00000";
  char *line;
  line = get_line(message1);
  if (strcmp("123456789", line) == 0)
  {
    printf("Successful\n");
  }
  else
  {
    printf("Result:\n%s", line);
  }
  free(line);


  char message2[] = "\n";
  line = get_line(message2);
  if (line == NULL)
  {
    printf("Successful\n");
  }
  else
  {
    printf("Failed\n");
    printf("Result:\n%s\n", line);
  }
  free(line);

}

void test_arp_parse()
{

  ARP_DATA *result = NULL;

#if 1
  arp_detection();
  result = arp_parse(5000);
  if (result == NULL)
  {
    printf("Successful\n");
  }
  else
  {
    printf("Failed! %s\n", result);
  }

  free(result);
  result = NULL;
  arp_cleanup();

#endif


  arp_detection();
  result = arp_parse(1);
  if (strlen(result->IP) > 5)
  {
    printf("Successful IP:%s\n", result->IP);
  }
  else
  {
    printf("Failed:\n%s", result->IP);
  }
  free(result);
  result = NULL;
  arp_cleanup();



  arp_detection();
  result = arp_parse(2);
  if (result == NULL)
  {
    printf("Failed\n");
  }
  else
  {
    if (strlen(result->IP) > 5)
    {
      printf("Successful IP:%s\n", result->IP);
    }
    else
    {
      printf("Failed:\n%s", result->IP);
    }
    free(result);
    result = NULL;
    arp_cleanup();
  }

}

