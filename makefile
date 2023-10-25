CC = gcc
CFLAGS = -Wall -pthread

all: s_talk 

s_talk: main.c list.o keyboard.c screen.c receive.c send.c manage_thread.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f s_talk 