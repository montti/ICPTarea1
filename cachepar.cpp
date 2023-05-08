#include <iostream>
#include <cstdint>
#include <vector>
#include <omp.h>
#include "./metrictime2.hpp"

using namespace std;

int main (int argc, char *argv[]) {

    if(argc != 4){
	    cout<<" uso: "<<argv[0]<<" em en el\n";
	    return 1;
    } 
    int em = atoi(argv[1]); 
    int en = atoi(argv[2]); 
    int el = atoi(argv[3]); 
    cout<<" em "<<em<<" en "<<en<<" el "<<el<<endl;
    const uint64_t m = 1 << em;
    const uint64_t n = 1 << en;
    const uint64_t l = 1 << el;

    /*
    const uint64_t m = 1 << 10;
    const uint64_t n = 1 << 10;
    const uint64_t l = 1 << 10;
    */

    TIMERSTART(init)
    vector<float> A (m*l, 0); // m x l
    vector<float> B (l*n, 0); // l x n
    vector<float> C (m*n, 0); // m x n
    TIMERSTOP(init)


    for(int i=0; i<m; i++){
       for(int j=0; j<l; j++){
            A[i*l+j] = rand()%5;
       }
    }

    for(int i=0; i<l; i++){
       for(int j=0; j<n; j++){
            B[i*l+j] = rand()%6;
       }
    }

    /*
    cout << "A: " << endl;
    for(int i=0; i<l; i++){
       for(int j=0; j<l; j++){
        cout << A[i*l+j] << " ";
       }
       cout << endl;
    }

    cout << "B: " << endl;

    for(int i=0; i<l; i++){
       for(int j=0; j<l; j++){
        cout << B[i*l+j] << " ";
       }
       cout << endl;
    }
    */

    // Se usará Loop Interchange.

    uint64_t i,j,k;
    float accum = 0;

    TIMERSTART(cachefriendly)
    #pragma omp parallel for collapse(3)
    for (i = 0; i < m; i++){
        for (k = 0; k < l; k++) {
            for (j = 0; j < n; j++) {
                C[i * n+j] += A[i * l+k] * B[k * n+j];
            }
        }
    }

    TIMERSTOP(cachefriendly)

    /*
    for(int i=0; i<l; i++){
       for(int j=0; j<l; j++){
        cout << C[i*l+j] << " ";
       }
       cout << endl;
    }
    */
}
