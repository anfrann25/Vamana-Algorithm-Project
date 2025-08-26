#include <iostream>
#include <map>
#include <list>
#include "Graph.h"
#include <algorithm>
#include <set>
#include <unordered_set>
#include <cstdint>


using namespace std;



//Επιστροφή μεγέθους κόμβων
int Graph::getSize(){
    return this->size;
}


//Προσθήκη ακμής
void Graph::addEdge(int u, int v){
    adjList[u].emplace_back(v); // Προσθέτει τον v στη λίστα γειτόνων του u.
}

//Διαγραφή ακμής
void Graph::deleteEdge(int source,int destination){
  // Αφαίρεση του destination από τη λίστα γειτόνων του source.
    adjList[source].erase(remove(adjList[source].begin(), adjList[source].end(), destination), adjList[source].end());

}


//Εκτύπωση γράφου
void Graph::printGraph(){
    
    int counter = 0;
    for (auto node : adjList) {
        // if(counter == 1000) break;
        // if(cordinates[node.first][0] == 0){
            cout<<node.first<<" ";
        //     cout << endl;
        // }
        for (int neighboors : node.second) {
            cout<<neighboors <<" ";
            
            
        }
         cout << endl;
        counter++;
        
        
    }
    // for (auto node : cordinates) {
    //     cout << "cordinates: " << node.first << ": ";
    //     for (float cordinates : node.second) {
    //         cout << cordinates << " ";
    //     }
    //     cout << endl;
    // }


}



// Μέθοδος για την προσθήκη συντεταγμένων σε έναν κόμβο.
void Graph::addNode(int node, vector<float>& cords){     
    cordinates[node] = cords;
}

 //Μέθοδος για την επιστροφή map με τους γείτονες κάθε κόμβου
map<int, vector<int>>& Graph::getList(){
    return adjList;
}
//Μέθοδος για την επιστροφή της λίστας με τους γείτονες του i κόμβου
vector<int>& Graph::getList_of_node_i(int i){
    return adjList[i];
}

//Μέθοδος για την επιστρογή του map τών συντεταγμένων κάθε κόμβου
map<int, vector<float>>& Graph::getCordinates(){
    return cordinates;
}


map<DistanceMapKeys, float>& Graph::getMedoidMap(){
    return DistanceMap;
}


//Μέθοδος για την επιστρογή του map τών συντεταγμένων κάθε κόμβου
vector<float>& Graph::getCordinates_of_node_i(int i){
    return cordinates[i];
}
//Μέθοδος για την επιστρογή του map τών συντεταγμένων κάθε κόμβου
vector<float> Graph::getCordinates_of_node_i_filtered(int i){
     if (i >= 0 && i < cordinates.size() && cordinates[i].size() > 4) {
        // Επιστρέφει τα στοιχεία από την 4η θέση και μετά
        return vector<float>(cordinates[i].begin() + 4, cordinates[i].end());
    } else {
        // Επιστρέφει έναν άδειο vector ή κάποιο default (ανάλογα με την ανάγκη σου)
        return vector<float>();  // Επιστρέφει άδειο vector αν το i είναι εκτός ορίων ή αν δεν υπάρχουν αρκετά στοιχεία
    }
}

//Mέθοδος που επιστρέφει true/false ανάλογα άν βρέθηκε ο γείτονας source -> destination
bool Graph::checkIfNeighboors(int source, int destination){
    auto it = find(adjList[source].begin(), adjList[source].end(), destination);
    if (it != adjList[source].end()) {
        return true;
    }else {
        return false;    
    }
}

