# Makefile

CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -Werror
INCLUDE = -I./lib
LDFLAGS = -lgflags -lunp
LDPATH = -L./lib

PROGS =	tcp_block_client tcp_block_server

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

tcp_block_client: tcp_block_client.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcp_block_server: tcp_block_server.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)
