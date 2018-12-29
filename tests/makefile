CC=gcc
CFLAGS=-I. -I../src -DTRACE -g 
#CFLAGS=-I. -I../src -D DEBUG -DTEST -g 
#CFLAGS=-I. -I../src 
SOURCE=check_aprscanner.c 
OBJ=../src/arpscanner.o ../src/mailalarm.o ../src/log.o check_arpscanner.o 
LDFLAGS = -Wl,-rpath -Wl,/usr/local/lib  -lwiringPi -lquickmail
LINT=cppcheck --language=c --enable=warning,style,unusedFunction,information --template=gcc 


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

check_arpscanner_tests: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	
.PHONY: clean
clean:
	rm -f *.o  
	rm -f check_arpscanner_tests

.PHONY: indent
indent:
	indent -kr -bl -bli0 -ci2 -cli2 -i2 -l80 -nut *.c 
		
.PHONY: lint
lint :
	$(LINT) $(SOURCE) 

.PHONY: test
test: check_arpscanner_tests
	./check_arpscanner_tests