//Συνάρτηση για το GreedySearch
pair<set<Lnodes, CompareValue>, unordered_set<int>> Graph::GreedySearch(int S, vector<float>& Xq, int k, int L){
    unordered_set<int> Visited;
    set<Lnodes, CompareValue> LResult;
    LResult.insert(Lnodes(S, cordinates[S], Xq));
    

    unordered_set<int> DiffResult = checkDiff(LResult,Visited); //Επιστροφή στοιχείων που εμπεριέχονται στο L και δεν εμπεριέχονται στο V
    while(!DiffResult.empty()){
         //Όσο όλα τα στοιχεία του L δεν υπάρχουν και στον V
        int p = findMin(DiffResult,cordinates,Xq);
        //Βάζουμε όλους του γείτονες του p στο Lresult και το p στο visited
        for (int neighbors : adjList[p]){
            Lnodes node(neighbors, cordinates[neighbors], Xq);
            if (LResult.find(node) == LResult.end()) { // Αν δεν υπάρχει ήδη στο LResult
                LResult.insert(node);
            }
        }
        Visited.insert(p);

        //εαν το μέγεθος της ουράς προτεραιότητας Lresult ξεπεράσει το L κρατάμε τα L κοντινότερα σημεία στο xq
        if (LResult.size() > L) {
            auto it = LResult.end();
            advance(it, - (LResult.size() - L)); // Μετακινήσου στο σωστό σημείο
            LResult.erase(it, LResult.end()); // Αφαίρεσε τα υπόλοιπα στοιχεία
        }

             
        DiffResult = checkDiff(LResult,Visited);

    }

    if (LResult.size() > k) {
        auto it = LResult.end();
        advance(it, - (LResult.size() - k)); // Μετακινήσου στο σωστό σημείο
        LResult.erase(it, LResult.end()); // Αφαίρεσε τα υπόλοιπα στοιχεία
    }

    //Επιστρέφουμε τα K κοντινότερα σημεία στο xq από το L 
   
    return make_pair(LResult, Visited);
}


//Συνάρτηση για το UpdatedGreedySearch
pair<set<Lnodes, CompareValue>, unordered_set<int>> Graph::UpdatedGreedySearch(int S, int XqIndex, int k, int L,  float*& xq_distances,vector<int>& randomVec){
    float* neighbors_distances = new float[1];
    neighbors_distances[0] = -1;
    unordered_set<int> Visited;
    set<Lnodes, CompareValue> LResult;
    vector<float>& Xq = cordinates[XqIndex];
    LResult.insert(Lnodes(S, cordinates[S], Xq));
    unordered_set<int> DiffResult = checkDiff(LResult,Visited); //Επιστροφή στοιχείων που εμπεριέχονται στο L και δεν εμπεριέχονται στο V
    while(!DiffResult.empty()){
         //Όσο όλα τα στοιχεία του L δεν υπάρχουν και στον V
        int p = UpdatedfindMin(DiffResult,cordinates,Xq, XqIndex, xq_distances, neighbors_distances);
        //Βάζουμε όλους του γείτονες του p στο Lresult και το p στο visited
        for (int neighbors : adjList[p]){
            Lnodes node(neighbors, cordinates[neighbors], Xq);
            if (LResult.find(node) == LResult.end()) { // Αν δεν υπάρχει ήδη στο LResult
                LResult.insert(node);
            }
        }
        Visited.insert(p);

        //εαν το μέγεθος της ουράς προτεραιότητας Lresult ξεπεράσει το L κρατάμε τα L κοντινότερα σημεία στο xq
        if (LResult.size() > L) {
            auto it = LResult.end();
            advance(it, - (LResult.size() - L)); // Μετακινήσου στο σωστό σημείο
            LResult.erase(it, LResult.end()); // Αφαίρεσε τα υπόλοιπα στοιχεία
        }

             
        DiffResult = checkDiff(LResult,Visited);

    }

    if (LResult.size() > k) {
        auto it = LResult.end();
        advance(it, - (LResult.size() - k)); // Μετακινήσου στο σωστό σημείο
        LResult.erase(it, LResult.end()); // Αφαίρεσε τα υπόλοιπα στοιχεία
    }

    //Επιστρέφουμε τα K κοντινότερα σημεία στο xq από το L 
    delete[] neighbors_distances;
    return make_pair(LResult, Visited);
}



