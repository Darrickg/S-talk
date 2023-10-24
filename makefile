CC = gcc
CFLAGS = -Wall -pthread

all: s_talk s_talk2

s_talk: main.c list.o keyboard.c screen.c receive.c send.c
	$(CC) $(CFLAGS) $^ -o $@

s_talk2: main2.c list.o keyboard.c screen.c receive.c send.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f s_talk s_talk2