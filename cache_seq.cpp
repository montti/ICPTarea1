#include <iostream>
#include <cstdint>
#include <vector>
#include "./metrictime2.hpp"

using namespace std;

int main (int argc, char *argv[]) {

    if(argc != 2){
        cout<<" uso: " << argv[0] <<" dimension" << endl;
        cout<<" Las dimensiones son de 2^x, x siendo el numero insertado en la terminal. " << endl;
        return 1;
    }

    int dimension = atoi(argv[1]);
    const uint64_t l = 1 << dimension;

    TIMERSTART(init)
    vector<float> A (l*l, 0); // m x l
    vector<float> B (l*l, 0); // l x n
    vector<float> C (l*l, 0); // m x n
    TIMERSTOP(init)


    for(int i=0; i<l; i++){
       for(int j=0; j<l; j++){
            A[i*l+j] = rand()%5;
       }
    }

    for(int i=0; i<l; i++){
       for(int j=0; j<l; j++){
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
    for (i = 0; i < l; i++){
        for (k = 0; k < l; k++) {
            for (j = 0; j < l; j++) {
                C[i * l+j] += A[i * l+k] * B[k * l+j];
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
