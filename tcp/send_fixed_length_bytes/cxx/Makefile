# Makefile

CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -Werror
INCLUDE = -I./lib
LDFLAGS = -lgflags -lunp
LDPATH = -L./lib

PROGS =	tcp_sixteen_client tcp_sixteen_server

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

tcp_sixteen_client: tcp_sixteen_client.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcp_sixteen_server: tcp_sixteen_server.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)
