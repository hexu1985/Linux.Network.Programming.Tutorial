# Makefile

CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDE = -I./lib
LDFLAGS =
LDLIBS = -lgflags
VPATH = ./lib

PROGS =	print_hostname_ip

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

print_hostname_ip: print_hostname_ip.o wrapsock.o wrapnetdb.o error.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

