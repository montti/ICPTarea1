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

vector<float> strassen(vector<float> A, vector<float> B, uint64_t l){

    vector<float> resultado(l*l,0);
    
    // Escrito al pie de la letra como está en el PDF.
    float m1 = (A[0] + A[3]) * (B[0] + B[3]);
    float m2 = (A[2] + A[3]) * B[0];
    float m3 = A[0] * (B[1] - B[3]);
    float m4 = A[3] * (B[2] - B[0]);
    float m5 = (A[0] + A[1]) * B[3];
    float m6 = (A[2] - A[0]) * (B[0] + B[1]);
    float m7 = (A[1] - A[3]) * (B[2] + B[3]);

    resultado[0] = m1 + m4 - m5 + m7;
    resultado[1] = m3 + m5;
    resultado[2] = m2 + m4;
    resultado[3] = m1 - m2 + m3 + m6;

    return resultado;
}

vector<float> divideConquer(vector<float>& A, vector<float>& B, uint64_t l, int opcion){

    // Si el largo de la matriz es mayor a 2, se debe dividir y conquistar.
    // Como las matrices a evaluar son cuadradas, largo = ancho.

    if(l > 2){

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

        // Ahora que se han creado las submatrices, hay que aplicarle dividir y conquistar a cada una.

        // En realidad, se "replica" la multiplicación, tomando las matrices de nxn como si fueran
        // los elementos a multiplicar.
        
        vector<float> c1_1(tam_sub,0), c1_2(tam_sub,0), c2_1(tam_sub,0), c2_2(tam_sub,0);
        vector<float> c3_1(tam_sub,0), c3_2(tam_sub,0), c4_1(tam_sub,0), c4_2(tam_sub,0);

        c1_1 = divideConquer(a1,b1,l/2,opcion);
        c1_2 = divideConquer(a2,b3,l/2,opcion);
        c2_1 = divideConquer(a1,b2,l/2,opcion);
        c2_2 = divideConquer(a2,b4,l/2,opcion);
        c3_1 = divideConquer(a3,b1,l/2,opcion);
        c3_2 = divideConquer(a4,b3,l/2,opcion);
        c4_1 = divideConquer(a3,b2,l/2,opcion);
        c4_2 = divideConquer(a4,b4,l/2,opcion);

        for(int i = 0; i < l/2; i++){
            for(int j = 0; j < l/2; j++){
                c1[i*(l/2)+j] = c1_1[i*(l/2)+j] + c1_2[i*(l/2)+j];
                c2[i*(l/2)+j] = c2_1[i*(l/2)+j] + c2_2[i*(l/2)+j];
                c3[i*(l/2)+j] = c3_1[i*(l/2)+j] + c3_2[i*(l/2)+j];
                c4[i*(l/2)+j] = c4_1[i*(l/2)+j] + c4_2[i*(l/2)+j];
            }
        }

        // Si las submatrices son de 2x2 acá ya deberían haberse procesado.
        // Por lo que se agregan a la matriz de resultados C.

        vector<float> C(l*l,0);

        for(uint64_t i = 0; i < l/2; i++){
            for(uint64_t j = 0; j < l/2; j++){

                // Parte superior izquierda
                C[i*l+j] = c1[i*(l/2)+j];

                // Parte superior derecha
                C[(l/2)+(i*l)+j] = c2[i*(l/2)+j];

                // Parte inferior izquierda
                C[(l/2*l)+(i*l)+j] = c3[i*(l/2)+j];

                // Parte inferior derecha 
                C[(l/2*l)+(l/2)+(i*l)+j] = c4[i*(l/2)+j];

            }
        }

        return C;

    }

    // Si A y B son matrices de 2x2, se hace la multiplicación
    else{

        // Resultado de multiplicación
        vector<float> res;

        // Opción 0 es multiplicación tradicional.
        if (opcion == 0){
            res = matrixMult(A,B,l);
        }

        // Opción 1 es Strassen.
        else if (opcion == 1){
            res = strassen(A,B,l);
        }

        // Acá se asignan los valores del resultado a la matriz C.
        return res;

        // Acá no tengo acceso a la "matriz superior"
        // (La que, por ejemplo, sería de 4x4 si mi matriz a trabajar acá es de 2x2)

        // Eso se procesa en en el if.

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
    int dimension = 3;
    const uint64_t l = 1 << dimension;
    int opcion = 0;

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

    C = divideConquer(A,B,l,opcion);

    TIMERSTOP(divideconquer)

    cout << "C: " << endl;

    for(int i=0; i<l; i++){
       for(int j=0; j<l; j++){
        cout << C[i*l+j] << " ";
       }
       cout << endl;
    }

}
