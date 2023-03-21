# Makefile

CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -Werror
INCLUDE = -I./lib
LDFLAGS = -lgflags -lunp
LDPATH = -L./lib

PROGS =	udp_local_client udp_local_server

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

udp_local_client: udp_local_client.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)

udp_local_server: udp_local_server.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)
