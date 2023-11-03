CC = gcc
CFLAGS = -Wall -std=c11
LIBS = -lraylib -lm -lpthread -ldl -lrt
TARGET = vsort

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