//Συνάρτηση για το robust prune
void Graph::RobustPrune(int p, unordered_set<int>& Visited, int a, int R){
    //Βάζουμε στο visited τους γείτονες του p
    for (int neighboors : adjList[p]){
        Visited.insert(neighboors);
        
    }
    //Βγάζουμε από το visited το ίδιο το p αν υπάρχει
    Visited.erase(p);
    //διαγράφουμε όλους τους γείτονες του p
    adjList[p].clear();
    while(!Visited.empty()){
        int p_star = findMin(Visited,cordinates,cordinates[p]);
        adjList[p].emplace_back(p_star);
        if(adjList[p].size() == R){
            break;

        }
        //Βρίσκουμε τους κόμβους που πρέπει να διαγραφούν και τους τοποθετούμε σε ένα set
        unordered_set <int> nodes_to_remove;
        for(const auto& node: Visited){
            if((a * euklidianDistance(cordinates[p_star],cordinates[node])) <= euklidianDistance(cordinates[p],cordinates[node])){
                nodes_to_remove.insert(node);
            }
        }
        //Διάγραφουμε από το Visited όλους τους κόμβους που βάλαμε στο set nodes_to_remove
        for(const auto& node: nodes_to_remove){
            Visited.erase(node);
        }
    }
}

//Συνάρτηση για το robust prune
void Graph::UpdatedRobustPrune(int p, unordered_set<int>& Visited, int a, int R,  float*& xq_distances,vector<int>& randomVec,float*& neighbors_distances){
    //Βάζουμε στο visited τους γείτονες του p
    for (int neighboors : adjList[p]){
        Visited.insert(neighboors);
        
    }
    //Βγάζουμε από το visited το ίδιο το p αν υπάρχει
    Visited.erase(p);
    //διαγράφουμε όλους τους γείτονες του p
    adjList[p].clear();
    while(!Visited.empty()){
        int p_star = UpdatedfindMin(Visited,cordinates,cordinates[p],p, xq_distances,neighbors_distances);
        adjList[p].emplace_back(p_star);
        if(adjList[p].size() == R){
            break;

        }
        //Βρίσκουμε τους κόμβους που πρέπει να διαγραφούν και τους τοποθετούμε σε ένα set
        unordered_set <int> nodes_to_remove;
        for(const auto& node: Visited){
            if(xq_distances[0]!=-1){
                if((a * UpdatedeuklidianDistance(cordinates[p_star],cordinates[node],p_star,node)) <= xq_distances[node]){
                    nodes_to_remove.insert(node);
                }
            }else{
                if((a * UpdatedeuklidianDistance(cordinates[p_star],cordinates[node],p_star,node)) <= UpdatedeuklidianDistance(cordinates[p],cordinates[node],p,node)){
                    nodes_to_remove.insert(node);
                }
            }
        }
        //Διάγραφουμε από το Visited όλους τους κόμβους που βάλαμε στο set nodes_to_remove
        for(const auto& node: nodes_to_remove){
            Visited.erase(node);
        }
    }
}

