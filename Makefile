CC=gcc
VERSION=1.4

DEBUG=1
ifeq ($(DEBUG),1)
	CFLAGS=-I./include -g -Wall -DDEBUG=1
	TARGET=texted
else
	CFLAGS=-I./include -O2
	TARGET=texted-$(VERSION)
endif

OBJ=src/edit.o src/insert.o src/print.o src/permissions.o main.o

all: $(TARGET) include/texted/texted.h

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm src/*.o
	rm *.o $(TARGET) || rm texted

install:
	sudo install $(TARGET) /usr/bin

remove:
	sudo rm /usr/bin/$(TARGET)
