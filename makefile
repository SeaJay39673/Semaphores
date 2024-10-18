CC = g++
CFLAGS = -pthread
COMP = $(CC) $(CFLAGS)


all: clean problem1 problem2 problem3 problem4

problem1.o: problem1.cpp
	$(COMP) -c problem1.cpp
problem1: problem1.o
	$(COMP) -o problem1 problem1.o


problem2.o: problem2.cpp
	$(COMP) -c problem2.cpp
problem2: problem2.o
	$(COMP) -o problem2 problem2.o


problem3.o: problem3.cpp
	$(COMP) -c problem3.cpp
problem3: problem3.o
	$(COMP) -o problem3 problem3.o

problem4.o: problem4.cpp
	$(COMP) -c problem4.cpp
problem4: problem4.o
	$(COMP) -o problem4 problem4.o


clean:
	rm -f problem1.o problem1 problem2.o problem2 problem3, problem3.o problem4 problem4.o