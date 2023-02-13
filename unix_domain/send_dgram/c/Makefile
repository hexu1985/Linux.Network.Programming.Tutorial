# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = -I./lib
LDFLAGS = 
LDPATH = 
VPATH = ./lib

PROGS =	unix_domain_dgram_client unix_domain_dgram_server

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

unix_domain_dgram_client: unix_domain_dgram_client.o wrapsock.o error.o
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LDPATH)

unix_domain_dgram_server: unix_domain_dgram_server.o wrapsock.o error.o
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LDPATH)

