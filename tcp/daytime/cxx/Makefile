# Makefile

CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -Werror -std=c++17
INCLUDE = -I./lib
LDFLAGS = -lgflags -lunp
LDPATH = -L./lib

PROGS =	tcp_daytime_client tcp_daytime_server

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

tcp_daytime_client: tcp_daytime_client.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcp_daytime_server: tcp_daytime_server.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)
