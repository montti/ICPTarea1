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

    /*
    const uint64_t m = 1 << 10;
    const uint64_t n = 1 << 10;
    const uint64_t l = 1 << 10;
    */

    TIMERSTART(init)
    vector<float> A (l*l, 0);
    vector<float> B (l*l, 0);
    vector<float> C (l*l, 0);
    TIMERSTOP(init)


    for(int i=0; i<l; i++){
       for(int j=0; j<l; j++){
            A[i*l+j] = rand()%5;
       }
    }

    /*
    for(int i=0; i<m; i++){
       for(int j=0; j<l; j++){
            cout << A[i*l+j] << " ";
       }
       cout << endl;
    }
    */


    for(int i=0; i<l; i++){
       for(int j=0; j<l; j++){
            B[i*l+j] = rand()%6;
       }
    }


    TIMERSTART(seqmult)
    for (uint64_t i = 0; i < l; i++)
        for (uint64_t j = 0; j < l; j++) {
            float accum = 0;
	    for (uint64_t k = 0; k < l; k++)
            accum += A[i*l+k]*B[k*l+j];
	    C[i*l+j] = accum;
	}

    TIMERSTOP(seqmult)
}