//Συνάρτηση για το FilteredGreedySearch
pair<set<Lnodes, CompareValue>, unordered_set<int>> Graph::FilteredGreedySearch(map<float, float> S, vector<float>& Xq, int k, int L, vector<int>& Fq){
    unordered_set<int> Visited;
    set<Lnodes, CompareValue> LResult;

    //has a filter
    if(Xq[0]==1){
        for (const auto& element : S) {
            if(element.first == Xq[1]){
                vector<float> cordinates_neighbor;
                vector<float> cordinates_xq;
                for(int i=4;i<cordinates[element.second].size();i++){
                    cordinates_neighbor.push_back(cordinates[element.second][i]);
                    cordinates_xq.push_back(Xq[i]);
                }
                LResult.insert(Lnodes(element.second, cordinates_neighbor, cordinates_xq));
            }
        }
    }
    //has no filter
    if(Xq[0] == 0){
        for (const auto& element : S) {
            vector<float> cordinates_neighbor;
            vector<float> cordinates_xq;
            for(int i=4;i<cordinates[element.second].size();i++){
                cordinates_neighbor.push_back(cordinates[element.second][i]);
                cordinates_xq.push_back(Xq[i]);
            }
            LResult.insert(Lnodes(element.second, cordinates_neighbor, cordinates_xq));
        }

    }
    vector<float> xq_cords;
    for(int i=4;i<Xq.size();i++){
        xq_cords.push_back(Xq[i]);
    }

    unordered_set<int> DiffResult = checkDiff(LResult,Visited); //Επιστροφή στοιχείων που εμπεριέχονται στο L και δεν εμπεριέχονται στο V
    while(!DiffResult.empty()){
        //Όσο όλα τα στοιχεία του L δεν υπάρχουν και στον V
        int p = findFilteredMin(DiffResult,cordinates,xq_cords);
        //Βάζουμε όλους του γείτονες του p στο Lresult και το p στο visited
        Visited.insert(p);
        for (int neighbors : adjList[p]){
            vector<float> cordinates_neighbor;
            for(int i=4;i<cordinates[neighbors].size();i++){
                cordinates_neighbor.push_back(cordinates[neighbors][i]);
            }
            Lnodes node(neighbors, cordinates_neighbor, xq_cords);
            if(Xq[0] == 1){
                if (LResult.find(node) == LResult.end() && cordinates[neighbors][1] == Xq[1] && Visited.find(neighbors) == Visited.end() ) { // Αν δεν υπάρχει ήδη στο LResult
                    LResult.insert(node);
                }
            }
            if(Xq[0]==0){
                if (LResult.find(node) == LResult.end() &&  Visited.find(neighbors) == Visited.end() ) { // Αν δεν υπάρχει ήδη στο LResult
                    LResult.insert(node);
                }
            }
            
        }

        //εαν το μέγεθος της ουράς προτεραιότητας Lresult ξεπεράσει το L κρατάμε τα L κοντινότερα σημεία στο xq
        if (LResult.size() > L) {
            auto it = LResult.end();
            advance(it, - (LResult.size() - L)); // Μετακινήσου στο σωστό σημείο
            LResult.erase(it, LResult.end()); // Αφαίρεσε τα υπόλοιπα στοιχεία
        }

             
        DiffResult = checkDiff(LResult,Visited);

    }

    if (LResult.size() > k) {
        auto it = LResult.end();
        advance(it, - (LResult.size() - k)); // Μετακινήσου στο σωστό σημείο
        LResult.erase(it, LResult.end()); // Αφαίρεσε τα υπόλοιπα στοιχεία
    }

    //Επιστρέφουμε τα K κοντινότερα σημεία στο xq από το L 

    return make_pair(LResult, Visited);

}

