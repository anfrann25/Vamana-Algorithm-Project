#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_set>
#include <cmath>
#include <utility>
#include <map>


using namespace std;


//Συνάρτηση υπολογισμού της ευκλείδιας απόστασης
float euklidianDistance(const vector<float>& point_source, const vector<float>& point_dest);


struct Lnodes{
    int index;
    vector<float> values;
    vector<float> xq;
    Lnodes(int ind, vector<float> val, vector<float> Xq) : index(ind), values(val), xq(Xq) {}
};


struct CompareValue {
    bool operator()(const Lnodes& a, const Lnodes& b) const {
        float firstEukl = euklidianDistance(a.values, a.xq);
        float secondEukl = euklidianDistance(b.values, b.xq);
        
        return firstEukl < secondEukl || (firstEukl == secondEukl && a.index > b.index);
    }
};




// Συνάρτηση για τον υπολογισμό του medoid
int findMedoid(vector<vector<float>>& data);


//Συνάρτηση άνοιγμα αρχείου fvec
vector<vector<float>> open_fvec(string fileName);

//Συνάρτηση άνοιγμα αρχείου ivec
vector<vector<int>> open_ivec(string fileName);

//Συνάρτηση που επιστρέφει το id του κόμβου που έχει μικρότερη αποσταση απο το Xq
int findMin(unordered_set<int>& DiffResult, map<int, vector<float>> &cordinates, vector<float>& Xq);

//Συνάρτηση που επιστρέφει τα στοιχεία του L που δέν εμπεριέχονται στο  V
unordered_set<int> checkDiff(set<Lnodes, CompareValue>& pq , unordered_set<int>& V);

//PART 2
map<float, float> findFiltersMedoid(vector <vector<float>>& P, int threshold, set<float>& filters);


// Συνάρτηση για τον υπολογισμό του FilteredMedoid 
int findMedoidForFilter(vector<vector<float>>& data, vector<float>& indexes);


// Συνάρτηση που επιστρέφει το id του κόμβου που έχει μικρότερη αποσταση απο το Xq
int findFilteredMin(unordered_set<int>& DiffResult,map<int, vector<float>>& cordinates,vector<float>& Xq);
//Συνάρτηση που αποθηκεύει το medoidMap σε ένα αρχείο
void SaveMedoidsMapToFile(const string& filename, const std::map<int, int>& medoidsMap);
//Συνάρτηση που φορτώνει το MedoidMap από ένα αρχείο
void LoadMedoidsMapFromFile(const string& filename, map<int, int>& medoidsMap);



