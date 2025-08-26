#Επιλογές μεταγλώττισης
CC      = g++
FLAGS   = -std=c++17 -g -O3 -c -pthread

#Αρχεία για FilteredVamana
FilteredVamana_OUT = FilteredVamana
OBJS    = FilteredVamana.o ../Includes/Graph/Graph.o ../Includes/Functions/Functions.o
SOURCE  = FilteredVamana.cpp ../Includes/Graph/Graph.cpp ../Includes/Functions/Functions.cpp
HEADER  = FilteredVamana.hpp ../Includes/Graph/Graph.hpp ../Includes/Functions/Functions.hpp

UpdatedFilteredVamana_OUT = UpdatedFilteredVamana
UpdatedOBJS    = UpdatedFilteredVamana.o ../Includes/Graph/Graph.o ../Includes/Functions/Functions.o
UpdatedSOURCE  = UpdatedFilteredVamana.cpp ../Includes/Graph/Graph.cpp ../Includes/Functions/Functions.cpp
UpdatedHEADER  = UpdatedFilteredVamana.hpp ../Includes/Graph/Graph.hpp ../Includes/Functions/Functions.hpp



# Αρχεία για τα tests
TEST_SRC = ../Tests/test.cpp   
TEST_OBJS = ../Tests/test.o ../Includes/Graph/Graph.o ../Includes/Functions/Functions.o  
TEST_OUT = test_run


all: $(OBJS) $(UpdatedOBJS)
	$(CC) -g -O3 -pthread $(OBJS) -o $(FilteredVamana_OUT)
	$(CC) -g -O3 -pthread $(UpdatedOBJS) -o $(UpdatedFilteredVamana_OUT)
test: $(TEST_OBJS)
	$(CC) -g -O3 $(TEST_OBJS)  -o $(TEST_OUT)

#Run Tests
run_tests: test
	./$(TEST_OUT)

#Run FilteredVamana
run: FilteredVamana
	./$(FilteredVamana_OUT) ${FilteredVamana_ARGS}

runUpdated: UpdatedFilteredVamana
	./$(UpdatedFilteredVamana_OUT) $(UpdatedFilteredVamana_ARGS)

#Μεταγλώττιση FilteredVamana
FilteredVamana.o: FilteredVamana.cpp
	$(CC) $(FLAGS) FilteredVamana.cpp -o FilteredVamana.o

#Μεταγλώττιση FilteredVamana
UpdatedFilteredVamana.o: UpdatedFilteredVamana.cpp
	$(CC) $(FLAGS) UpdatedFilteredVamana.cpp -o UpdatedFilteredVamana.o

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
	rm -f $(OBJS) $(FilteredVamana_OUT) $(TEST_OBJS) $(TEST_OUT) $(UpdatedFilteredVamana_OUT) $(UpdatedOBJS)
