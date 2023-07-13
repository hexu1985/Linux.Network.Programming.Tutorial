CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -Werror
#AR = ar
#ARFLAGS = rv

OBJECTS = blocks.o sock_ntop.o socket_address.o wraplib.o wrapnetdb.o wrapsock.o wrapunix.o read_buffer.o \
		  socket_istream.o
TARGET = libunp.a

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"
