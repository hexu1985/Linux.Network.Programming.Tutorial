# Makefile

RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -std=c++17
INCLUDE = 
LDFLAGS = 
LDPATH = 

PROGS =	tcp_sixteen_client #tcp_sixteen_server

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

tcp_sixteen_client: tcp_sixteen_client.o wrapsock.o error.o
	$(CXX) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LDPATH)

#tcp_sixteen_server: tcp_sixteen_server.o wrapsock.o error.o
#	$(CXX) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LDPATH)
