CC=gcc
VERSION=1.4

DEBUG=1
ifeq ($(DEBUG),1)
	CFLAGS=-I./include -g -Wall -DDEBUG=1
else
	CFLAGS=-I./include -O2
endif

TARGET=texted
OBJ=src/edit.o src/insert.o src/print.o src/permissions.o src/texted.o  src/fileio.c main.o

all: $(TARGET)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm src/*.o
	rm *.o $(TARGET)

install:
	sudo install $(TARGET) /usr/bin

remove:
	sudo rm /usr/bin/$(TARGET)
