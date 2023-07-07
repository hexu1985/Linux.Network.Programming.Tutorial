# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -Werror
INCLUDE = -I./lib
LDFLAGS = -lunp
LDPATH = -L./lib

PROGS =	tcp_echo_client tcp_echo_server

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

tcp_echo_client: tcp_client.o str_cli.o
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LDPATH)

tcp_echo_server: tcp_server.o str_echo.o
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LDPATH)

