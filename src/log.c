#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

extern int errno;

#define MAX_LEN_LOG_MESSAGE 1023

void logger(const char *tag, const char *message)
{
  time_t curtime = 0;
  char s[MAX_LEN_LOG_MESSAGE + 1];
  time_t t = time(&curtime);
  struct tm *p = localtime(&t);
  strftime(s, MAX_LEN_LOG_MESSAGE, "%Y-%m-%d %H:%M:%S", p);
  printf("%s [%s]: %s\n", s, tag, message);
  fflush(stdout);
}


int logToFile()
{
  // Ej klar med
  FILE *fp;
  fp = fopen("motionsendmail.log", "r");
  if (fp == NULL)
  {
    fprintf(stderr, "Value of errno: %d\n", errno);
    fprintf(stderr, "Error opening file: %s\n", strerror(errno));
  }
  else
  {
    fclose(fp);
  }

  return (0);
}
