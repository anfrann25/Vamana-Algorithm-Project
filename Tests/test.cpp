#include <iostream>
#include <vector>
#include <cmath>   
#include "acutest.h"
#include "../Includes/Graph/Graph.h"

using namespace std;


//-----------------------FUNCTIONS TEST-----------------------// 
//Tests για τις συναρτήσεις που ορίζονται στο αρχείο Functions.h

//Test για την συνάρτηση ευκλείδειας απόστασης.
void test_euklidianDistance(void) {
    vector<float> point_1 = {1.0, 2.0, 3.0, 4.0};
    vector<float> point_2 = {8.0, 7.0, 6.0, 5.0};
    float result = euklidianDistance(point_1, point_2);
    float expected = 9.16515;

    // Ανοχή σφάλματος
    float tolerance = 0.0001;
    
    TEST_ASSERT(fabs(result - expected) < tolerance);
}

void test_findMedoid(void){
    vector<vector<float>> vec1 ={{1.0, 2.0},
                                {3.0, 0.1},
                                {2.5,2.5}, 
                                {2.0, 2.0}, //Medoid
                                {4.2, 1.4}};

    TEST_ASSERT(findMedoid(vec1) == 3);
}



void test_checkDiff(void) {
    //Set που θέλουμε να ελέγξουμε πόσα στοιχεία του ανήκουν σε ένα άλλο Set
    unordered_set<int> V1 = {1, 2, 3, 4, 5};
    set<Lnodes, CompareValue> Set;

    
    vector<float> xq = {};
    vector<float> invec = {};

    Set.insert(Lnodes(1, invec, xq)); //1
    Set.insert(Lnodes(2, invec, xq)); //2
    Set.insert(Lnodes(3, invec, xq)); //3
    Set.insert(Lnodes(4, invec, xq)); //4
    Set.insert(Lnodes(5, invec, xq)); //5
    Set.insert(Lnodes(6, invec, xq)); //6
    Set.insert(Lnodes(30, invec, xq)); //30
    Set.insert(Lnodes(90, invec, xq)); //90


    //Η συνάρτηση checkDiff(Set, V1) επιστρέφει όσα στοιχεία υπάρχουν στο Set και δεν υπάρχουν στο V1.
    unordered_set<int> result = checkDiff(Set, V1);
    //Περιμένουμε να δούμε οτι τα στοιχεία 6,30,90 υπάρχουν στο Set και όχι στο V1.
    unordered_set<int> expected = {6, 30, 90};

    TEST_ASSERT(result == expected);
}



void test_findMin(void){

    unordered_set<int> DiffResult = {1, 2, 3};
    map<int, vector<float>> cordinates = {
        {1, {1.0, 2.0, 3.0}},
        {2, {4.0, 5.0, 6.0}},
        {3, {7.0, 8.0, 9.0}}
    };

    vector<float> Xq = {2.0, 3.0, 4.0};


    int result = findMin(DiffResult, cordinates, Xq);

    TEST_ASSERT(result == 1);

}

//-----------------------GRAPH TEST-----------------------// 
//Tests για τις συναρτήσεις που ορίζονται στο αρχείο Graph.h


void test_Graph(void){
    //Αρχικοποίηση γράφου
    Graph graph(0);


    //Αρχικοποίηση null vector
    vector<float> cords = {};

    //Προσθήκη ακμών
    //0->1 και 0->2
    graph.addEdge(0,1);
    graph.addEdge(0,2);
    
    //1->1 και 1->2 και 1->3
    graph.addEdge(1,1);
    graph.addEdge(1,2);
    graph.addEdge(1,3);

    //Θα ελέγξουμε οτι οι γείτονες του test_array υπάρχουν στον γράφο
    int test_array[5] = {1, 2, 1, 2, 3};

    int counter = 0;
    for (auto node : graph.getList()) {//Για κάθε κόμβο
        for (int neighboors : node.second) {//Για κάθε γείτονα
            TEST_ASSERT(neighboors == test_array[counter]);
            counter++;
        }
    }

    //Διαγραφή ακμής 1->3  
    graph.deleteEdge(1, 3);
    int test_array_after_delete[5] = {1, 2, 1, 2};

    counter = 0;
    for (auto node : graph.getList()) {//Για κάθε κόμβο
        for (int neighboors : node.second) {//Για κάθε γείτονα
            TEST_ASSERT(neighboors == test_array_after_delete[counter]);
            counter++;
        }
    }


    //Προσθήκη ακμής
    graph.addEdge(2,4);
    bool result = graph.checkIfNeighboors(2, 4);
    TEST_ASSERT(result == true);

    //Διαγραφή ακμής
    graph.deleteEdge(2, 4);
    result = graph.checkIfNeighboors(2, 4);
    TEST_ASSERT(result == false);
}

