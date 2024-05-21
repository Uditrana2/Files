#include <bits/stdc++.h>
#include <omp.h>

using namespace std;
using namespace std::chrono;

int main() {
    const int N = 10000000; 
    vector<int> vecA(N, 1);
    vector<int> vecB(N, 2);
    vector<int> vecC(N, 0);

    auto start = high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        vecC[i] = vecA[i] + vecB[i];
    }
    auto end = high_resolution_clock::now();
    duration<double> sequentialDuration = end - start;
    cout << "Sequential execution time: " << sequentialDuration.count() << " seconds" << endl;

    
    start = high_resolution_clock::now();
    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        vecC[i] = vecA[i] + vecB[i];
    }
    end = high_resolution_clock::now();
    duration<double> parallelDuration = end - start;
    cout << "Parallel execution time: " << parallelDuration.count() << " seconds" << endl;

    return 0;
}

