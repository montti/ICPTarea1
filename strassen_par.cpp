#include <iostream>
#include <cstdint>
#include <vector>
#include <cmath>
#include <omp.h>
#include <bits/stdc++.h>
#include "./metrictime2.hpp"

using namespace std;

vector<float> mult(vector<float>& A, vector<float>& B, uint64_t l) {
    vector<float> res(l*l, 0);

    int i, j;

    #pragma omp parallel for collapse(2)
    for (i = 0; i < l; i++) {
        for (j = 0; j < l; j++) {
            res[(i*l)+j] = 0;
            for (int k = 0; k < l; k++) {
                res[(i*l)+j] += A[(i*l)+k] * B[(k*l)+j];
            }
        }
    }

    return res;
}


vector<float> strassenRecursive(vector<float>& A, vector<float>& B, uint64_t l){

    // Si el largo de la matriz es mayor a 2, se debe dividir y conquistar.
    // Como las matrices a evaluar son cuadradas, largo = ancho.

    if (l <= 4) {
        //vector<float> aux(1, A[0] * B[0]);
        //return aux;
        return mult(A, B, l);
    } else {

        // Las sub-matrices tendrán los tamaños fijados.

        // Si la matriz grande posee tamaño l*l
        // Los cuadrantes tendrán tamaño l/2*l/2
        uint64_t tam_sub = (l/2) * (l/2);

        // Sub-matrices de los multiplicadores
        vector<float> a1(tam_sub,0), a2(tam_sub,0), a3(tam_sub,0), a4(tam_sub,0);
        vector<float> b1(tam_sub,0), b2(tam_sub,0), b3(tam_sub,0), b4(tam_sub,0);

        // Sub-matrices del producto.
        vector<float> c1(tam_sub,0), c2(tam_sub,0), c3(tam_sub,0), c4(tam_sub,0);

        // Estos ciclos for deberían avanzar por todos los espacios
        // de las 4 sub-matrices.
        #pragma omp parallel for collapse(2)
        for(int i = 0; i < l/2; i++){
            for(int j = 0; j < l/2; j++){

                // Testeado en cuaderno con una matriz de 4x4.
                // Y en una de 8x8.
                
                // Matriz superior izquierda
                a1[i*(l/2)+j] = A[i*l+j];
                b1[i*(l/2)+j] = B[i*l+j];

                // Matriz superior derecha
                a2[i*(l/2)+j] = A[(l/2)+(i*l)+j];
                b2[i*(l/2)+j] = B[(l/2)+(i*l)+j];

                // Matriz inferior izquierda
                a3[i*(l/2)+j] = A[(l/2*l)+(i*l)+j];
                b3[i*(l/2)+j] = B[(l/2*l)+(i*l)+j];

                // Matriz inferior derecha
                a4[i*(l/2)+j] = A[(l/2*l)+(l/2)+(i*l)+j];
                b4[i*(l/2)+j] = B[(l/2*l)+(l/2)+(i*l)+j];

            }
        }

        // Sumas

        vector<float> add_a1a4(tam_sub, 0);
        vector<float> add_b1b4(tam_sub, 0);
        vector<float> add_a3a4(tam_sub, 0);
        vector<float> add_a1a2(tam_sub, 0);
        vector<float> add_b1b2(tam_sub, 0);
        vector<float> add_b3b4(tam_sub, 0);
        vector<float> sub_b2b4(tam_sub, 0);
        vector<float> sub_b3b1(tam_sub, 0);
        vector<float> sub_a3a1(tam_sub, 0);
        vector<float> sub_a2a4(tam_sub, 0);


        #pragma omp parallel for
        for (int i = 0; i < tam_sub; ++i) {
            add_a1a4[i] = a1[i] + a4[i];
            add_b1b4[i] = b1[i] + b4[i];
            add_a3a4[i] = a3[i] + a4[i];
            add_a1a2[i] = a1[i] + a2[i];
            add_b1b2[i] = b1[i] + b2[i];
            add_b3b4[i] = b3[i] + b4[i];
            sub_b2b4[i] = b2[i] - b4[i];
            sub_b3b1[i] = b3[i] - b1[i];
            sub_a3a1[i] = a3[i] - a1[i];
            sub_a2a4[i] = a2[i] - a4[i];
        }


        vector<float> m1;
        #pragma omp task shared(m1)
        {m1 = strassenRecursive(add_a1a4, add_b1b4, l/2);}

        vector<float> m2;
        #pragma omp task shared(m2)
        {m2 = strassenRecursive(add_a3a4, b1, l/2);}

        vector<float> m3;
        #pragma omp task shared(m3)
        {m3 = strassenRecursive(a1, sub_b2b4, l/2);}

        vector<float> m4;
        #pragma omp task shared(m4)
        {m4 = strassenRecursive(a4, sub_b3b1, l/2);}

        vector<float> m5;
        #pragma omp task shared(m5)
        {m5 = strassenRecursive(add_a1a2, b4, l/2);}

        vector<float> m6;
        #pragma omp task shared(m6)
        {m6 = strassenRecursive(sub_a3a1, add_b1b2, l/2);}

        vector<float> m7;
        #pragma omp task shared(m7)
        {m7 = strassenRecursive(sub_a2a4, add_b3b4, l/2);}

        #pragma omp taskwait

        #pragma omp parallel for 
        for (int i = 0; i < tam_sub; ++i) {
            c1[i] = m1[i] + m4[i] - m5[i] + m7[i];   
            c2[i] = m3[i] + m5[i];
            c3[i] = m2[i] + m4[i];
            c4[i] = m1[i] - m2[i] + m3[i] + m6[i];       
        }


        vector<float> C(l*l,0);

        #pragma omp parallel for collapse(2)
        for(int i = 0; i < l/2; i++){
            for(int j = 0; j < l/2; j++){

                // Matriz superior izquierda
                C[i*l+j] = c1[i*(l/2)+j];

                // Matriz superior derecha
                C[(l/2)+(i*l)+j] = c2[i*(l/2)+j];

                // Matriz inferior izquierda
                C[(l/2*l)+(i*l)+j] = c3[i*(l/2)+j];

                // Matriz inferior derecha
                C[(l/2*l)+(l/2)+(i*l)+j] = c4[i*(l/2)+j];

            }
        }

        return C;

    } 
}