//Συνάρτηση για το FilteredGreedySearch
pair<set<Lnodes, CompareValue>, unordered_set<int>> Graph::UpdatedFilteredGreedySearch(map<float, float> S, int Xqindex, int k, int L, vector<int>& Fq,float*& xq_distances,vector<int>& randomVec){
    float* neighbors_distances = new float[1];
    neighbors_distances[0] = -1;
    unordered_set<int> Visited;
    set<Lnodes, CompareValue> LResult;
    vector<float> Xq = cordinates[Xqindex];
    //has a filter
    if(Xq[0]==1){
        for (const auto& element : S) {
            if(element.first == Xq[1]){
                vector<float> cordinates_neighbor;
                vector<float> cordinates_xq;
                for(int i=4;i<cordinates[element.second].size();i++){
                    cordinates_neighbor.push_back(cordinates[element.second][i]);
                    cordinates_xq.push_back(Xq[i]);
                }
                LResult.insert(Lnodes(element.second, cordinates_neighbor, cordinates_xq));
            }
        }
    }
    //has no filter
    if(Xq[0] == 0){
        for (const auto& element : S) {
            vector<float> cordinates_neighbor;
            vector<float> cordinates_xq;
            for(int i=4;i<cordinates[element.second].size();i++){
                cordinates_neighbor.push_back(cordinates[element.second][i]);
                cordinates_xq.push_back(Xq[i]);
            }
            LResult.insert(Lnodes(element.second, cordinates_neighbor, cordinates_xq));
        }

    }
    vector<float> xq_cords;
    for(int i=4;i<Xq.size();i++){
        xq_cords.push_back(Xq[i]);
    }
    unordered_set<int> DiffResult = checkDiff(LResult,Visited); //Επιστροφή στοιχείων που εμπεριέχονται στο L και δεν εμπεριέχονται στο V
    while(!DiffResult.empty()){
        //Όσο όλα τα στοιχεία του L δεν υπάρχουν και στον V
        int p = UpdatedfindFilteredMin(DiffResult,cordinates,xq_cords,Xqindex,xq_distances,neighbors_distances);
        //Βάζουμε όλους του γείτονες του p στο Lresult και το p στο visited
        Visited.insert(p);
        for (int neighbors : adjList[p]){
            vector<float> cordinates_neighbor;
            for(int i=4;i<cordinates[neighbors].size();i++){
                cordinates_neighbor.push_back(cordinates[neighbors][i]);
            }
            Lnodes node(neighbors, cordinates_neighbor, xq_cords);
            if(Xq[0] == 1){
                if (LResult.find(node) == LResult.end() && cordinates[neighbors][1] == Xq[1] && Visited.find(neighbors) == Visited.end() ) { // Αν δεν υπάρχει ήδη στο LResult   
                    LResult.insert(node);
                }
            }
            if(Xq[0]==0){
                if (LResult.find(node) == LResult.end() &&  Visited.find(neighbors) == Visited.end() ) { // Αν δεν υπάρχει ήδη στο LResult

                    LResult.insert(node);
                }
            }
            
        }
        // Visited.insert(p);

        //εαν το μέγεθος της ουράς προτεραιότητας Lresult ξεπεράσει το L κρατάμε τα L κοντινότερα σημεία στο xq
        if (LResult.size() > L) {
            auto it = LResult.end();
            advance(it, - (LResult.size() - L)); // Μετακινήσου στο σωστό σημείο
            LResult.erase(it, LResult.end()); // Αφαίρεσε τα υπόλοιπα στοιχεία
        }

             
        DiffResult = checkDiff(LResult,Visited);

    }

    if (LResult.size() > k) {
        auto it = LResult.end();
        advance(it, - (LResult.size() - k)); // Μετακινήσου στο σωστό σημείο
        LResult.erase(it, LResult.end()); // Αφαίρεσε τα υπόλοιπα στοιχεία
    }
    delete [] neighbors_distances;

    //Επιστρέφουμε τα K κοντινότερα σημεία στο xq από το L 

    return make_pair(LResult, Visited);

}

//Συνάρτηση για το Filteredrobust prune
void Graph::FilteredRobustPrune(int p, unordered_set<int>& Visited, int a, int R){
    //Βάζουμε στο visited τους γείτονες του p
    for (int neighboors : adjList[p]){
        Visited.insert(neighboors);
    }
    //Βγάζουμε από το visited το ίδιο το p αν υπάρχει
    Visited.erase(p);
    //διαγράφουμε όλους τους γείτονες του p
    adjList[p].clear();

    vector<float> PCords_without_filters;
    for(int i=4;i<cordinates[p].size();i++){
        PCords_without_filters.push_back(cordinates[p][i]);
    }
    while(!Visited.empty()){
        int p_star = findFilteredMin(Visited,cordinates,PCords_without_filters);
        adjList[p].emplace_back(p_star);
        if(adjList[p].size() == R){
            break;
        }

        //Βρίσκουμε τους κόμβους που πρέπει να διαγραφούν και τους τοποθετούμε σε ένα set
        unordered_set <int> nodes_to_remove;
        for(const auto& node: Visited){
            if(cordinates[p][1] == cordinates[node][1] && cordinates[p][1] != cordinates[p_star][1]){//**** *
                continue;
            }
            vector<float> NodeCords_without_filters;
            vector<float> PstarCords_without_filters;
            for(int i=4;i<cordinates[node].size();i++){
                NodeCords_without_filters.push_back(cordinates[node][i]);
                PstarCords_without_filters.push_back(cordinates[p_star][i]);
            }
            if((a * euklidianDistance(PstarCords_without_filters,NodeCords_without_filters)) <= euklidianDistance(PCords_without_filters,NodeCords_without_filters)){
                nodes_to_remove.insert(node);
            }
        }
        //Διάγραφουμε από το Visited όλους τους κόμβους που βάλαμε στο set nodes_to_remove
        for(const auto& node: nodes_to_remove){
            Visited.erase(node);
        }
    }
}


