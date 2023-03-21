# Makefile

CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -Werror
INCLUDE = -I./lib
LDFLAGS = -lgflags -lunp
LDPATH = -L./lib

PROGS =	unix_domain_dgram_client unix_domain_dgram_server

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

unix_domain_dgram_client: unix_domain_dgram_client.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)

unix_domain_dgram_server: unix_domain_dgram_server.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)
