#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <regex>
#include <cstdint>
#include "../Includes/io.h"
#include "pthread.h"

#include "../Includes/Graph/Graph.h"
#include <random>

using namespace std;
//Δεδομένα των threads
struct ThreadData {
    int thread_id;
    int start_filter_idx;
    int end_filter_idx;
    vector<int>* randomVec;
    vector<int>* filters;
    Graph* graph;  // Assuming Graph is your graph class
    map<float,float>* medoidsMap;
    int l;
    double a;
    int R;
};

//Συνάρτηση των threads για εκτέλεση του FilteredGreadySearch
void* runFilteredGreedySearch(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    for (int i = 0; i < data->randomVec->size(); i++) {
        vector<float> cordinates_randVec =  data->graph->getCordinates_of_node_i((*data->randomVec)[i]);
        if(cordinates_randVec[1] ==  data->start_filter_idx) {
          if(i%1000==0)cout<<i<<endl;
            std::vector<int> filterVector(data->filters->begin() + data->start_filter_idx, 
                               data->filters->begin() + data->end_filter_idx);
            auto result = data->graph->FilteredGreedySearch(*data->medoidsMap,
            data->graph->getCordinates_of_node_i((*data->randomVec)[i]), 0, data->l, filterVector);

            auto& Vf = result.second;
            data->graph->FilteredRobustPrune((*data->randomVec)[i], Vf, data->a, data->R);

            // Handle outgoing edges
            for (int node : data->graph->getList_of_node_i((*data->randomVec)[i])) {
                vector<int> outgoing = data->graph->getList_of_node_i(node);
                outgoing.push_back((*data->randomVec)[i]);
                if (outgoing.size() > data->R) {
                    unordered_set<int> outgoing_set(outgoing.begin(), outgoing.end());
                    data->graph->FilteredRobustPrune(node, outgoing_set, data->a, data->R);
                } else {
                    data->graph->addEdge(node, (*data->randomVec)[i]);
                }
            }
        }
    }
    pthread_exit(nullptr);
}

float compare_with_id(const vector<float>& a, const vector<float>& b) {
    float sum = 0.0;
    // Skip the first 2 dimensions
    for (size_t i = 2; i < a.size(); ++i) {
        float diff = a[i] - b[i];
        sum += diff * diff;
    }
    return sum;
}

