#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <regex>
#include <cstdint>
#include "../Includes/io.h"

#include "../Includes/Graph/Graph.h"
#include <random> 

using namespace std;


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

  // Also accept other path for source data
  // if (argc > 1) {
  //   source_path = string(argv[1]);
  // }

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


  /** A basic method to compute the KNN results using sampling  **/
  const int K = 100;    // To find 100-NN

  for(uint i = 0; i < nq; i++){
    uint32_t query_type = queries[i][0];
    int32_t v = queries[i][1];
    float l = queries[i][2];
    float r = queries[i][3];
    vector<float> query_vec;

    // first push_back 2 zeros for aligning with dataset
    query_vec.push_back(0);
    query_vec.push_back(0);
    for(uint j = 4; j < queries[i].size(); j++)
      query_vec.push_back(queries[i][j]);

    vector<uint32_t> knn; // candidate knn

    // Handling 4 types of queries
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

    // If the number of knn in the sampled data is less than K, then fill the rest with the last few nodes
    if(knn.size() < K){
      uint32_t s = 1;
      while(knn.size() < K) {
        knn.push_back(n - s);
        s = s + 1;
      }
    }

    // build another vec to store the distance between knn[i] and query_vec
    vector<float> dists;
    dists.resize(knn.size());
    for(uint32_t j = 0; j < knn.size(); j++)
      dists[j] = compare_with_id(nodes[knn[j]], query_vec);

    vector<uint32_t > ids;
    ids.resize(knn.size());
    iota(ids.begin(), ids.end(), 0);
    // sort ids based on dists
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

  // save the results
  SaveKNN(knn_results, knn_save_path);



  srand(static_cast<unsigned>(time(0)));
  vector<int> randomVec(nodes.size()); //Vector που θα περιέχει τις τιμές απο 0 εως n-1
  
  
  //save all values of filters
  vector<int> filters;
  set<float> filters_set;

  //Διαμορφώνουμε τα nodes για να έχουν τον ίδιο τύπο με τα query.
  for(int i=0; i < nodes.size(); i++){
    filters.push_back(nodes[i][0]); //Το καθε φιλτρο καθε σημειου.
    filters_set.insert(nodes[i][0]); //Set με ολα τα φιλτρα.
    
    nodes[i].erase(nodes[i].begin()+ 1);
    nodes[i].insert(nodes[i].begin(), 1);//Ολα έχουν φίλτρο οπότε βάζουμε το 1 στην πρώτη θέση
    nodes[i].insert(nodes[i].begin() + 2, -1);// Timestamps
    nodes[i].insert(nodes[i].begin() + 3, -1);// Timestamps
  }

 
  
  //Aποθηκεύουμε στο MedoidMap τα Medoids των δεδομένων βασισμένα στα φίλτρα
  map<float, float> medoidsMap = findFiltersMedoid(nodes, 100, filters_set);

  
  // Κινητήρας τυχαίων αριθμών
  random_device rd;                    
  default_random_engine eng(rd()); 
    
  //Ανακατεύουμε τις τιμές του Vector
  shuffle(randomVec.begin(), randomVec.end(), eng);


  int R; //Το R είναι απο 1 μεχρι 13
  int l;
  float a;


  Graph graph(nodes.size());
  //Αρχικοποίηση άδειου γράφου
  for (size_t i = 0; i < nodes.size(); ++i){
    randomVec[i] = i;
    graph.addNode(i,nodes[i]);
    
  }

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
  graph.SaveGraphBinary("OutGraphs/FilteredGraph.bin");
  return 0;
}

