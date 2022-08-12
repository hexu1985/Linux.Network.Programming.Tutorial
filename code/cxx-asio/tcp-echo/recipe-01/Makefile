
RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -DASIO_STANDALONE
INCLUDES = -I../../asio/include
LDFLAGS = -lpthread
LDPATH =

SOURCES = $(shell ls *.cpp)
PROGS = $(subst .cpp,,$(SOURCES))
#PROGS = $(SOURCES:%.cpp=%)

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	$(RM) $(PROGS)

%: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
