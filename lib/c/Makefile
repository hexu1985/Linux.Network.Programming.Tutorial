CC = gcc
CFLAGS = -g3 -Wall -Wextra -Werror
#AR = ar
#ARFLAGS = rv

OBJECTS = blocks.o error.o wraplib.o wrapnetdb.o wrapsock.o wrapunix.o sock_ntop.o sock_ntop_port.o connect_nonb.o
TARGET = libunp.a

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"
