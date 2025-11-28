

CC = gcc
CFLAGS = -Wall -g
TARGET = main_prg

all: $(TARGET)

$(TARGET): main_prg.c
	$(CC) $(CFLAGS) -o $(TARGET) main_prg.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
