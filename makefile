CC = gcc
CFLAGS = -Wall -Werror -pedantic-errors
TARGET = runFile
SOURCE = list.c list.h test.c

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $@ $^

run: ./$(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)