//Συνάρτηση για το Filteredrobust prune
void Graph::UpdatedFilteredRobustPrune(int p, unordered_set<int>& Visited, int a, int R, float*& xq_distances,vector<int>& randomVec,float*& neighbors_distances){
    //Βάζουμε στο visited τους γείτονες του p
    for (int neighboors : adjList[p]){
        Visited.insert(neighboors);
    }
    //Βγάζουμε από το visited το ίδιο το p αν υπάρχει
    Visited.erase(p);
    //διαγράφουμε όλους τους γείτονες του p
    adjList[p].clear();

    vector<float> PCords_without_filters;
    for(int i=4;i<cordinates[p].size();i++){
        PCords_without_filters.push_back(cordinates[p][i]);
    }
    while(!Visited.empty()){
        int p_star = UpdatedfindFilteredMin(Visited, cordinates, PCords_without_filters, p , xq_distances, neighbors_distances);
        adjList[p].emplace_back(p_star);
        if(adjList[p].size() == R){
            break;
        }

        //Βρίσκουμε τους κόμβους που πρέπει να διαγραφούν και τους τοποθετούμε σε ένα set
        unordered_set <int> nodes_to_remove;
        for(const auto& node: Visited){
            if(cordinates[p][1] == cordinates[node][1] && cordinates[p][1] != cordinates[p_star][1]){//**** *
                continue;
            }
            vector<float> NodeCords_without_filters;
            vector<float> PstarCords_without_filters;
            for(int i=4;i<cordinates[node].size();i++){
                NodeCords_without_filters.push_back(cordinates[node][i]);
                PstarCords_without_filters.push_back(cordinates[p_star][i]);
            }
            if((a * euklidianDistance(PstarCords_without_filters,NodeCords_without_filters)) <= euklidianDistance(PCords_without_filters,NodeCords_without_filters)){
                nodes_to_remove.insert(node);
            }
        }
        //Διάγραφουμε από το Visited όλους τους κόμβους που βάλαμε στο set nodes_to_remove
        for(const auto& node: nodes_to_remove){
            Visited.erase(node);
        }
    }
}



void Graph::SaveGraphBinary(const string& filename) {
    ofstream outFile(filename, ios::binary);
    if (!outFile) {
        cerr << "Error: Unable to open file for writing: " << filename << endl;
        return;
    }

    // Αποθήκευση του μεγέθους (size)
    outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

    // Αποθήκευση των ακμών (adjList)
    uint32_t adjListSize = adjList.size();
    outFile.write(reinterpret_cast<const char*>(&adjListSize), sizeof(adjListSize));
    for (const auto& [node, neighbors] : adjList) {
        outFile.write(reinterpret_cast<const char*>(&node), sizeof(node));
        uint32_t numNeighbors = neighbors.size();
        outFile.write(reinterpret_cast<const char*>(&numNeighbors), sizeof(numNeighbors));
        outFile.write(reinterpret_cast<const char*>(neighbors.data()), numNeighbors * sizeof(int));
    }

    // Αποθήκευση των συντεταγμένων (cordinates)
    uint32_t cordinatesSize = cordinates.size();
    outFile.write(reinterpret_cast<const char*>(&cordinatesSize), sizeof(cordinatesSize));
    for (const auto& [node, coords] : cordinates) {
        outFile.write(reinterpret_cast<const char*>(&node), sizeof(node));
        uint32_t numCoords = coords.size();
        outFile.write(reinterpret_cast<const char*>(&numCoords), sizeof(numCoords));
        outFile.write(reinterpret_cast<const char*>(coords.data()), numCoords * sizeof(float));
    }

    outFile.close();
}



