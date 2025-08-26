#Επιλογές μεταγλώττισης
CC      = g++
FLAGS   = -std=c++17 -g -O3 -c

#Αρχεία για CheckGroundtrouth
CheckGroundtrouth_OUT = CheckGroundtrouth
OBJS    = CheckGroundtrouth.o ../Includes/Graph/Graph.o ../Includes/Functions/Functions.o
SOURCE  = CheckGroundtrouth.cpp ../Includes/Graph/Graph.cpp ../Includes/Functions/Functions.cpp
HEADER  = CheckGroundtrouth.hpp ../Includes/Graph/Graph.hpp ../Includes/Functions/Functions.hpp


# Αρχεία για τα tests
TEST_SRC = ../Tests/test.cpp   
TEST_OBJS = ../Tests/test.o ../Includes/Graph/Graph.o ../Includes/Functions/Functions.o  
TEST_OUT = test_run


all: $(OBJS)
	$(CC) -g -O3 $(OBJS) -o $(CheckGroundtrouth_OUT)

test: $(TEST_OBJS)
	$(CC) -g -O3 $(TEST_OBJS)  -o $(TEST_OUT)

#Run Tests
run_tests: test
	./$(TEST_OUT)

#Run CheckGroundtrouth
run: CheckGroundtrouth
	./$(CheckGroundtrouth_OUT) ${CheckGroundtrouth_ARGS}

#Μεταγλώττιση CheckGroundtrouth
CheckGroundtrouth.o: CheckGroundtrouth.cpp
	$(CC) $(FLAGS) CheckGroundtrouth.cpp -o CheckGroundtrouth.o

#Μεταγλώττιση Graph
../Includes/Graph/Graph.o: ../Includes/Graph/Graph.cpp
	$(CC) $(FLAGS) ../Includes/Graph/Graph.cpp -o ../Includes/Graph/Graph.o

#Μεταγλώττιση Functions
../Includes/Functions/Functions.o: ../Includes/Functions/Functions.cpp
	$(CC) $(FLAGS) ../Includes/Functions/Functions.cpp -o ../Includes/Functions/Functions.o

#Μεταγλώττιση του test
../Tests/test.o: ../Tests/test.cpp
	$(CC) $(FLAGS) ../Tests/test.cpp -o ../Tests/test.o

#clean
clean:
	rm -f $(OBJS) $(CheckGroundtrouth_OUT) $(TEST_OBJS) $(TEST_OUT)