int main (int argc, char *argv[]) {

    if(argc != 2){
        cout<<" uso: " << argv[0] <<" dimension" << endl;
        cout<<" Las dimensiones son de 2^x, x siendo el numero insertado en la terminal. " << endl;
        return 1;
    }

    int dimension = atoi(argv[1]);
    const uint64_t l = 1 << dimension;

    /*
    // DEBUG
    int dimension = 7;
    const uint64_t l = 1 << dimension;
    */

    TIMERSTART(init)
    vector<float> A (l*l, 0);
    vector<float> B (l*l, 0);
    vector<float> C (l*l, 0);
    TIMERSTOP(init)
    
    for(int i=0; i<l; i++){
       for(int j=0; j<l; j++){
            A[i*l+j] = rand()%5+1;
       }
    }

    for(int i=0; i<l; i++){
       for(int j=0; j<l; j++){
            B[i*l+j] = rand()%6+1;
       }
    }

    /**
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
    **/

    omp_set_num_threads(12);

    TIMERSTART(strassenRecursive)
    #pragma omp parallel
    {
        #pragma omp single
         C = strassenRecursive(A,B,l);
    }
    TIMERSTOP(strassenRecursive)

    /**
    cout << "C: " << endl;

    for(int i=0; i<l; i++){
       for(int j=0; j<l; j++){
        cout << C[(i*l)+j] << "\t";
       }
       cout << endl;
    }
    **/
}
