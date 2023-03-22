# Makefile

CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -Werror
INCLUDE = -I./lib
LDFLAGS = -lgflags -lunp
LDPATH = -L./lib

PROGS =	tcp_ping_c tcp_ping_py

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

tcp_ping_c: tcp_ping_c.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcp_ping_py: tcp_ping_py.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)
