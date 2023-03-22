CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -Werror
#AR = ar
#ARFLAGS = rv

OBJECTS = blocks.o utility.o wrapunix.o wrapnetdb.o wrapsock.o socket_address.o
TARGET = libunp.a

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"
