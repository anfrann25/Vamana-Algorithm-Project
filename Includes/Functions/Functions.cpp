#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <algorithm>
#include <utility>
#include "Functions.h"
#include <map>
#include <string>
#include <sstream>


using namespace std;
//PART 1

//Συνάρτηση υπολογισμού της ευκλείδιας απόστασης
float euklidianDistance(const vector<float>& point_source, const vector<float>& point_dest){
    //Ελέγχουμε αν τα διανύσματα έχουν ίδιες διαστάσεις
    if(point_source.size() != point_dest.size()){
        
        return -1;
    }
    float distance = 0.0;
    for(int i=0;i<point_source.size();i++){
        distance += pow(point_source[i] - point_dest[i], 2);
    }
    return sqrt(distance);
}


//Συνάρτηση άνοιγμα αρχείου fvec
vector<vector<float>> open_fvec(string fileName){
    ifstream file(fileName, ios::binary);

    if(!file.is_open()){
        cout << "Error opening: " << fileName << endl;
        return {};
    }

    //Vector που αποθηκέυει τους κόμβους που διαβάζουμε απο το αρχείο
    vector<vector<float>> nodes;

    //Προσπέλαση στο αρχείο και γέμισμα του Vector nodes
    while(!file.eof()){
        int dimension;

        //Υπολογισμός διάστασης
        file.read(reinterpret_cast<char*>(&dimension), sizeof(int));
        vector<float> vec(dimension);

        //Διαβάζουμε ένα Vector με διάσταση dimension και το αποθηκέυουμε στο Vector nodes
        file.read(reinterpret_cast<char*>(vec.data()), dimension * sizeof(float));
        nodes.push_back(vec);
        
    }

    file.close();

    return nodes;
}



//Συνάρτηση άνοιγμα αρχείου ivec
vector<vector<int>> open_ivec(string fileName){
    ifstream file(fileName, ios::binary);

    // Έλεγχος αν άνοιξε το αρχείο σωστά
    if (!file.is_open()) {
        cout << "Error opening: " << fileName << endl;
        return {};
    }

    //Vector που αποθηκέυει τους κόμβους που διαβάζουμε απο το αρχείο
    vector<vector<int>> nodes;

    // Προσπέλαση στο αρχείο και γέμισμα του Vector nodes
    while (true) {
        int dimension;

        // Υπολογισμός διάστασης
        file.read(reinterpret_cast<char*>(&dimension), sizeof(int));
        if (!file) break; // Έλεγχος αν η ανάγνωση πέτυχε

        vector<int> vec(dimension);

        // Διαβάζουμε το διάνυσμα με διάσταση `dimension` και το αποθηκεύουμε
        file.read(reinterpret_cast<char*>(vec.data()), dimension * sizeof(int));
        if (!file) break; // Έλεγχος αν η ανάγνωση πέτυχε

        nodes.push_back(vec); // Προσθήκη του διανύσματος στον πίνακα
    }

    file.close(); // Κλείσιμο αρχείου
    return nodes;
}

// Συνάρτηση για τον υπολογισμό του medoid
int findMedoid(vector<vector<float>>& data) {
    float minDistanceSum = numeric_limits<float>::max();
    size_t medoidIndex = 0;

    // Για κάθε vector
    for (size_t i = 0; i < data.size(); ++i) {
        float distanceSum = 0.0;
        // Υπολογισμός αποστάσεων από όλα τα υπόλοιπα vectors
        for (size_t j = 0; j < data.size(); ++j) {
            if (i != j) {
                distanceSum += euklidianDistance(data[i], data[j]);
            }
        }

        // Αν αυτό το vector έχει το μικρότερο άθροισμα αποστάσεων, αποθηκεύεται
        if (distanceSum < minDistanceSum) {
            minDistanceSum = distanceSum;
            medoidIndex = i;
        }
    }

    return medoidIndex; // Επιστρέφει το medoid vector
}

//Συνάρτηση που επιστρέφει τα στοιχεία του L που δέν εμπεριέχονται στο  V
unordered_set<int> checkDiff(set<Lnodes, CompareValue>& LResult , unordered_set<int>& Visited ){
    unordered_set<int> result;

    

    for (const auto& node : LResult) {
        if (Visited.find(node.index) == Visited.end()) { // Αν το node δεν ανήκει στο Visited
            result.insert(node.index); // Προσθήκη στο αποτέλεσμα
        }
    }

    return result; // Όλα τα στοιχεία του L που δεν υπάρχουν στο V
}

