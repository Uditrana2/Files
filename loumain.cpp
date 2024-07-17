#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string.h>
#include <climits>
#include <omp.h>
#include "comm3.h"
#include "comm3.cc"
using namespace std;

int main() {
    char filePath[] = "../sample_graphs/soc-Epinions1.txt";
    graph g(filePath);
    g.parseGraph();
    
    
int total_nodes_before = g.num_nodes();
   
    

    
double start_time = omp_get_wtime();

int a = 0;
float b =0;
float c = 0;
int d = 0;
int numCommunities =  LouvainCommunityDetection(g);

    
double end_time = omp_get_wtime();


    
double elapsed_time_ms = (end_time - start_time) * 1000.0;

cout<< "No of nodes: "<< total_nodes_before <<endl;

cout << "No of communities: " << numCommunities << endl;
cout << "Time taken: " << elapsed_time_ms << " milliseconds" << endl;
return 0;  
}
