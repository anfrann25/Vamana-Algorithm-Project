#include <iostream>
#include <map>
#include <list>
#include <vector>
#include "../Functions/Functions.h"

using namespace std;

// Ορισμός της δομής
struct DistanceMapKeys {
    int a, b;
    // Υπερφόρτωση του τελεστή < για την σύγκριση δύο MyStruct
    bool operator<(const DistanceMapKeys& other) const {
        // Συγκρίνουμε πάντα τα ζεύγη ως (min(a, b), max(a, b))
        if (min(a, b) != min(other.a, other.b)) return min(a, b) < min(other.a, other.b);
        return max(a, b) < max(other.a, other.b);
    }
};


class Graph{

    map<int, vector<int>> adjList;

    map<int, vector<float>> cordinates;

    map<DistanceMapKeys, float> DistanceMap;


    int size;
public:
    //Constructor γράφου
    Graph(int R) : size(size) {}
    // Destructor
    ~Graph() {
        // Cleanup if necessary (though in this case, it's automatically handled by containers)
        adjList.clear();
        cordinates.clear();
    }

    //Επιστροφή μεγέθους κόμβων
    int getSize();

    //Προσθήκη ακμής
    void addEdge(int u, int v);

    //Διαγραφή ακμής
    void deleteEdge(int source, int destination);

    //Εκτύπωση γράφου
    void printGraph();

    // Μέθοδος για την προσθήκη συντεταγμένων σε έναν κόμβο.
    void addNode(int node, vector<float>& cords);

    //Μέθοδος για την επιστροφή map με τους γείτονες κάθε κόμβου
    map<int, vector<int>>& getList();

    //Μέθοδος για την επιστροφή της λίστας με τους γείτονες του i κόμβου
    vector<int>& getList_of_node_i(int i);

    //Μέθοδος για την επιστρογή του map τών συντεταγμένων κάθε κόμβου
    map<int, vector<float>>& getCordinates();


    map<DistanceMapKeys, float>& getMedoidMap();

   

    //Μέθοδος για την επιστρογή των συντεταγμένων του κόμβου i
    vector<float>& getCordinates_of_node_i(int i);
    //Μέθοδος για την επιστρογή των συντεταγμένων του κόμβου i
    vector<float> getCordinates_of_node_i_filtered(int i);

    //Μέθοδος για τον έλεγχο άν υπάρχει ένας ο γείτονας.
    bool checkIfNeighboors(int source, int destination);

    //Συνάρτηση για το GreedySearch
    pair<set<Lnodes, CompareValue>, unordered_set<int>> GreedySearch(int S, vector<float>& Xq, int k, int L);

    //Συνάρτηση για το UpdatedGreedySearch
    pair<set<Lnodes, CompareValue>, unordered_set<int>> UpdatedGreedySearch(int S, int XqIndex, int k, int L, float*& xq_distances,vector<int>& randomVec);
 
    //Συνάρτηση για το robust prune
    void RobustPrune(int p, unordered_set<int>& Visited, int a, int R);

    void UpdatedRobustPrune(int p, unordered_set<int>& Visited, int a, int R,  float*& xq_distances,vector<int>& randomVec,float*& neighbors_distances);

    //Συνάρτηση για το FilteredGreedySearch
    pair<set<Lnodes, CompareValue>, unordered_set<int>> FilteredGreedySearch(map<float, float> S, vector<float>& Xq, int k, int L, vector<int>& Fq);
    
    pair<set<Lnodes, CompareValue>, unordered_set<int>> UpdatedFilteredGreedySearch(map<float, float> S, int Xqindex, int k, int L, vector<int>& Fq,float*& xq_distances,vector<int>& randomVec);
    //Συνάρτηση για το filtered robust prune
    void FilteredRobustPrune(int p, unordered_set<int>& Visited, int a, int R);

    void UpdatedFilteredRobustPrune(int p, unordered_set<int>& Visited, int a, int R, float*& xq_distances,vector<int>& randomVec,float*& neighbors_distances);


    //Συνάρτηση που αποθηκεύει τον γράφο σε binary μορφή
    void SaveGraphBinary(const string& filename);

    //Συνάρτηση που φορτώνει τον γράφο απο το binary αρχείο και τον επιστρέφει.
    Graph LoadGraphBinary(const string& filename);


    int UpdatedfindMin(unordered_set<int>& DiffResult,map<int, vector<float>>& cordinates,vector<float>& Xq, int XqIndex, float*& xq_distances,float*& neighbors_distances);
    
    float UpdatedeuklidianDistance(const vector<float>& point_source, const vector<float>& point_dest, int XqIndex, int point_sourceIndex);


    int UpdatedfindFilteredMin(unordered_set<int>& DiffResult, map<int, vector<float>>& cordinates, vector<float>& Xq, int XqIndex, float*& xq_distances,float*& neighbors_distances);


};
