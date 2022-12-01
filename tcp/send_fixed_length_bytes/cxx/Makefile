# Makefile

CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDE = -I./lib
LDFLAGS =
LDLIBS = -lgflags
VPATH = ./lib

PROGS =	tcp_sixteen_client tcp_sixteen_server

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

tcp_sixteen_client: tcp_sixteen_client.o wrapsock.o error.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

tcp_sixteen_server: tcp_sixteen_server.o wrapsock.o error.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)
