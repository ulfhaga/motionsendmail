#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "pid.h"

void storePid(pid_t pid)
{
  char * message;
  message = (char *) malloc(100);
  sprintf(message, "PID: %d", pid);
  logger(INFO, message);
  free(message);
  
  FILE *file_pid;
  file_pid = fopen(FILE_PID,"w");
  fprintf(file_pid,"%d", pid);
  fclose(file_pid); 
}

pid_t loadPid()
{
  char * message;
  FILE *file_pid;
  pid_t num; 
  file_pid = fopen(FILE_PID,"r");
  fscanf(file_pid,"%d", &num);
  fclose(file_pid);
  
  message = (char *) malloc(100);
  sprintf(message, "Read PID from file: %d", num);
  logger(INFO, message);
  free(message);
  return num;
}




