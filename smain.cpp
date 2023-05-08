#include <iostream>
#include <cstdint>
#include <vector>
#include <cmath>
#include "./metrictime2.hpp"

using namespace std;


// Se llegará a un punto final, donde se tendrá una matriz de 2x2
vector<float> matrixMult(vector<float> A, vector<float> B, uint64_t l){

    vector<float> resultado(l*l,0);

    //Multiplicaciones de cada celda.
    float res1 = A[0] * B[0] + A[1] * B[2];
    float res2 = A[0] * B[1] + A[1] * B[3];
    float res3 = A[2] * B[0] + A[3] * B[2];
    float res4 = A[2] * B[1] + A[3] * B[3];

    //Agregar a una (sub)matriz de resultados
    resultado[0] = res1;
    resultado[1] = res2;
    resultado[2] = res3;
    resultado[3] = res4;

    return resultado;

}

vector<float> naive(vector<float>& A, vector<float>& B, uint64_t l) {
    vector<float> prod(l*l, 0);

    int i, j;

    for (i = 0; i < l; i++) {
        for (j = 0; j < l; j++) {
            prod[(i*l)+j] = 0;
            for (int k = 0; k < l; k++) {
                prod[(i*l)+j] += A[(i*l)+k] * B[(k*l)+j];
            }
        }
    }

    return prod;
}


vector<float> addMatrix (vector<float>& A, vector<float>& B) {
    vector<float> resultado(A.size(),0);

    for (int i = 0; i < A.size(); ++i) {
         resultado[i] = A[i] + B[i];       
    }

    return resultado;
}

vector<float> subMatrix (vector<float>& A, vector<float>& B) {
    vector<float> resultado(A.size(),0);

    for (int i = 0; i < A.size(); ++i) {
         resultado[i] = A[i] - B[i];       
    }

    return resultado;
}

vector<float> strassenRecursive(vector<float>& A, vector<float>& B, uint64_t l){

    // Si el largo de la matriz es mayor a 2, se debe dividir y conquistar.
    // Como las matrices a evaluar son cuadradas, largo = ancho.

    if (l <= 2) {
        //return matrixMult(A, B, l);
        //vector<float> aux(1, A[0] * B[0]);
        //return aux;
        return naive(A, B, l);
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
        for(int i = 0; i < l/2; i++){
            for(int j = 0; j < l/2; j++){

                // Testeado en cuaderno con una matriz de 4x4.
                // Y en una de 8x8.
                // Ojalá funcione.
                
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

                // It just works

            }
        }



        for(int i=0; i<a1.size(); i++){
            cout << a1[i] << " ";
        }

        cout  << endl << "---" << endl;

                for(int i=0; i<a2.size(); i++){
            cout << a2[i] << " ";
        }

        cout  << endl << "---" << endl;

                for(int i=0; i<a3.size(); i++){
            cout << a3[i] << " ";
        }

        cout  << endl << "---" << endl;

                for(int i=0; i<a4.size(); i++){
            cout << a4[i] << " ";
        }

        cout  << endl << "---" << endl;

        vector<float> add_a1a4 = addMatrix(a1, a4);
        vector<float> add_b1b4 = addMatrix(b1, b4);
        vector<float> add_a3a4 = addMatrix(a3, a4);
        vector<float> add_a1a2 = addMatrix(a1, a2);
        vector<float> add_b1b2 = addMatrix(b1, b2);
        vector<float> add_b3b4 = addMatrix(b3, b4);
        
        vector<float> sub_b2b4 = subMatrix(b2, b4);
        vector<float> sub_b3b1 = subMatrix(b3, b1);
        vector<float> sub_a3a1 = subMatrix(a3, a1);
        vector<float> sub_a2a4 = subMatrix(a2, a4);

        vector<float> m1 = strassenRecursive(add_a1a4, add_b1b4, l/2);
        vector<float> m2 = strassenRecursive(add_a3a4, b1, l/2);
        vector<float> m3 = strassenRecursive(a1, sub_b2b4, l/2);
        vector<float> m4 = strassenRecursive(a4, sub_b3b1, l/2);
        vector<float> m5 = strassenRecursive(add_a1a2, b4, l/2);
        vector<float> m6 = strassenRecursive(sub_a3a1, add_b1b2, l/2);
        vector<float> m7 = strassenRecursive(sub_a2a4, add_b3b4, l/2);

        for (int i = 0; i < tam_sub; ++i) {
            c1[i] = m1[i] + m4[i] - m5[i] + m7[i];   
            c2[i] = m3[i] + m5[i];
            c3[i] = m2[i] + m4[i];
            c4[i] = m1[i] - m2[i] + m3[i] + m6[i];       
        }

        vector<float> C(l*l,0);

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

    /*
    if(argc != 3){
        cout<<" uso: " << argv[0] <<" dimension opcion" << endl;
        cout<<" opcion 0 (normal) o 1 (strassen)" << endl;
        return 1;
    }

    int dimension = atoi(argv[1]);
    const uint64_t l = 1 << dimension;

    int opcion = atoi(argv[2]);

    if (opcion != 0 && opcion != 1){
        cout <<" Por favor usar 0 (normal) o 1 (strassen)" << endl;
        return 1;
    }
    */

    // DEBUG
    int dimension = 4;
    const uint64_t l = 1 << dimension;
    int opcion = 1;

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

    TIMERSTART(divideconquer)
    C = strassenRecursive(A,B,l);

    TIMERSTOP(divideconquer)

    cout << "C: " << endl;

    for(int i=0; i<l; i++){
       for(int j=0; j<l; j++){
        cout << C[(i*l)+j] << "\t";
       }
       cout << endl;
    }

}
