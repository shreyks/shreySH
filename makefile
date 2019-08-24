CC=gcc
CFLAGS=-I. -Wall -no-pie
DEPS = echobuiltin.h cdbuiltin.h pwdbuiltin.h pinfobuiltin.h remindme.h lsbuiltin.h clockbuiltin.h jobcontrol.h
OBJ = execute.o parse.o split.o final.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

shreySH: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
