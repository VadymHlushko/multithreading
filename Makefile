CC = gcc
CFLAGS = -g -Wall -pthread
SRC = main.c queue_t.c
TARGET = queue
$(TARGET):
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
clean:
	rm -rf $(TARGET)
