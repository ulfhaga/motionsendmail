#!/bin/bash
set -x
#export LD_LIBRARY_PATH=/usr/local/lib
#LD_RUN_PATH=/usr/local/lib
gcc -g -Wall -c motion.c
gcc -g -Wall -c motionmail.c
gcc -Wl,-rpath -Wl,/usr/local/lib -Wall -o motionmail motionmail.o motion.o -lwiringPi -llirc_client -lquickmail
chmod +x motionmail
# cp -f sendmymail    ~/bin/sendmymail 
# file ~/bin/motion 

