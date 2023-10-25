CC = gcc
CFLAGS = -Wall -pthread

all: s-talk 

s-talk:
	$(CC) $(CFLAGS) main.c list.o keyboard.c screen.c receive.c send.c manage_thread.c -o s-talk -lpthread

clean:
	rm -f s-talk 