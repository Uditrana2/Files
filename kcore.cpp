
#include <bits/stdc++.h>
#include <omp.h>

using namespace std;


struct Edge {
    int src, dest;
};


vector<Edge> readGraph(const string& filename) {
    ifstream file(filename);
    vector<Edge> edges;
    int src, dest;
    while (file >> src >> dest) {
        edges.push_back({src, dest});
    }
    return edges;
}


int kCoreDecompositionSequential(const vector<Edge>& edges, int k) {
    unordered_map<int, int> degree;
    unordered_map<int, vector<int>> adjacencyList;
    
   
    for (const auto& edge : edges) {
        degree[edge.src]++;
        degree[edge.dest]++;
        adjacencyList[edge.src].push_back(edge.dest);
        adjacencyList[edge.dest].push_back(edge.src);
    }
    

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    for (const auto& it : degree) {
        pq.push({it.second, it.first});
    }
    
    
    while (!pq.empty()) {
        int node = pq.top().second;
        int minDegree = pq.top().first;
        pq.pop();
        
        if (minDegree < k) {
            for (int neighbor : adjacencyList[node]) {
                pq.push({degree[neighbor], neighbor});
            }
            degree.erase(node);
            for (int neighbor : adjacencyList[node]) {
                degree[neighbor]--;
            }
            adjacencyList.erase(node);
        } else {
            break;
        }
    }
    
    return degree.size(); 
}


int kCoreDecompositionParallel(const vector<Edge>& edges, int k) {
    unordered_map<int, int> degree;
    unordered_map<int, vector<int>> adjacencyList;
    
    
    for (const auto& edge : edges) {
        degree[edge.src]++;
        degree[edge.dest]++;
        adjacencyList[edge.src].push_back(edge.dest);
        adjacencyList[edge.dest].push_back(edge.src);
    }
    
  
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    for (const auto& it : degree) {
        pq.push({it.second, it.first});
    }
    
    
    while (!pq.empty()) {
        int node = pq.top().second;
        int minDegree = pq.top().first;
        pq.pop();
        
        if (minDegree < k) {
            #pragma omp parallel for
            for (int i = 0; i < adjacencyList[node].size(); ++i) {
                int neighbor = adjacencyList[node][i];
                #pragma omp critical
                {
                    pq.push({degree[neighbor], neighbor});
                }
            }
            #pragma omp critical
            {
                degree.erase(node);
            }
            for (int neighbor : adjacencyList[node]) {
                #pragma omp atomic
                degree[neighbor]--;
            }
            #pragma omp critical
            {
                adjacencyList.erase(node);
            }
        } else {
            break;
        }
    }
    
    return degree.size(); 
}

int main() {
    string filepath = "C:\\Users\\uditr\\Downloads\\New folder (2)\\twitter.txt";
    int k;
    
    
    cout << "Enter k value: ";
    cin >> k;
    
    auto startSeq = chrono::high_resolution_clock::now();
    
    
    vector<Edge> edges = readGraph(filepath);
    
    
    int beforeNodes = edges.size();
    int afterNodesSeq = kCoreDecompositionSequential(edges, k);
    
    auto endSeq = chrono::high_resolution_clock::now();
    auto durationSeq = chrono::duration_cast<chrono::milliseconds>(endSeq - startSeq);
    
    auto startPar = chrono::high_resolution_clock::now();
    
    
    int afterNodesPar = kCoreDecompositionParallel(edges, k);
    
    auto endPar = chrono::high_resolution_clock::now();
    auto durationPar = chrono::duration_cast<chrono::milliseconds>(endPar - startPar);
    
    
    cout << "Total number of nodes " << beforeNodes << endl;
    cout << "Number of nodes after k-core decomposition " << afterNodesPar << endl;
    cout << "Sequential Execution time: " << durationSeq.count() << " milliseconds" << endl;
    cout << "Parallel Execution time: " << durationPar.count() << " milliseconds" << endl;
    
    return 0;
}
