#ifndef ARPSCANNER_H
#define ARPSCANNER_H
#include <stdbool.h>

#define  ARP_DATA_IP_SIZE 15
#define  ARP_DATA_MAC_SIZE  17
#define  ARP_DATA_DEVICE_SIZE  100

typedef struct ARP_DATA {
  char IP[ARP_DATA_IP_SIZE+1];
  char MAC[ARP_DATA_MAC_SIZE+1];
  char device[ARP_DATA_DEVICE_SIZE+1] ;
} ARP_DATA;



char * arp_detect();
void arp_detection();
char * get_line(const char * buf);
char * point_next_line(const char * buf);
ARP_DATA* arp_parse(int number);




ARP_DATA* parse_line_arp_data(const char * buf);

#endif  

