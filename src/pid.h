#ifndef PID_H
#define PID_H

#include <unistd.h>

#define FILE_PID "/tmp/pid.txt"

void storePid(pid_t pid);
pid_t loadPid();

#endif /* PID */
