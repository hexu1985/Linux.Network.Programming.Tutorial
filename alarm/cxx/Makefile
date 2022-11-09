CXX = g++
CXXFLAGS = -g3 -Wall -Wextra -DDEBUG
INCLUDE = 
LDFLAGS = 
LDLIBS = -lpthread

PROGS =	alarm_thread alarm_thread2 alarm_cond

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

alarm_thread: alarm_thread.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

alarm_thread2: alarm_thread2.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

alarm_cond: alarm_cond.o cond_timer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

