 
 	
#include <bits/stdc++.h>
#include <omp.h>
using namespace std;
int main() {
    const int N = 1000000000; 
    vector<int> vecA(N, 1);
    vector<int> vecB(N, 2);
    vector<int> vecC(N, 0); 
    double starttime = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        vecC[i] = vecA[i] + vecB[i];
    }
    double endtime = omp_get_wtime();
    cout<<"done in : "<<endtime-starttime<<"s"<<endl;
    return 0;
}