int main(int argc, char **argv) {
    string source_path = "../Data/dummy-data.bin";
    string query_path = "../Data/dummy-queries.bin";
    string knn_save_path = "../Data/output.bin";

    uint32_t num_data_dimensions = 102;
    float sample_proportion = 0.001;

    // Read data points
    vector <vector<float>> nodes;
    ReadBin(source_path, num_data_dimensions, nodes);

    // Read queries
    uint32_t num_query_dimensions = num_data_dimensions + 2;
    vector <vector<float>> queries;
    ReadBin(query_path, num_query_dimensions, queries);

    vector <vector<uint32_t>> knn_results; // for saving knn results

    uint32_t n = nodes.size();
    uint32_t d = nodes[0].size();
    uint32_t nq = queries.size();
    uint32_t sn = uint32_t(sample_proportion * n);

    const int K = 100;    // To find 100-NN

    for(uint i = 0; i < nq; i++){
        uint32_t query_type = queries[i][0];
        int32_t v = queries[i][1];
        float l = queries[i][2];
        float r = queries[i][3];
        vector<float> query_vec;

        query_vec.push_back(0);
        query_vec.push_back(0);
        for(uint j = 4; j < queries[i].size(); j++)
            query_vec.push_back(queries[i][j]);

        vector<uint32_t> knn; // candidate knn

        if(query_type == 0){  // only ANN
            for(uint32_t j = 0; j < sn; j++){
                knn.push_back(j);
            }
        }
        else if(query_type == 1){ // equal + ANN
            for(uint32_t j = 0; j < sn; j++){
                if(nodes[j][0] == v){
                    knn.push_back(j);
                }
            }
        }
        else if(query_type == 2){ // range + ANN
            for(uint32_t j = 0; j < sn; j++){
                if(nodes[j][1] >= l && nodes[j][1] <= r)
                    knn.push_back(j);
            }
        }
        else if(query_type == 3){ // equal + range + ANN
            for(uint32_t j = 0; j < sn; j++){
                if(nodes[j][0] == v && nodes[j][1] >= l && nodes[j][1] <= r)
                    knn.push_back(j);
            }
        }

        if(knn.size() < K){
            uint32_t s = 1;
            while(knn.size() < K) {
                knn.push_back(n - s);
                s = s + 1;
            }
        }

        vector<float> dists;
        dists.resize(knn.size());
        for(uint32_t j = 0; j < knn.size(); j++)
            dists[j] = compare_with_id(nodes[knn[j]], query_vec);

        vector<uint32_t > ids;
        ids.resize(knn.size());
        iota(ids.begin(), ids.end(), 0);
        sort(ids.begin(), ids.end(), [&](uint32_t a, uint32_t b){
            return dists[a] < dists[b];
        });
        vector<uint32_t> knn_sorted;
        knn_sorted.resize(K);
        for(uint32_t j = 0; j < K; j++){
            knn_sorted[j] = knn[ids[j]];
        }
        knn_results.push_back(knn_sorted);
    }

    SaveKNN(knn_results, knn_save_path);

    srand(static_cast<unsigned>(time(0)));
    vector<int> randomVec(nodes.size());

    vector<int> filters;
    set<float> filters_set;

    for(int i=0; i < nodes.size(); i++){
        filters.push_back(nodes[i][0]);
        filters_set.insert(nodes[i][0]);
        
        nodes[i].erase(nodes[i].begin()+ 1);
        nodes[i].insert(nodes[i].begin(), 1);
        nodes[i].insert(nodes[i].begin() + 2, -1);
        nodes[i].insert(nodes[i].begin() + 3, -1);
    }
    
    map<float, float> medoidsMap = findFiltersMedoid(nodes, 100, filters_set);

    random_device rd;                     
    default_random_engine eng(rd());

    int R; 
    int l;
    float a;

    if(argc >= 2){
        R = atoi(argv[1]);
        l = atoi(argv[2]);
        a = atoi(argv[3]);
    }else{
        cout<<"Give an R: "; 
        if (scanf("%d", &R) != 1) {
            fprintf(stderr, "Error reading R.\n");
            return EXIT_FAILURE;
        }
        cout<<"Give an L: ";
        if (scanf("%d", &l) != 1) {
            fprintf(stderr, "Error reading l.\n");
            return EXIT_FAILURE;
        }
        cout<<"Give an a: ";
        if (scanf("%f", &a) != 1) {
            fprintf(stderr, "Error reading a.\n");
            return EXIT_FAILURE;
        }
    }
    int optionForBetterTime;
    cout<<"Πατήστε 1 εάν θέλετε βελτιστοποίηση μέσω αρχικοποίησης γράφου με ακμές, πατήστε 0 εάν θέλετε βελτιστοποίση με threads(ένα thread για κάθε φίλτρο) "; 
    if (scanf("%d", &optionForBetterTime) != 1) {
        fprintf(stderr, "Error reading R.\n");
        return EXIT_FAILURE;
    }
    if(optionForBetterTime==0){//Βελτιστοποίηση μέσω threads
        Graph graph(nodes.size());
        for (size_t i = 0; i < nodes.size(); ++i){
            randomVec[i] = i;
            graph.addNode(i,nodes[i]);
        }

        shuffle(randomVec.begin(), randomVec.end(), eng);

        cout<<"Creating FilteredVamana..."<<endl;

        int num_threads = filters_set.size(); 
        pthread_t threads[num_threads];
        ThreadData thread_data[num_threads];
        int num_filters = filters.size();
        int filters_per_thread = num_filters / num_threads;

        for (int t = 0; t < num_threads; t++) {
            thread_data[t].thread_id = t;
            thread_data[t].randomVec = &randomVec;
            thread_data[t].filters = &filters;
            thread_data[t].graph = &graph;
            thread_data[t].medoidsMap = &medoidsMap;
            thread_data[t].l = l;
            thread_data[t].a = a;
            thread_data[t].R = R;

            thread_data[t].start_filter_idx = t;
            thread_data[t].end_filter_idx = (t == num_threads - 1) ? num_filters : (t + 1) * filters_per_thread;
            //Κάθε thread αναλαμβάμει να εκτελέσει των FilteredGreedySearch για ένα φίλτρο
            pthread_create(&threads[t], nullptr, runFilteredGreedySearch, &thread_data[t]);
        }
        //Αναμονή τερματισμού των threads
        for (int t = 0; t < num_threads; t++) {
            pthread_join(threads[t], nullptr);
        }
        //Αποθηκεύουμε τον γράφο
        graph.SaveGraphBinary("OutGraphs/UpdatedFilteredGraph0.bin");
    }
    else if(optionForBetterTime == 1){//Βελτιστοποίση μέσω αρχικοποίησης του γράφου με ακμές
        Graph graph(nodes.size());
        //Αρχικοποίηση άδειου γράφου
        for (size_t i = 0; i < nodes.size(); ++i){
            randomVec[i] = i;
            graph.addNode(i,nodes[i]);
            //Αρχικοποίηση γράφων με τυχαίες ακμές
            //Γεννήτρια για τυχαίο αριθμό ακμών από 1 έως R
            int numEdges = 1 + rand() % R-1; // Τυχαίος αριθμός στο διάστημα [1, R]
            
            for (int j = 0; j < numEdges; ++j) {
              int limited_random_number = rand() % (nodes.size() - 1);
                
              // Βεβαιώσου ότι ο κόμβος δεν έχει ήδη προστεθεί ως γείτονας
              while (graph.checkIfNeighboors(i, limited_random_number) == true || limited_random_number == i) {
                limited_random_number = rand() % (nodes.size() - 1);
              }
              graph.addEdge(i, limited_random_number);
            }
        
        }
        shuffle(randomVec.begin(), randomVec.end(), eng);
        //FilteredVamana αλγόριθμος 
        cout<<"Creating FilteredVamana..."<<endl;
        for(int i=0; i < nodes.size(); i++){
            if(i%1000==0)
            cout<<i<<endl;
            //cout<<"Running Vamana for node: "<<i<<endl;
            auto result = graph.FilteredGreedySearch(medoidsMap,graph.getCordinates_of_node_i(randomVec[i]),0,l,filters);
            auto& Vf = result.second;
            graph.FilteredRobustPrune(randomVec[i],Vf,a,R);
            for(int nodes: graph.getList_of_node_i(randomVec[i])){
            vector<int> outgoing = graph.getList_of_node_i(nodes);
            outgoing.emplace_back(randomVec[i]);
            if(outgoing.size() > R){
                unordered_set<int> outgoing_set(outgoing.begin(), outgoing.end()); 
                graph.FilteredRobustPrune(nodes,outgoing_set,a,R); 
            }
            else {
                graph.addEdge(nodes,randomVec[i]);
            }
            }
        }
        //Αποθηκεύουμε τον γράφο
        graph.SaveGraphBinary("OutGraphs/UpdatedFilteredGraph1.bin");
    }
        
    return 0;
}
