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


float compare_with_id(const vector<float>& a, const vector<float>& b) {
  float sum = 0.0;
  // Skip the first 2 dimensions
  for (size_t i = 2; i < a.size(); ++i) {
    float diff = a[i] - b[i];
    sum += diff * diff;
  }
  return sum;
}


int main(int argc, char* argv[]){
    string source_path = "../Data/dummy-data.bin";
    string query_path = "../Data/dummy-queries.bin";
    string knn_save_path = "../Data/output.bin";


    uint32_t num_data_dimensions = 102;
    float sample_proportion = 0.001;

    // Read data points
    vector <vector<float>> nodes;
    ReadBin(source_path, num_data_dimensions, nodes);
    cout<<nodes.size()<<"\n";


    // Read queries
    uint32_t num_query_dimensions = num_data_dimensions + 2;
    vector <vector<float>> queries;
    ReadBin(query_path, num_query_dimensions, queries);

    vector <vector<uint32_t>> knn_results; // for saving knn results

    uint32_t n = nodes.size();
    uint32_t d = nodes[0].size();
    uint32_t nq = queries.size();
    uint32_t sn = uint32_t(sample_proportion * n);

    cout<<"# data points:  " <<n<<"\n";
    cout<<"# data point dim:  " << d<<"\n";
    cout<<"# queries:      " << nq<<"\n";

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

    int option = atoi(argv[1]);
    Graph graph(10);
    if(option != 2){
        string filename = argv[2];
        graph = graph.LoadGraphBinary(filename);
    }
    

    
   
    if(option == 1){//FilteredVamanaCheck
        

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
        int k = atoi(argv[3]);

        //Ελέγχουμε τον γράφου που έφτιαξε ο Vamana με το αρχείο grount.txt


        std::ifstream file("ground.txt"); // Άνοιγμα του αρχείου
        if (!file.is_open()) {
            std::cerr << "Failed to open file." << std::endl;
            return 1;
        }

        std::vector<std::vector<int>> groundtrouth; // Vector για αποθήκευση των queries
        std::string line;

        while (std::getline(file, line)) {
            // Ελέγχουμε αν η γραμμή περιέχει "Query"
            if (line.find("Query") != std::string::npos) {
                std::vector<int> query_results;
                std::getline(file, line); // Διαβάζουμε την επόμενη γραμμή που περιέχει τα IDs

                std::stringstream ss(line);
                int id;
                char comma;

                // Διαβάζουμε τα IDs από τη γραμμή
                while (ss >> id) {
                    query_results.push_back(id);
                    ss >> comma; // Παρακάμπτουμε τα κόμματα
                }

                // Προσθέτουμε τα αποτελέσματα στον κεντρικό vector
                groundtrouth.push_back(query_results);
            }
        }


        file.close(); 
        

        //Αποθηκεύουμε τα ερωτήματα με query type 0 ή 1
        vector<vector<float>> filtered_queries;

        for (const auto& q : queries) {
            // Ελέγχουμε αν ο τύπος είναι 0 ή 1
            if (!q.empty() && (q[0] == 0 || q[0] == 1)) {
            filtered_queries.push_back(q);
            }
        }

        //Υπολογίζουμε το ποσοστό επιτυχίας του αλγορόθμου vamana
        long long int   Filtered_total_sum = 0;
        int countFiltered = 0;
        long long int total_sum = 0;
        long long int Unfiltered_total_sum = 0;
        int countUnfiltered = 0;
        for(int i=0; i<filtered_queries.size(); i++){
            cout<<"For"<<i<<": ";
            map<float,float> unfiltered_map;
            pair<set<Lnodes, CompareValue>, unordered_set<int>> result1;
            if(filtered_queries[i][0] == 0){ //Αν το query δεν έχει φίλτρο
            for (const auto& pair : medoidsMap) {
                map<float, float> medoidsMap_star;
                medoidsMap_star[pair.first] = pair.second;
                auto result = graph.FilteredGreedySearch(medoidsMap_star,filtered_queries[i],1,10,filters);
                unordered_set<int> visited = result.second;
                auto it = visited.begin();
                unfiltered_map[pair.first] = *it;
            }
            result1 = graph.FilteredGreedySearch(unfiltered_map,filtered_queries[i],k,150,filters);
            
            }
            else{//Αν το query έχει φίλτρο
            result1 = graph.FilteredGreedySearch(medoidsMap,filtered_queries[i],k,150,filters);
            }
        
            auto& tempSet = result1.first;
            float sum = 0;
            int flag = 0;
            for (const auto& node : tempSet) {
            int counter = 0;
            for(int number:groundtrouth[i]){
                if(counter == k) break;
                if(node.index == number){
                sum++;
                break;
                }
                counter++;
            }
            
            }
            if(tempSet.size()!= 0 ){
            sum = (sum/tempSet.size())*100;
            }
            if(filtered_queries[i][0] == 0){
            Unfiltered_total_sum+=sum;
            countUnfiltered++;

            }else{
            Filtered_total_sum+=sum;
            countFiltered++;
            }

            cout<<"found "<<sum<<endl;
            total_sum = total_sum+sum;
            cout<<endl;
        }
    
        double pos = total_sum/groundtrouth.size();
        double FilteredPos = Filtered_total_sum/countFiltered;
        double UnfilteredPos = Unfiltered_total_sum/countUnfiltered;

        cout<<"ΠΟΣΟΣΤΟ Filtered Queries: "<<FilteredPos<<"%"<<endl;
        cout<<"ΠΟΣΟΣΤΟ Unfiltered Queries:"<<UnfilteredPos<<"%"<<endl;
        cout<<"ΠΟΣΟΣΤΟ όλων των Queries: "<<pos<<"%"<<endl;

        return 0;

    }else if(option == 2){//check StichedVamana
        string folderName = argv[2];
        int k = atoi(argv[3]);

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
        vector<vector<float>> nodes_cords;
        for (const auto& node : nodes) {
            if (node.size() > 4) { // Ελέγχει ότι υπάρχει 4η θέση και παραπάνω.
                vector<float> cords(node.begin() + 4, node.end()); // Αντιγραφή από την 4η θέση μέχρι το τέλος.
                nodes_cords.push_back(cords); // Προσθήκη στο νέο vector.
            }
        }

        //Αποθηκεύουμε τα ερωτήματα με query type 0 ή 1
        std::vector<std::vector<float>> filtered_queries;

        for (const auto& q : queries) {
            // Ελέγχουμε αν ο τύπος είναι 0 ή 1
            if (!q.empty() && (q[0] == 0 || q[0] == 1)) {
            filtered_queries.push_back(q);
            }
        }

        //αποθηκεύουμε σε ενα map το φιλτρο κάθε query
        map<int,int> queries_filter;
        for (size_t i = 0; i < filtered_queries.size(); ++i) {
            queries_filter[i] = filtered_queries[i][1];
        }

        vector<vector<float>> queriesWithoutFilters;
        for (const auto& node : filtered_queries) {
            if (node.size() > 4) { // Ελέγχει ότι υπάρχει 4η θέση και παραπάνω.
                vector<float> cords(node.begin() + 4, node.end()); // Αντιγραφή από την 4η θέση μέχρι το τέλος.
                queriesWithoutFilters.push_back(cords); // Προσθήκη στο νέο vector.
            }
        }

        vector<set<int>> P(filters_set.size());
        for(auto v: filters_set){
            for(int i=0; i < nodes.size(); i++){
                if(nodes[i][1] == v){
                    P[v].insert(i);
                }
            }
        }
        
        // Output the contents of P
        vector<vector<int>> p_indexes(P.size()); 
        for (size_t v = 0; v < P.size(); v++) {
            for (const int& index : P[v]) {
                p_indexes[v].push_back(index);
            }
        }

        
        int numGraphs = filters_set.size();  // Ο αριθμός των γραφικών αντικειμένων
        
        

   

        std::ifstream file("ground.txt"); // Άνοιγμα του αρχείου
        if (!file.is_open()) {
            std::cerr << "Failed to open file." << std::endl;
            return 1;
        }

        std::vector<std::vector<int>> groundtrouth; // Vector για αποθήκευση των queries
        std::string line;

        while (std::getline(file, line)) {
            // Ελέγχουμε αν η γραμμή περιέχει "Query"
            if (line.find("Query") != std::string::npos) {
                std::vector<int> query_results;
                std::getline(file, line); // Διαβάζουμε την επόμενη γραμμή που περιέχει τα IDs

                std::stringstream ss(line);
                int id;
                char comma;

                // Διαβάζουμε τα IDs από τη γραμμή
                while (ss >> id) {
                    query_results.push_back(id);
                    ss >> comma; // Παρακάμπτουμε τα κόμματα
                }

                // Προσθέτουμε τα αποτελέσματα στον κεντρικό vector
                groundtrouth.push_back(query_results);
            }
        }


        file.close(); 
        




        
        //Υπολογίζουμε το ποσοστό επιτυχίας του αλγορόθμου vamana
        int graph_more = 0;
        int total_pos = 0;

        map<int, int> medoidsMap;
        string MedoidMapfilename = "../StichedVamana/OutGraphs/medoidMap.txt";
        LoadMedoidsMapFromFile(MedoidMapfilename,  medoidsMap); // Φόρτωση του medoidsMap
        for(int graph_counter = 0;  graph_counter< numGraphs; graph_counter++){
            string filename = folderName + "/graph_" + to_string(graph_counter) + ".bin";
            graph = graph.LoadGraphBinary(filename);
           
           
            int total_sum = 0;
            int Filtered = 0;
            
            
            for(int i=0; i<filtered_queries.size(); i++){
                if(filtered_queries[i][1] == graph_counter && filtered_queries[i][0] == 1 && filtered_queries[i][1] <= 128){
                    Filtered++;
                   
                    auto result1 = graph.GreedySearch(medoidsMap[graph_counter], queriesWithoutFilters[i], k, 150);
                    auto& tempSet = result1.first;
                    float sum = 0;
                    for (const auto& node : tempSet) {
                        int counter = 0;
                        for(int number:groundtrouth[i]){
                            if(counter == k) break;
                            if(node.index == number){
                                sum++;
                                break;
                            }
                            counter++;
                        }
                    }
                    if(tempSet.size()!= 0){
                        sum = (sum/tempSet.size())*100;
                    }
                    cout<<"found "<<sum<<endl;
                    total_sum = total_sum+sum;
                }
            }
            if(Filtered == 0){
                continue;
            }

            graph_more++;
            int pos = total_sum/Filtered;
            total_pos = total_pos + pos;
        }
        total_pos = total_pos/graph_more;
        cout<<endl<<"ΠΟΣΟΣΤΟ: "<<total_pos<<"%"<<endl;
            
            






        return 0;



    }else if(option == 3){//CheckVamana

        //Άνοιγμα αρχείου query.fvecs
        string query_fvecs = "../Data/siftsmall/siftsmall/siftsmall_query.fvecs";
        vector<vector<float>> query_val = open_fvec(query_fvecs);

        //Άνοιγμα αρχείου groundtrouth.ivecs
        string grounth_ivecs = "../Data/siftsmall/siftsmall/siftsmall_groundtruth.ivecs";
        vector<vector<int>> grounth_val = open_ivec(grounth_ivecs);

        int medoid;
        string MedoidOut = "../Vamana/medoid.txt";
        ifstream inFile(MedoidOut);

        // Check if the file was opened successfully
        if (!inFile) {
            std::cerr << "Error: Could not open the file " << MedoidOut << std::endl;
            return 1; // Exit with error code
        }
        inFile >> medoid;
        inFile.close();


        int k = atoi(argv[3]);

            
        //Υπολογίζουμε το ποσοστό επιτυχίας του αλγορόθμου vamana
        int total_sum = 0;
        for(int i=0; i<query_val.size()-1; i++){
            cout<<"For"<<i<<": ";
            auto result1 = graph.GreedySearch(medoid,query_val[i],k,150);
            auto& tempSet = result1.first;
            float sum = 0;
            for (const auto& node : tempSet) {  //Διατρέχουμε κάθε στοιχείο του `set`
                int counter = 0;
                for(int number:grounth_val[i]){
                    if(counter == k) break;
                    if(node.index == number){
                        sum++;
                        break;
                    }
                    counter++;
                }
            }
        
            sum = (sum/tempSet.size())*100;
            cout<<"found "<<sum<<endl;
            total_sum = total_sum+sum;
        }

        int pos = total_sum/grounth_val.size();
        cout<<"ΠΟΣΟΣΤΟ: "<<pos<<"%"<<endl;




        return 0;


    }
    
    return 0;
}