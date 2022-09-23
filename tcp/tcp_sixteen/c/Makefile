# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	tcp_sixteen_client

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

tcp_sixteen_client: tcp_sixteen_client.o wrapsock.o error.o
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LDPATH)

