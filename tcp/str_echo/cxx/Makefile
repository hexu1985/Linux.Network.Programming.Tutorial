# Makefile

CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -Werror -fsanitize=address -fno-omit-frame-pointer
INCLUDE = -I./lib
LDFLAGS = -lasan -lgflags -lunp 
LDPATH = -L./lib

PROGS =	tcp_echo_client tcp_echo_server

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

tcp_echo_client: tcp_client.o str_cli.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)

tcp_echo_server: tcp_server.o str_echo.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)
