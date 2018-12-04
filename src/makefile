CC=gcc
CFLAGS=-I. 
#CFLAGS=-I. -DDEBUG -DTEST -g 
DEPS=motion.h mailalarm.h log.h leds.h arpscanner.h
SOURCE=motionmail.c motion.c mailalarm.c log.c leds.c arpscanner.c
OBJ=motionmail.o motion.o mailalarm.o log.o leds.o arpscanner.o
LDFLAGS = -Wl,-rpath -Wl,/usr/local/lib  -lwiringPi -lquickmail
LINT=cppcheck --language=c --enable=warning,style,unusedFunction,information --template=gcc 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

motionmail: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	
.PHONY: clean
clean:
	rm -f *.o  
	rm -f motionmail

.PHONY: indent
indent:
	indent -kr -bl -bli0 -ci2 -cli2 -i2 -l80 -nut *.c 

.PHONY: indent1
indent1:
	indent -nbad -bap -nbc -bbo -bl -bli2 -bls -ncdb -nce -cp1 -cs -di2 -ndj -nfc1 -nfca -hnl -i2 -ip5 -lp -pcs -psl -nsc -nsob *.c 
	
.PHONY: install
install:
	cp -f -v motionmail /home/pi/bin/
 
.PHONY: lint
lint :
	$(LINT) $(SOURCE) 


