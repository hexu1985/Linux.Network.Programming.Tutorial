# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -Werror
INCLUDE = -I./lib
LDFLAGS = -lunp
LDPATH = -L./lib

PROGS =	tcp_daytime_client tcp_daytime_server

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

tcp_daytime_client: tcp_daytime_client.o
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LDPATH)

tcp_daytime_server: tcp_daytime_server.o
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LDPATH)

