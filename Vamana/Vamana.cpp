#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include "../Includes/Graph/Graph.h"
#include <cstdlib>  
#include <ctime>
#include <algorithm>
#include <random> 



using namespace std;



int main(int argc,char* argv[]){
    //Ελέγχος για το αν δόθηκαν σωστά τα ορίσματα
    if(argc > 7){
        cout << "Please give ./Vamana base.fvecs query.fvecs groundtrouth.ivecs R l a\n";
        return 1;
    }

    //Άνοιγμα αρχείου base.fvecs
    string base_fvecs = argv[1];
    vector<vector<float>> base_val = open_fvec(base_fvecs);

    //Άνοιγμα αρχείου query.fvecs
    string query_fvecs = argv[2];
    vector<vector<float>> query_val = open_fvec(query_fvecs);

    //Άνοιγμα αρχείου groundtrouth.ivecs
    string grounth_ivecs = argv[3];
    vector<vector<int>> grounth_val = open_ivec(grounth_ivecs);
    int R; //Το R είναι απο 1 μεχρι 13
    int l;
    float a;

    if(argc > 4){
        R = atoi(argv[4]);
        l = atoi(argv[5]);
        a = atoi(argv[6]);
    }else{
        cout<<"Give an R: "; 
        if (scanf("%d", &R) != 1) {
            fprintf(stderr, "Error reading R.\n");
            return EXIT_FAILURE;
        }
        cout<<"Give an L: ";
        if (scanf("%d", &l) != 1) {
            fprintf(stderr, "Error reading R.\n");
            return EXIT_FAILURE;
        }
        cout<<"Give an a: ";
        if (scanf("%f", &a) != 1) {
            fprintf(stderr, "Error reading a.\n");
            return EXIT_FAILURE;
        }
        cout<<endl;
    }

    srand(static_cast<unsigned>(time(0)));
    
    vector<int> randomVec(base_val.size()-1); //Vector που θα περιέχει τις τιμές απο 0 εως n-1
    //Κατασκευή τυχαίου γράφου που κάθε κόμβος του έχει  R ακμές
    Graph graph(base_val.size() - 1);
    for (size_t i = 0; i < base_val.size() - 1; ++i) {
        randomVec[i] = i;
        graph.addNode(i, base_val[i]);
        // Γεννήτρια για τυχαίο αριθμό ακμών από 1 έως R
        int numEdges = 1 + rand() % R; // Τυχαίος αριθμός στο διάστημα [1, R]
    
        for (int j = 0; j < numEdges; ++j) {
            int limited_random_number = rand() % (base_val.size() - 1);
        
            // Βεβαιώσου ότι ο κόμβος δεν έχει ήδη προστεθεί ως γείτονας
            while (graph.checkIfNeighboors(i, limited_random_number) == true || limited_random_number == i) {
                limited_random_number = rand() % (base_val.size() - 1);
            }
            graph.addEdge(i, limited_random_number);
        }
    }

    random_device rd;                    
    default_random_engine eng(rd());       // Κινητήρας τυχαίων αριθμών
    
    //Ανακατεύουμε τις τιμές του Vector
    shuffle(randomVec.begin(), randomVec.end(), eng);


    int medoid = findMedoid(base_val);
    // int medoid = 8736;

    
    //Vamana αλγόριθμος 
    cout<<"Starting Vamana..."<<endl;
    for(int i=0; i < base_val.size()-1; i++){
        if(i%1000==0)
            cout<<i<<endl;
        auto result = graph.GreedySearch(medoid,graph.getCordinates_of_node_i(randomVec[i]),1,l);
        
        auto& Visited = result.second;
        graph.RobustPrune(randomVec[i],Visited,a,R);
        
        for(int nodes: graph.getList_of_node_i(randomVec[i])){
            vector<int> outgoing = graph.getList_of_node_i(nodes);
            outgoing.emplace_back(randomVec[i]);
            if(outgoing.size() > R){
                unordered_set<int> outgoing_set(outgoing.begin(), outgoing.end()); 
                graph.RobustPrune(nodes,outgoing_set,a,R); 
            }
            else{
                graph.addEdge(nodes,randomVec[i]);
            }
        }
        
    }
    cout<<"Vamana Finished!"<<endl;
    graph.SaveGraphBinary("VamanaGraph.bin");
    string MedoidOut = "medoid.txt";
    ofstream outFile(MedoidOut);
    outFile << medoid;
    outFile.close();

    return 0;
}