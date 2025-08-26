# Επιλογές μεταγλώττισης
CC      = g++
FLAGS   = -std=c++17 -g -O3 -c
LDFLAGS = -pthread -std=c++17 -g -O3

# Αρχεία για StitchedVamana
StitchedVamana_OUT = StitchedVamana
OBJS    = StitchedVamana.o ../Includes/Graph/Graph.o ../Includes/Functions/Functions.o
SOURCE  = StitchedVamana.cpp ../Includes/Graph/Graph.cpp ../Includes/Functions/Functions.cpp
HEADER  =  ../Includes/Graph/Graph.h ../Includes/Functions/Functions.h

# Αρχεία για UpdatedStitchedVamana
UpdatedStitchedVamana_OUT = UpdatedStitchedVamana
UpdatedOBJS    = UpdatedStitchedVamana.o ../Includes/Graph/Graph.o ../Includes/Functions/Functions.o
UpdatedSOURCE  = UpdatedStitchedVamana.cpp ../Includes/Graph/Graph.cpp ../Includes/Functions/Functions.cpp
UpdatedHEADER  = ../Includes/Graph/Graph.h ../Includes/Functions/Functions.h

# Αρχεία για τα tests
TEST_SRC = ../Tests/test.cpp   
TEST_OBJS = ../Tests/test.o ../Includes/Graph/Graph.o ../Includes/Functions/Functions.o  
TEST_OUT = test_run

# Στόχοι
all: $(StitchedVamana_OUT) $(UpdatedStitchedVamana_OUT)

test: $(TEST_OUT)

# Τρέξιμο των προγραμμάτων
run_tests: test
	./$(TEST_OUT)

run: $(StitchedVamana_OUT)
	./$(StitchedVamana_OUT) ${StitchedVamana_ARGS}

runUpdated: $(UpdatedStitchedVamana_OUT)
	./$(UpdatedStitchedVamana_OUT) $(UpdatedStitchedVamana_ARGS)

# Μεταγλώττιση StitchedVamana
$(StitchedVamana_OUT): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(StitchedVamana_OUT)

StitchedVamana.o: StitchedVamana.cpp 
	$(CC) $(FLAGS) StitchedVamana.cpp -o StitchedVamana.o

# Μεταγλώττιση UpdatedStitchedVamana
$(UpdatedStitchedVamana_OUT): $(UpdatedOBJS)
	$(CC) $(LDFLAGS) $(UpdatedOBJS) -o $(UpdatedStitchedVamana_OUT)

UpdatedStitchedVamana.o: UpdatedStitchedVamana.cpp 
	$(CC) $(FLAGS) UpdatedStitchedVamana.cpp -o UpdatedStitchedVamana.o

# Μεταγλώττιση Graph
../Includes/Graph/Graph.o: ../Includes/Graph/Graph.cpp ../Includes/Graph/Graph.h
	$(CC) $(FLAGS) ../Includes/Graph/Graph.cpp -o ../Includes/Graph/Graph.o

# Μεταγλώττιση Functions
../Includes/Functions/Functions.o: ../Includes/Functions/Functions.cpp ../Includes/Functions/Functions.h
	$(CC) $(FLAGS) ../Includes/Functions/Functions.cpp -o ../Includes/Functions/Functions.o

# Μεταγλώττιση του test
$(TEST_OUT): $(TEST_OBJS)
	$(CC) $(LDFLAGS) $(TEST_OBJS) -o $(TEST_OUT)

../Tests/test.o: ../Tests/test.cpp ../Includes/Graph/Graph.h ../Includes/Functions/Functions.h
	$(CC) $(FLAGS) ../Tests/test.cpp -o ../Tests/test.o

# Καθαρισμός
clean:
	rm -f $(OBJS) $(StitchedVamana_OUT) $(TEST_OBJS) $(TEST_OUT) $(UpdatedOBJS) $(UpdatedStitchedVamana_OUT)
