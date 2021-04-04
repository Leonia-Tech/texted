SHELL=/bin/sh

CC=gcc
VERSION=1.4.1
CFLAGS=-I./include

ifeq ($(DEBUG),1)
	CFLAGS+= -g -Wall -Wextra -Wpedantic -Wshadow -DDEBUG=1
else
	CFLAGS+= -O2
endif

LIBS=-lncurses -pthread -lreadline -lhistory
TARGET=texted
OBJS=src/edit.o src/insert.o src/print.o src/permissions.o src/texted.o  src/fileio.o src/credits.o src/unirun.o main.o

PREFIX=/usr
MANDIR=$(PREFIX)/share/man/man1
LIBDIR=$(PREFIX)/lib
DATADIR=$(PREFIX)/share
BINDIR=$(PREFIX)/bin

.PHONY: all
all: $(TARGET)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

.PHONY: clean
clean:
	rm src/*.o
	rm *.o $(TARGET)

.PHONY: install
install: $(TARGET)
	# Install documentation
	install -Dm 644 ./docs/texted.1 -t $(DESTDIR)$(MANDIR)/
	gzip $(DESTDIR)$(MANDIR)/texted.1

	# Install assets
	mkdir -p $(DESTDIR)$(DATADIR)/$(TARGET)
	cp -r ./assets/ $(DESTDIR)$(DATADIR)/$(TARGET)

	# Install scripts
	mkdir -p $(DESTDIR)$(LIBDIR)/$(TARGET)
	cp -r ./src/highlighter/ $(DESTDIR)$(LIBDIR)/$(TARGET)

	# Install binary
	install -Dm 755 $(TARGET) -t $(DESTDIR)$(BINDIR)/

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(MANDIR)/texted.1.gz		# Remove documentation
	rm -rf $(DESTDIR)$(DATADIR)/$(TARGET)		# Remove assets
	rm -rf $(DESTDIR)$(LIBDIR)/$(TARGET)		# Remove scripts
	rm -f $(DESTDIR)$(BINDIR)/$(TARGET)			# Remove binary