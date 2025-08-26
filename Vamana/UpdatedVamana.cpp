#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include "../Includes/Graph/Graph.h"
#include <cstdlib>  
#include <ctime>
#include <algorithm>
#include <random> 
#include <pthread.h>
#include <limits>
#include <cstring>



using namespace std;
//Δεδομένα των threads
struct ThreadData {
    Graph* graph;
    vector<int>* randomVec;
    float* xq_distances;
    int start; // Αρχικό όριο υπολογισμού
    int end;   // Τελικό όριο υπολογισμού
    int currentIndex; // To i από το randomVec[i]
};
//Συνάρτηση των threads που υπολογίζει και αποθηκεύει τις αποστάσεις ορισμένων σημείων
void* computeDistances(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    for (int j = data->start; j < data->end; ++j) {
        data->xq_distances[j] = euklidianDistance(
            data->graph->getCordinates_of_node_i(j),
            data->graph->getCordinates_of_node_i((*data->randomVec)[data->currentIndex])
        );
    }

    pthread_exit(nullptr);
}
//Δεδομένα των threads 
struct threadData2 {
    Graph* graph;
    int node;
    float* neighbor_distances;
    int start; // Αρχικό όριο υπολογισμού
    int end;   // Τελικό όριο υπολογισμού
    vector<int>* outgoing;
};
//Συνάρτηση των threads που υπολογίζει και αποθηκεύει τις αποστάσεις ορισμένων σημείων 
void* computeDistances2(void* arg) {
    threadData2* data = (threadData2*)arg;

    for (int j = data->start; j < data->end; ++j) {
        data->neighbor_distances[j] = euklidianDistance(
            data->graph->getCordinates_of_node_i((*data->outgoing)[j]),
            data->graph->getCordinates_of_node_i(data->node)
        );
    }

    pthread_exit(nullptr);
}


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
    
    //Αρχικοποίηση medoid στον Vamana με τυχαία σημεία
    vector<vector<float>> randomMedoids;
    for(int i=0; i< base_val.size() / 10; i++){
        uniform_int_distribution<> dis(0, base_val.size() - 1); // Τυχαίοι αριθμοί στο διάστημα [0, data.size() - 1]
        randomMedoids.push_back(base_val[dis(eng)]);
    }


    // Αποθήκευση αποστάσεων
    vector<vector<float>> distances(randomMedoids.size(), vector<float>(randomMedoids.size(), 0.0));

    int medoid = findMedoid(randomMedoids);
    // int medoid = 8376;
                

                
    //Vamana αλγόριθμος 
    cout<<"Starting UpdatedVamana..."<<endl;
    for(int i=0; i < base_val.size()-1; i++){
        if(i%1000==0)
            cout<<i<<endl;
        // Αρχικοποίηση xq_distances
        float* xq_distances = new float[randomVec.size()-1];

        // Ορισμός παραμέτρων για pthreads
        int numThreads = 8; // Πλήθος threads
        pthread_t threads[numThreads];
        ThreadData threadData[numThreads];

        int segmentSize = randomVec.size() / numThreads;
        int remaining = randomVec.size() % numThreads;

        for (int t = 0; t < numThreads; ++t) {
            threadData[t].graph = &graph;
            threadData[t].randomVec = &randomVec; // Πλήρης πίνακας
            threadData[t].xq_distances = xq_distances;
            threadData[t].start = t * segmentSize;
            threadData[t].end = (t + 1) * segmentSize;

            if (t == numThreads - 1) {
                threadData[t].end += remaining;
            }

            threadData[t].currentIndex = i;
            //καλούμε τα threads για να υπολογσίσουμε τις αποστάσεις του xq από τα υπολοίπα σημεία 
            pthread_create(&threads[t], nullptr, computeDistances, (void*)&threadData[t]);
        }
        //αναμονή των threads
        for (int t = 0; t < numThreads; ++t) {
            pthread_join(threads[t], nullptr);
        }
        
        auto result = graph.UpdatedGreedySearch(medoid,randomVec[i],1,l, xq_distances,randomVec);
        
        auto& Visited = result.second;
        float* neighbors_distances_2 = new float[1];
        neighbors_distances_2[0] = -1;
        graph.UpdatedRobustPrune(randomVec[i],Visited,a,R, xq_distances,randomVec,neighbors_distances_2);
        delete[] neighbors_distances_2;
        
        xq_distances[0] = -1;//Για να μην λάβει υπόψιν ο παρακάτω robust prune αυτόν τιν πίνακα

        for(int nodes: graph.getList_of_node_i(randomVec[i])){
            vector<int> outgoing = graph.getList_of_node_i(nodes);
            outgoing.emplace_back(randomVec[i]);
            float* neighbors_distances = new float[outgoing.size()];
            if(outgoing.size() > R){
                // Ορισμός παραμέτρων για pthreads
                int numThreads2 = 8; // Πλήθος threads
                pthread_t threads2[numThreads];
                threadData2 ThreadData2[numThreads];

                int segmentSize = outgoing.size()/ numThreads2;
                int remaining = outgoing.size() % numThreads2;

                for (int t = 0; t < numThreads2; ++t) {
                    ThreadData2[t].graph = &graph;
                    ThreadData2[t].node = nodes; // Πλήρης πίνακας
                    ThreadData2[t].neighbor_distances = neighbors_distances;
                    ThreadData2[t].start = t * segmentSize;
                    ThreadData2[t].end = (t + 1) * segmentSize;

                    if (t == numThreads - 1) {
                        ThreadData2[t].end += remaining;
                    }

                    ThreadData2[t].outgoing = &outgoing;
                    //καλούμε τα threads για να υπολογσίσουμε τις αποστάσεις του node από τα υπολοίπα σημεία 
                    pthread_create(&threads[t], nullptr, computeDistances2, (void*)&ThreadData2[t]);
                }
                //Αναμονή των threads
                for (int t = 0; t < numThreads; ++t) {
                    pthread_join(threads[t], nullptr);
                }
                unordered_set<int> outgoing_set(outgoing.begin(), outgoing.end()); 
                graph.UpdatedRobustPrune(nodes,outgoing_set,a,R, xq_distances,randomVec,neighbors_distances); 
            }
            else{
                graph.addEdge(nodes,randomVec[i]);
            }
            delete[] neighbors_distances;
        }
        
        delete[] xq_distances;
    }
    
    cout<<"Updated Vamana Finished"<<endl;
    graph.SaveGraphBinary("UpdatedVamanaGraph.bin");
    string MedoidOut = "medoid.txt";
    ofstream outFile(MedoidOut);
    outFile << medoid;
    outFile.close();


    
    
    return 0;


}


