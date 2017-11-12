CC=gcc
CFLAGS=-g -Wall -pthread
RM=rm

all: clean serv

serv: main.c
		$(CC) $(CFLAGS) -o serv main.c

clean:
		$(RM) -rf serv *.dSYM
