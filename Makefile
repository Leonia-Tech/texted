CC=gcc
CFLAGS=-I./include -g -Wall
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