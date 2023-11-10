CC = gcc
CFLAGS = -Wall -Wextra
SRCS = words.c
TARGET = words

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
