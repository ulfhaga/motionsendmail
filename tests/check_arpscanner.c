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
void test_arp_scanner_get_line();
void test_point_next_line();
void test_parse_line_arp_data();
void test_arp_parse();

int main(int argc, char **argv)
{
    #ifdef DEBUG   
     printf("Debug run\n");
    #else
       printf("Release run\n");
    #endif
  //test_send_mail();
  //  test_arp_scanner();
//  test_arp_scanner_get_line();
//  test_point_next_line();
   // test_parse_line_arp_data();
 test_arp_parse();

  return 0;
}

void test_send_mail()
{
  char message[] = "123456789";
  sendmail(message);
}

void test_parse_line_arp_data()
{
  ARP_DATA* arp_data;
  const char * buf = "192.168.1.201 20:d5:bf:0b:f3:3f Samsung Eletronics Co., Ltd";
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
char * line;

line = point_next_line(message);
    if (strcmp(line, "abc") == 0)
    {
      printf("Successful\n");
    } else
    {
      printf("Result:\n%s", line);
      printf("Failed\n");
    }
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
#ifdef NDEBUG
     printf("Result:\n%s", arpdata);
#endif
  } else
  {
    printf("Failed\n");
  }
  free(arpdata);
}

void test_arp_scanner_get_line()
{
char message1[] = "123456789\n00000";
char * line;
line = get_line(message1);
if (strcmp("123456789", line) == 0)
    {
      printf("Successful\n");
    } else
    {
      printf("Result:\n%s", line);
    }
free(line);


char message2[] = "\n";
line = get_line(message2);
if ( line == NULL)
    {
      printf("Successful\n");
    } else
    {
      printf("Failed\n");
      printf("Result:\n%s\n", line);
    }
free(line);

}

 void test_arp_parse()
 {
   arp_detection();
   ARP_DATA* result;
   result = arp_parse(1);
   free (result);
 }

/*
void parse_arp_data()
{
    char str1[]= "To be or not to be";
    char *data = "Interface: eth0, datalink type: EN10MB (Ethernet)\nStarting arp-scan 1.9 with 256 hosts (http://www.nta-monitor.com/tools/arp-scan/)\n192.168.1.1 c4:ea:1d:e4:ed:8c (Unknown)\n192.168.1.201 20:d5:bf:0b:f3:3f Samsung Eletronics Co., Ltd\n192.168.1.243 b8:27:eb:84:10:91 Raspberry Pi Foundation\n\n3 packets received by filter, 0 packets dropped by kernel;\n"  
}
 */
