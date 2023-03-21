# Makefile

CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -Werror
INCLUDE = -I./lib
LDFLAGS = -lunp
LDPATH = -L./lib

PROGS =	get_addr_info_c get_addr_info_py

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

get_addr_info_c: get_addr_info_c.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)

get_addr_info_py: get_addr_info_py.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)
