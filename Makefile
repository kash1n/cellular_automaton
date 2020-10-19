CC = g++
CFLAGS = -g -Wall -W -Wextra -std=c++11 -lm --fast-math -O3

a.out: main.o automaton.o
	$(CC) $(CFLAGS) $^
main.o: main.cpp automaton.h
	$(CC) $(CFLAGS) -c $<
automaton.o: automaton.cpp automaton.h
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f *.out *.o
