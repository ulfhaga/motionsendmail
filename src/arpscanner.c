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

char *arpdata = NULL;

void arp_detection()
{
  char inbuf[MSGSIZE + 1];
  size_t size = 0;
  int p[2], pid;
  ssize_t nbytes;
  int sleep = 1;

  logger(TRACE, "Start procedure arp_detect");

  if (arpdata != NULL)
  {
    free(arpdata);
    arpdata = NULL;
  }

  if (pipe(p) < 0)
  {
    logger(ERROR, "pipe");
  }
  else
  {
    /* continued */
    pid = fork();

    if (pid == -1)
    {
      logger(ERROR, "fork");

    }
    else
    {
      if (pid == 0)
      {
        close(p[0]);            // close reading end in the child
        dup2(p[1], 1);          // send stdout to the pipe
        dup2(p[1], 2);          // send stderr to the pipe
        close(p[1]);            // this descriptor is no longer needed
        execl("/usr/bin/arp-scan", "--interface=wlan0", "--localnet",
              "--numeric", "--ignoredups", NULL);
        logger(ERROR, "arp_detection execl failed");
        wait(&sleep);           // Sleep 
        _exit (EXIT_FAILURE);
      }
      else
      {
        close(p[1]);            // Close reads from pipe

        nbytes = read(p[0], inbuf, MSGSIZE);
        size = (size_t) MSGSIZE;
        arpdata = (char *) calloc(size + 1, sizeof(char));
        strncpy(arpdata, inbuf, nbytes);

        while ((nbytes = read(p[0], inbuf, MSGSIZE)) > 0)
        {
          size += (size_t) MSGSIZE;
#ifdef DEBUG
          printf("inbuf:%s\n", inbuf);
          printf("nbytes:%i\n", nbytes);
          printf("size:%i\n", size);
          printf("address arpdata:%u\n", arpdata);
#endif
          arpdata = (char *) realloc(arpdata, size + 1);
#ifdef DEBUG
          printf("----->>>Address = %u\n", arpdata);
#endif
          if (arpdata == NULL)
          {
            logger(ERROR, "Could not allocate arpdata");
            perror("Could not allocate arpdata");
            break;
          }
          else
          {
            strncat(arpdata, inbuf, nbytes);
#ifdef DEBUG
            printf("arpdata:%s\n", arpdata);
#endif
          }
        }                       //while

        if ((arpdata != NULL) && size > 0)
        {
#ifdef DEBUG
          printf("inbuf:%s\n", inbuf);
          printf("nbytes:%i\n", nbytes);
          printf("size:%i\n", size);
          printf("address arpdata:%u\n", arpdata);
#endif
          arpdata[size] = '\0';
        }

        if ((arpdata != NULL) && (strncmp("Interface:", arpdata, 10) != 0))
        {
          logger(ERROR, "Detection of apr scan failed");
          logger(ERROR, arpdata);
          free(arpdata);
          arpdata = NULL;
        }

      }
    }
  }

#if defined(DEBUG)
  printf("\nPackage>>\n%s>>Package\n", arpdata);
#endif
  logger(TRACE, "End procedure arp_detect");
}


char *get_arpdata()
{
  char *dest;
  size_t size = strlen(arpdata);
  dest = malloc(size + 1);
  memcpy(dest, arpdata, size + 1);
  return dest;
}

ARP_DATA *parse_line_arp_data(const char *buf)
{
  size_t i;
  size_t n;
  char *line;
  line = (char *) buf;
  ARP_DATA *arp_data;

  arp_data = NULL;

  if (buf != NULL)
  {
    arp_data = (ARP_DATA *) calloc(1, sizeof(ARP_DATA));
    n = strlen(buf);

    // Get IP
    for (i = 0;
         (i < n) && (line[i] != '\0') && (line[i] != ' ') && (line[i] != '\t');
         i++)
    {
      (*arp_data).IP[i] = line[i];
    }
    arp_data->IP[i] = '\0';
    i++;

    // Remove space
    for (; (i < n) && (line[i] != '\0') && (line[i] == ' '); i++)
    {
    }

    // Get MAC
    for (int j = 0;
         (i < n) && (line[i] != '\0') && (line[i] != ' ') && (line[i] != '\t');
         i++, j++)
    {

      arp_data->MAC[j] = line[i];
    }

    arp_data->MAC[i] = '\0';
    i++;

    // Remove space
    for (; (i < n) && (line[i] != '\0') && (line[i] == ' '); i++)
    {
    }

    // Get device name
    for (int j = 0;
         (i < n) && (line[i] != '\0') && (line[i] != '\t') && (line[i] != '\n');
         i++, j++)
    {
      arp_data->device[j] = line[i];
    }

    arp_data->device[i] = '\0';

  }
  return arp_data;
}

// Point to next line.
char *point_next_line(const char *buf)
{
  int i;
  char c;
  size_t size;
  char *line;
  line = (char *) buf;

  if (buf != NULL)
  {
    size = strlen(buf);

    while (true)
    {
      c = *line;
      if (c == '\0')
      {
        break;
      }
      if (c == '\n')
      {
        line++;
        break;
      }
      line++;
    }
  }
  return line;
}


// Get the new line
char *get_line(const char *buf)
{
  int i;
  char c;
  size_t size;
  char *line;

  size = strlen(buf);
  if (size > 0)
  {
    line = (char *) calloc(size + 1, sizeof(char));

    for (i = 0; i < size; i++)
    {
      c = buf[i];

      if ((c == '\n') || (c == '\0'))
      {
        break;
      }
    }
    if (i > 0)
    {
      strncpy(line, buf, i);
    }
    else
    {
      line = NULL;
    }
  }
  else
  {
    line = NULL;
  }
  return line;
}



int arp_hosts()
{
  int number;
  ARP_DATA *result = NULL;
  number = 0;
  if (arpdata != NULL)
  {
    char *arpdata_start = arpdata;
    char *arpdata_second_line = point_next_line(arpdata_start);
    char *line = arpdata_second_line;
    while (true)
    {
      char *current;
      current = point_next_line(line);
      line = current;
      if ((line == NULL) || (*line == '\n'))
      {
        break;
      }
      else
      {
        number++;
      }
    }

  }
  return number;
}



ARP_DATA *arp_parse(int number)
{

  ARP_DATA *result = NULL;

  if (arpdata != NULL)
  {
    char *current;
    bool parse = false;
    char *arpdata_start = arpdata;
    char *arpdata_second_line = point_next_line(arpdata_start);
    char *line = arpdata_second_line;
    for (int i = 1; i <= number; i++)
    {
      current = point_next_line(line);
      line = current;
      if ((line == NULL) || (*line == '\n'))
      {
        parse = false;
        break;
      }
      else
      {
        parse = true;
      }
    }
    if (parse)
    {
      result = parse_line_arp_data(current);
    }

  }
  return result;
}

// Clean up
void arp_cleanup()
{
  free(arpdata);
  arpdata = NULL;
}
