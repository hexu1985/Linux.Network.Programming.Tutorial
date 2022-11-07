CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
INCLUDE = 
LDFLAGS = 
LDLIBS = -lpthread

PROGS =	alarm_thread_timer alarm_thread_timer2

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

alarm_thread_timer: alarm_thread_timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

alarm_thread_timer2: alarm_thread_timer2.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

