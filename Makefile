CXXFLAGS := -g -Wall -W -Wextra -std=c++11 --fast-math -O3 -MMD
LDLIBS := -lm -lstdc++

main: main.o automaton.o

DEPS := $(shell find -name "*.d")
-include $(DEPS)

clean:
	rm -f main *.o *.d
