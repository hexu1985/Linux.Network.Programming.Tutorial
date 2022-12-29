# Makefile

CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDE = -I./lib
LDFLAGS =
LDLIBS = -lgflags
VPATH = ./lib

PROGS =	tcp_block_client tcp_block_server

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

tcp_block_client: tcp_block_client.o blocks.o wrapsock.o error.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

tcp_block_server: tcp_block_server.o blocks.o wrapsock.o error.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)
