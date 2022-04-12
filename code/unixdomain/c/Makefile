
RM = rm -f
CC = gcc
CFLAGS = -Wall -g -std=c99
INCLUDES = 
LDFLAGS =
LDPATH =

SOURCES = $(shell ls *.c)
PROGS = $(SOURCES:%.c=%)

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

distclean:
	$(RM) $(PROGS) *.o

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES)

$(PROGS): %: %.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)
