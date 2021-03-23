CC=gcc

DEBUG=0
ifeq ($(DEBUG),1)
	CFLAGS=-I./include -g -Wall -DDEBUG=1
else
	CFLAGS=-I./include -O2
endif

TARGET=texted
OBJ=src/edit.o src/insert.o src/print.o main.o

all: $(TARGET)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm src/*.o
	rm *.o texted