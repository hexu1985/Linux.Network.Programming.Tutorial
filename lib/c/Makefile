CC = gcc
CFLAGS = -g3 -Wall -Wextra -Werror
#AR = ar
#ARFLAGS = rv

OBJECTS = blocks.o error.o utility.o wrapnetdb.o wrapsock.o wrapunix.o connect_nonb.o
TARGET = libunp.a

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"
