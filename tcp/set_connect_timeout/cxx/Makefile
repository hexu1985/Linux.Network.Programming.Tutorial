# Makefile

CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -Werror
INCLUDE = -I./lib
LDFLAGS = -lgflags -lunp
LDPATH = -L./lib

PROGS =	tcp_ping

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

tcp_ping: tcp_ping.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)