Graph Graph::LoadGraphBinary(const string& filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Error: Unable to open file for reading: " << filename << endl;
        throw runtime_error("File not found.");
    }

    // Δημιουργία νέου αντικειμένου Graph
    Graph loadedGraph(0);

    // Φόρτωση του μεγέθους (size)
    inFile.read(reinterpret_cast<char*>(&loadedGraph.size), sizeof(loadedGraph.size));

    // Φόρτωση των ακμών (adjList)
    uint32_t adjListSize;
    inFile.read(reinterpret_cast<char*>(&adjListSize), sizeof(adjListSize));
    for (uint32_t i = 0; i < adjListSize; ++i) {
        int node;
        inFile.read(reinterpret_cast<char*>(&node), sizeof(node));
        uint32_t numNeighbors;
        inFile.read(reinterpret_cast<char*>(&numNeighbors), sizeof(numNeighbors));
        vector<int> neighbors(numNeighbors);
        inFile.read(reinterpret_cast<char*>(neighbors.data()), numNeighbors * sizeof(int));
        loadedGraph.adjList[node] = neighbors;
    }

    // Φόρτωση των συντεταγμένων (cordinates)
    uint32_t cordinatesSize;
    inFile.read(reinterpret_cast<char*>(&cordinatesSize), sizeof(cordinatesSize));
    for (uint32_t i = 0; i < cordinatesSize; ++i) {
        int node;
        inFile.read(reinterpret_cast<char*>(&node), sizeof(node));
        uint32_t numCoords;
        inFile.read(reinterpret_cast<char*>(&numCoords), sizeof(numCoords));
        vector<float> coords(numCoords);
        inFile.read(reinterpret_cast<char*>(coords.data()), numCoords * sizeof(float));
        loadedGraph.cordinates[node] = coords;
    }

    inFile.close();
    return loadedGraph;
}

//Συνάρτηση υπολογισμού της ευκλείδιας απόστασης
float Graph::UpdatedeuklidianDistance(const vector<float>& point_source, const vector<float>& point_dest, int XqIndex, int point_sourceIndex){
    //Ελέγχουμε αν τα διανύσματα έχουν ίδιες διαστάσεις
    if(point_source.size() != point_dest.size()){
        return -1;
    }
    double distance = 0.0;

    for(int i=0;i<point_source.size();i++){
        distance += pow(point_source[i] - point_dest[i], 2);
    }

    return sqrt(distance);
   
}


int Graph::UpdatedfindMin(unordered_set<int>& DiffResult, map<int, vector<float>>& cordinates, vector<float>& Xq, int XqIndex, float*& xq_distances,float*& neighbors_distances) {
    float minValue = numeric_limits<float>::max();
    int returnValue = -1;
    int counter = 0;
    for (const int& element : DiffResult) {
        float Eukl;
        if (xq_distances[0]!=-1 && neighbors_distances[0]==-1){
            Eukl = xq_distances[element];
        }else if (xq_distances[0]==-1 && neighbors_distances[0]!=-1){
            Eukl = neighbors_distances[counter];
        }else if(xq_distances[0]==-1 && neighbors_distances[0]==-1){
            Eukl = UpdatedeuklidianDistance(cordinates[element],Xq, XqIndex, element);
        }
        else{
        }
        
        
        
        if(Eukl < minValue){
            minValue = Eukl;
            returnValue = element;
        }
        counter++;
    }

   return returnValue;
}


int Graph::UpdatedfindFilteredMin(unordered_set<int>& DiffResult, map<int, vector<float>>& cordinates, vector<float>& Xq, int XqIndex, float*& xq_distances,float*& neighbors_distances){
    float minValue = numeric_limits<float>::max();
    int returnValue = -1;
    vector<float> cordinates_without_filter;
    // Δημιουργία του νέου vector χωρίς το πρώτο στοιχείο
    int counter = 0;
    for (const int& element : DiffResult) {
        vector<float> cordinates_without_filter(cordinates[element].begin() + 4, cordinates[element].end());

        float Eukl;
        if (xq_distances[0]!=-1 && neighbors_distances[0]==-1){
            Eukl = xq_distances[element];
        }else if (xq_distances[0]==-1 && neighbors_distances[0]!=-1){
            Eukl = neighbors_distances[counter];
        }else if(xq_distances[0]==-1 && neighbors_distances[0]==-1){
            Eukl = UpdatedeuklidianDistance(cordinates_without_filter,Xq, XqIndex, element);
        }
        else{
        }
        
        if(Eukl < minValue){
            minValue = Eukl;
            returnValue = element;
        }
    }

   return returnValue;
}






