# Source code folder
BDIR = bin
# Include files folder
IDIR = include
# Object files output folder
ODIR = obj

# All these files must be in bin/
_OBJS = texted.o
_OBJS += edit.o
_OBJS += insert.o
_OBJS += print.o

OBJS = $(foreach obj, $(_OBJS), $(ODIR)/$(obj))

DEPS = $(wildcard $(IDIR)/*)

CC = gcc
CFLAGS = -I$(IDIR) -Wall

$(ODIR)/%.o: $(BDIR)/%.c $(DEPS)
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

texted: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(IDIR)/*~ $(ODIR)/*.o *~ texted