void test_FilteredGreadySearch(void){
    // Αρχικοποίηση του γράφου με κάποιους κόμβους και συνδέσεις
    Graph graph(10);

    //Προσθήκη κόμβων.
    vector<float> xq = {1,10,-1,-1,10,10};
    vector<float> in0 = {1,10,-1,-1,0,0};
    vector<float> in1 = {1,20,-1,-1,1,1};
    vector<float> in2 = {1,30,-1,-1,2,2};
    vector<float> in3 = {1,10,-1,-1,3,3};
    vector<float> in4 = {1,50,-1,-1,4,4};
    vector<float> in5 = {1,10,-1,-1,5,5};
    vector<float> in6 = {1,70,-1,-1,6,6};
    vector<float> in7 = {1,80,-1,-1,7,7};
    vector<float> in8 = {1,90,-1,-1,8,8};
    vector<float> in9 = {1,10,-1,-1,9,9};

    //Ορισμός των γειτονικών κόμβων
    graph.addNode(0, in0);
    graph.addNode(1, in1);
    graph.addNode(2, in2);
    graph.addNode(3, in3);
    graph.addNode(4, in4);
    graph.addNode(5, in5);
    graph.addNode(6, in6);
    graph.addNode(7, in7);
    graph.addNode(8, in8);
    graph.addNode(9, in9);

    //Προσθήκη ακμών
    graph.addEdge(9,1);
    graph.addEdge(9,2);
    graph.addEdge(9,3);
    graph.addEdge(9,4);
    graph.addEdge(8,5);
    graph.addEdge(7,2);
    graph.addEdge(4,7);
    graph.addEdge(3,9);
    graph.addEdge(2,8);
    graph.addEdge(8,3);
  
    graph.addEdge(5,7);
    graph.addEdge(4,1);
    graph.addEdge(0,5);
    graph.addEdge(0,3);
    graph.addEdge(0,2);
    map<float, float> medoidsMap;
    medoidsMap[10] = 0;
    medoidsMap[20] = 2;
    medoidsMap[30] = 4;
    medoidsMap[40] = 6;
    medoidsMap[50] = 8;
    medoidsMap[60] = 9;
    medoidsMap[70] = 1;
   
    vector<int> filters = {10,20,30,10,50,10,70,80,90,10};



    auto result = graph.FilteredGreedySearch(medoidsMap, xq, 10,20,filters);  
    auto& tempSet = result.first;
    auto& tempVisited = result.second;

    int expected_array[10] = {9,5,3,0};

    int counter=0;
    for(const auto& node : tempSet) {  // Διατρέχουμε κάθε στοιχείο του `set`
        TEST_ASSERT(node.index == expected_array[counter]);
        counter++;
    }

    

   

}


void test_GreedySearch(void){
    // Αρχικοποίηση του γράφου με κάποιους κόμβους και συνδέσεις
    Graph graph(10);
    vector<float> xq = {10,10};
    //Προσθήκη κόμβων.
    vector<float> in0 = {0,0};
    vector<float> in1 = {1,1};
    vector<float> in2 = {2,2};
    vector<float> in3 = {3,3};
    vector<float> in4 = {4,4};
    vector<float> in5 = {5,5};
    vector<float> in6 = {6,6};
    vector<float> in7 = {7,7};
    vector<float> in8 = {8,8};
    vector<float> in9 = {9,9};

    //Ορισμός των γειτονικών κόμβων
    graph.addNode(0, in0);
    graph.addNode(1, in1);
    graph.addNode(2, in2);
    graph.addNode(3, in3);
    graph.addNode(4, in4);
    graph.addNode(5, in5);
    graph.addNode(6, in6);
    graph.addNode(7, in7);
    graph.addNode(8, in8);
    graph.addNode(9, in9);

    
    //Προσθήκη ακμών
    graph.addEdge(9,1);
    graph.addEdge(9,2);
    graph.addEdge(9,3);
    graph.addEdge(9,4);
    graph.addEdge(8,5);
    graph.addEdge(7,2);
    graph.addEdge(4,7);
    graph.addEdge(3,9);
    graph.addEdge(2,8);
    graph.addEdge(8,3);
  
    graph.addEdge(5,7);
    graph.addEdge(4,1);
    graph.addEdge(0,5);
    graph.addEdge(0,3);
    graph.addEdge(0,2);
    graph.addEdge(0,6);


    auto result = graph.GreedySearch(0, xq, 10,20 );  
    auto& tempSet = result.first;
    auto& tempVisited = result.second;
    int expected_array[10] = {9,8,7,6,5,4,3,2,1,0}; 

    int counter=0;
   for (const auto& node : tempSet) {  // Διατρέχουμε κάθε στοιχείο του `set`
        TEST_ASSERT(node.index == expected_array[counter]);
        counter++;
   }
   

}
void test_RobustPrune(void){
    // Αρχικοποίηση του γράφου με κάποιους κόμβους και συνδέσεις
    Graph graph(10);

    //Προσθήκη κόμβων.
    vector<float> in0 = {0,0};
    vector<float> in1 = {1,1};
    vector<float> in2 = {2,2};
    vector<float> in3 = {3,3};
    vector<float> in4 = {4,4};
    vector<float> in5 = {5,5};
    vector<float> in6 = {6,6};
    vector<float> in7 = {7,7};
    vector<float> in8 = {8,8};
    vector<float> in9 = {9,9};

    //Ορισμός των γειτονικών κόμβων
    graph.addNode(0, in0);
    graph.addNode(1, in1);
    graph.addNode(2, in2);
    graph.addNode(3, in3);
    graph.addNode(4, in4);
    graph.addNode(5, in5);
    graph.addNode(6, in6);
    graph.addNode(7, in7);
    graph.addNode(8, in8);
    graph.addNode(9, in9);

    //Προσθήκη ακμών
    graph.addEdge(9,1);
    graph.addEdge(9,2);
    graph.addEdge(9,3);
    graph.addEdge(9,4);
    graph.addEdge(8,5);
    graph.addEdge(7,2);
    graph.addEdge(4,7);
    graph.addEdge(3,6);
    graph.addEdge(2,8);
    graph.addEdge(8,3);
    graph.addEdge(9,3);
    graph.addEdge(5,1);
    graph.addEdge(4,1);
    graph.addEdge(3,8);


    auto result = graph.GreedySearch(9, in0, 5,10 );  
    auto& Visited = result.second;
    int Visited_size_before_RobustPrune = Visited.size();

    graph.RobustPrune(0, Visited, 1.2,10);

    TEST_ASSERT(Visited.size() < Visited_size_before_RobustPrune);
}