int findMin(unordered_set<int>& DiffResult,map<int, vector<float>>& cordinates,vector<float>& Xq){
    float minValue = numeric_limits<float>::max();
    int returnValue = -1;

    for (const int& element : DiffResult) {
        float Eukl = euklidianDistance(cordinates[element],Xq);
        if(Eukl < minValue){
            minValue = Eukl;
            returnValue = element;
        }
    }

   return returnValue;
}


//PART 2



int findFilteredMin(unordered_set<int>& DiffResult,map<int, vector<float>>& cordinates,vector<float>& Xq){
    float minValue = numeric_limits<float>::max();
    int returnValue = -1;
    vector<float> cordinates_without_filter;
    // Δημιουργία του νέου vector χωρίς το πρώτο στοιχείο
    
    for (const int& element : DiffResult) {
        vector<float> cordinates_without_filter(cordinates[element].begin() + 4, cordinates[element].end());
        float Eukl = euklidianDistance(cordinates_without_filter,Xq);
        if(Eukl < minValue){
            minValue = Eukl;
            returnValue = element;
        }
    }

   return returnValue;
}




int findMedoidForFilter(vector<vector<float>>& data, vector<float>& indexes) {
    float minDistanceSum = numeric_limits<float>::max();
    size_t medoidIndex = 0;

    // Για κάθε vector
    for (size_t i = 0; i < data.size(); ++i) {
        float distanceSum = 0.0;
        // Υπολογισμός αποστάσεων από όλα τα υπόλοιπα vectors
        for (size_t j = 0; j < data.size(); ++j) {
            if (i != j) {
                distanceSum += euklidianDistance(data[i], data[j]);
            }
        }

        // Αν αυτό το vector έχει το μικρότερο άθροισμα αποστάσεων, αποθηκεύεται
        if (distanceSum < minDistanceSum) {
            minDistanceSum = distanceSum;
            medoidIndex = indexes[i];
        }
    }

    return medoidIndex; // Επιστρέφει το medoid vector
}


// Συνάρτηση για τον υπολογισμό του FilteredMedoid   
 map<float, float> findFiltersMedoid(vector <vector<float>>& P, int threshold, set<float>& filters){
    

    map<float, float> medoidsMap; 
    vector <vector<float>> Pf;

    int filterCounter = 0;
    for (int value : filters) {
        vector<float> newFilterIndices;
        for(int i=0; i < P.size(); i++){
            if(P[i][1] == value){
                newFilterIndices.push_back(i);
            }    
        }
        Pf.push_back(newFilterIndices);  // Προσθέτουμε το νέο διάνυσμα στο Pf
        filterCounter++;

        vector<float> Rf;
        vector<vector<float>> Rf_cordinates;
        srand(time(0));  // Αρχικοποιούμε τον γεννήτρια τυχαίων αριθμών
        if(newFilterIndices.size() >= threshold){
            for (int i = 0; i < threshold; ++i) {
                int index = rand() % newFilterIndices.size();  // Επιλέγουμε τυχαία ένα δείγμα
                // cout<<"index: "<<index<<endl;
                Rf.push_back(newFilterIndices[index]);
            }
            
            

        }else{
            for (int i = 0; i < newFilterIndices.size(); ++i) {
                int index = rand() % newFilterIndices.size();  // Επιλέγουμε τυχαία ένα δείγμα
                Rf.push_back(newFilterIndices[index]);
            }
            
           
        }
        int medoid = rand() % Rf.size();
        int medoid_index = Rf[medoid];
        // Αποθήκευση του medoid στο χάρτη
        medoidsMap[value] = medoid_index;
    }





    

    
    return medoidsMap;
}

void SaveMedoidsMapToFile(const string& filename, const std::map<int, int>& medoidsMap) {
    ofstream outFile(filename, ios::app); // Άνοιγμα του αρχείου σε λειτουργία append
    if (!outFile) {
        cerr << "Error: Unable to open file for writing: " << filename << endl;
        return;
    }

    // Αποθήκευση του medoidsMap για τον κάθε γράφο
    for (const auto& entry : medoidsMap) {
        outFile << entry.first << " " << entry.second << "\n"; // Αποθηκεύουμε τα κλειδιά και τις τιμές
    }

    outFile.close();
}


void LoadMedoidsMapFromFile(const string& filename, map<int, int>& medoidsMap) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error: Unable to open file for reading: " << filename << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        int graph_id, medoid;
        ss >> graph_id >> medoid; // Διαβάζουμε το key (graph_id) και το value (medoid)
        medoidsMap[graph_id] = medoid; // Αποθηκεύουμε στο map
    }

    inFile.close();
}