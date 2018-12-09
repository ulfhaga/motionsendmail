#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "arpscanner.h"
#include "log.h"

#define MSGSIZE 512

char *arp_detect()
{
  char *arpdata;
  char inbuf[MSGSIZE];
  size_t size = 0;
  int p[2], pid;
  ssize_t nbytes;
  int sleep = 20;
  arpdata = NULL;

  logger(TRACE, "Start procedure arp_detect");

  if (pipe(p) < 0)
  {
    logger(ERROR, "pipe");
    perror("pipe");
  } else
  {

    /* continued */
    if ((pid = fork()) > 0)
    {
      close(p[0]);              // close reading end in the child
      dup2(p[1], 1);            // send stdout to the pipe
      dup2(p[1], 2);            // send stderr to the pipe
      close(p[1]);              // this descriptor is no longer needed
      execl("/usr/bin/arp-scan", "--interface=wlan0", "--localnet", NULL);
      wait(&sleep);             // Sleep 20 s 
    } else
    {
      close(p[1]);              // Close reads from pipe
      int loop = 0;
      while ((nbytes = read(p[0], inbuf, MSGSIZE)) > 0)
      {
        printf("inbuf:%s\n", inbuf);
        size += (size_t) nbytes;
#ifdef NDEBUG
        printf("nbytes:%i\n", nbytes);
        printf("size:%i\n", size);
        printf("address arpdata:%u\n", arpdata);
#endif

        arpdata = (char *) realloc(arpdata, size + 1);
#ifdef NDEBUG
        printf("----->>>Address = %u\n", arpdata);
#endif
        if (arpdata == NULL)
        {
          logger(ERROR, "Could not allocate arpdata");
          perror("Could not allocate arpdata");
          break;
        } else
        {
          if (loop == 0)
          {
            strncpy(arpdata, inbuf, nbytes);
          } else
          {
            strncat(arpdata, inbuf, nbytes);
          }
#ifdef NDEBUG
          printf("arpdata:%s\n", arpdata);
#endif
        }
        loop++;
      }
    }
  }
  logger(TRACE, "End procedure arp_detect");
#ifdef NDEBUG
  printf("\n\n*************************\nString = %s,  Address = %u\n", arpdata,
         arpdata);
#endif

  return arpdata;
}


#define BUFSIZE 9
void print_buf(char *buf)
{
  int i;
  char c;
  for (i = 0; i < BUFSIZE; i++)
  {
    c = buf[i];
    if (c == '\0')
    {
      printf("\\0");

    } else
    {
      printf("%c", buf[i]);
    }
  }
  printf("\n");
}
