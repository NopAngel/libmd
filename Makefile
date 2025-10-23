CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fPIC
LDFLAGS = -shared
TARGET = libmd.so
SOURCES = ./src/md.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

install_complete: $(TARGET)
	cp $(TARGET) /usr/local/lib/
	cp md.h /usr/local/include/
	ldconfig