void test_FilteredRobustPrune(void){
    // Αρχικοποίηση του γράφου με κάποιους κόμβους και συνδέσεις
    Graph graph(10);
    vector<float> xq = {1,10,-1,-1,10,10};
    //Προσθήκη κόμβων.
    vector<float> in0 = {1,10,-1,-1,0,0};
    vector<float> in1 = {1,20,-1,-1,1,1};
    vector<float> in2 = {1,30,-1,-1,2,2};
    vector<float> in3 = {1,10,-1,-1,3,3};
    vector<float> in4 = {1,50,-1,-1,4,4};
    vector<float> in5 = {1,10,-1,-1,5,5};
    vector<float> in6 = {1,70,-1,-1,6,6};
    vector<float> in7 = {1,80,-1,-1,7,7};
    vector<float> in8 = {1,90,-1,-1,8,8};
    vector<float> in9 = {1,10,-1,-1,9,9};

    //Ορισμός των γειτονικών κόμβων
    graph.addNode(0, in0);
    graph.addNode(1, in1);
    graph.addNode(2, in2);
    graph.addNode(3, in3);
    graph.addNode(4, in4);
    graph.addNode(5, in5);
    graph.addNode(6, in6);
    graph.addNode(7, in7);
    graph.addNode(8, in8);
    graph.addNode(9, in9);

    
    //Προσθήκη ακμών
    graph.addEdge(9,1);
    graph.addEdge(9,2);
    graph.addEdge(9,3);
    graph.addEdge(9,4);
    graph.addEdge(8,5);
    graph.addEdge(7,2);
    graph.addEdge(4,7);
    graph.addEdge(3,9);
    graph.addEdge(2,8);
    graph.addEdge(8,3);
  
    graph.addEdge(5,7);
    graph.addEdge(4,1);
    graph.addEdge(0,5);
    graph.addEdge(0,3);
    graph.addEdge(0,2);
    graph.addEdge(0,6);
    map<float, float> medoidsMap;
    medoidsMap[10] = 0;
    medoidsMap[20] = 2;
    medoidsMap[30] = 4;
    medoidsMap[40] = 6;
    medoidsMap[50] = 8;
    medoidsMap[60] = 9;
    medoidsMap[70] = 1;
   
    vector<int> filters = {10,20,30,10,50,10,70,80,90,10};

    auto result = graph.FilteredGreedySearch(medoidsMap, xq, 10, 20, filters);
    auto& Visited = result.second;
    int Visited_size_before_RobustPrune = Visited.size();

    graph.FilteredRobustPrune(0, Visited, 1.2,10);

    TEST_ASSERT(Visited.size() < Visited_size_before_RobustPrune);
}


TEST_LIST = {
    //Functions
    {"euclideanDistance", test_euklidianDistance},
    {"findMedoid", test_findMedoid},
    {"checkDiff", test_checkDiff},
    {"findMin", test_findMin},
    {"Graph", test_Graph},
    {"GreedySearch", test_GreedySearch},
    {"RobustPrune", test_RobustPrune},
    {"FilteredGreedySearch", test_FilteredGreadySearch},
    {"FilteredRobustPrune", test_FilteredRobustPrune},
    { NULL, NULL }  // Τέλος λίστας
};



