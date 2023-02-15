# Makefile

CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDE = -I./lib
LDFLAGS =
LDLIBS = -lgflags
VPATH = ./lib

PROGS =	unix_domain_dgram_client unix_domain_dgram_server

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

unix_domain_dgram_client: unix_domain_dgram_client.o wrapsock.o error.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

unix_domain_dgram_server: unix_domain_dgram_server.o wrapsock.o error.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)
