# Makefile

CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -Werror
INCLUDE = -I./lib
LDFLAGS = -lunp
LDPATH = -L./lib

PROGS =	print_hostname_ip

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

print_hostname_ip: print_hostname_ip.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)

