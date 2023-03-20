# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -Werror
INCLUDE = -I./lib
LDFLAGS = -lunp
LDPATH = -L./lib

PROGS =	get_host_by_name

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

get_host_by_name: get_host_by_name.o
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LDPATH)


