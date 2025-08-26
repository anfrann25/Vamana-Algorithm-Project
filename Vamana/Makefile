#Επιλογές μεταγλώττισης
CC      = g++
FLAGS = -std=c++17 -g -O3 -c -pthread

#Αρχεία για Vamana
VAMANA_OUT = Vamana
OBJS    = Vamana.o ../Includes/Graph/Graph.o ../Includes/Functions/Functions.o
SOURCE  = Vamana.cpp ../Includes/Graph/Graph.cpp ../Includes/Functions/Functions.cpp
HEADER  = Vamana.hpp ../Includes/Graph/Graph.hpp ../Includes/Functions/Functions.hpp
VAMANA_ARGS = ../Data/siftsmall/siftsmall/siftsmall_base.fvecs ../Data/siftsmall/siftsmall/siftsmall_query.fvecs ../Data/siftsmall/siftsmall/siftsmall_groundtruth.ivecs



#Αρχεία για Vamana
UpdatedVAMANA_OUT = UpdatedVamana
UpdatedOBJS    = UpdatedVamana.o ../Includes/Graph/Graph.o ../Includes/Functions/Functions.o
UpdatedSOURCE  = UpdatedVamana.cpp ../Includes/Graph/Graph.cpp ../Includes/Functions/Functions.cpp
UpdatedHEADER  = UpdatedVamana.hpp ../Includes/Graph/Graph.hpp ../Includes/Functions/Functions.hpp
UpdatedVAMANA_ARGS = ../Data/siftsmall/siftsmall/siftsmall_base.fvecs ../Data/siftsmall/siftsmall/siftsmall_query.fvecs ../Data/siftsmall/siftsmall/siftsmall_groundtruth.ivecs

# Αρχεία για τα tests
TEST_SRC = ../Tests/test.cpp   
TEST_OBJS = ../Tests/test.o Includes/Graph/Graph.o Includes/Functions/Functions.o  
TEST_OUT = test_run


all: $(OBJS) $(UpdatedOBJS)
	$(CC) -g -O3 -pthread $(OBJS) -o $(VAMANA_OUT)
	$(CC) -g -O3 -pthread $(UpdatedOBJS) -o $(UpdatedVAMANA_OUT)


test: $(TEST_OBJS)
	$(CC) -g -O3 -pthread $(TEST_OBJS)  -o $(TEST_OUT)

#Run Tests
run_tests: test
	./$(TEST_OUT)

#Run Vamana
run: Vamana
	./$(VAMANA_OUT) ${VAMANA_ARGS}

runUpdated: UpdatedVamana
	./$(UpdatedVAMANA_OUT) $(UpdatedVAMANA_ARGS)

#Μεταγλώττιση Vamana
Vamana.o: Vamana.cpp
	$(CC) $(FLAGS) Vamana.cpp -o Vamana.o

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
	rm -f $(OBJS) $(VAMANA_OUT) $(TEST_OBJS) $(TEST_OUT) $(UpdatedOBJS) $(UpdatedVAMANA_OUT)
