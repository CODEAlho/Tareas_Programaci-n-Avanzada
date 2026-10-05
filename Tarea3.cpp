#include <iostream>
#include <cstdlib>
#include <chrono>    // libreria para medir el tiempo en C++
#include <memory>

extern "C" {
#include <immintrin.h> 
}

using namespace std;

//  CONTADOR DE CICLOS 
typedef unsigned long long bench_t;

// Timestamp Counter de la CPU mediante rdtsc
static inline bench_t cycles(void) {
    unsigned int hi, lo;
    __asm__ __volatile__ ("rdtsc\n\t" : "=a" (lo), "=d" (hi));
    return ((bench_t) lo) | (((bench_t) hi) << 32);
}

// Algoritmo de Horner Estándar (Escalar float)
float horner(float X, float *coef, long size) {
    float ACC = 0.0f;
    for (int i = 0; i < size; i++) {
        ACC = (ACC + coef[i]) * X;
    }
    return ACC;    
}

// Algoritmo de Horner Vectorizado (AVX float de 256 bits)
float horner_intrinsic(float X, float *coef, long size) {
    float *R, P;
    int i;
    __m256 *ymm0, X256, Y;    
    
    // arreglo float a punteros de vectores __m256 
    //(8 floats por registro de 256 bits)
    ymm0 = (__m256*)coef; 
    
    // paso es X^8
    float X2 = X * X;
    float X4 = X2 * X2;
    float X8 = X4 * X4;
    
    X256 = _mm256_set1_ps(X8);
    Y = _mm256_set1_ps(0.0f);
    
    // Bucle vectorizado de Horner
    for (i = 0; i < size / 8 - 1; i++) {
        Y = _mm256_add_ps(Y, ymm0[i]);
        Y = _mm256_mul_ps(Y, X256);
    }
    
    Y = _mm256_add_ps(Y, ymm0[i]);
    
    // Lectura de los 8 acumuladores del registro
    R = (float *)(&Y);
    
    // Combinación escalar final
    float X_pow = X;
    P = 0.0f;
    for (int k = 7; k >= 0; k--) {
        P += R[k] * X_pow;
        X_pow *= X;
    }

    return P;
}

int main() {
    float X = 1.1f;
    float R1, R2;
    int size = 10000;
    int num_trials = 100000;
    
    bench_t c1, c2;

    srand(time(NULL));    

    // Asignación de memoria alineada a 32 bytes requerida por AVX (__m256)
    float *coeficientes = (float *)_mm_malloc(size * sizeof(float), 32);

    cout << "=== PRIMEROS 10 COEFICIENTES GENERADOS ===" << endl;
    for (int i = 0; i < size; i++) {
        coeficientes[i] = (float)(rand() % 1000) / 1000.0f;
        // Muestra los primeros 10 coeficientes
        if (i < 10) {
            cout << "coef[" << i << "] = " << coeficientes[i] << endl;
        }
    }
    cout << "==========================================\n" << endl;

    //  METODO ESCALAR 
    auto start_time = chrono::high_resolution_clock::now();
    c1 = cycles();

    for (int j = 0; j < num_trials; j++) {
        R1 = horner(X, coeficientes, size);
    }

    c2 = cycles();
    auto end_time = chrono::high_resolution_clock::now();

    chrono::duration<float> duration_scalar = end_time - start_time;
    bench_t cycles_scalar = c2 - c1;

    cout << "=== METODO ESCALAR (FLOAT) ===" << endl;
    cout << "Resultado P(X): " << R1 << endl;
    cout << "Tiempo transcurrido (chrono): " << duration_scalar.count() << " segundos" << endl;
    cout << "Ciclos de CPU (rdtsc): " << cycles_scalar << " ciclos\n" << endl;


    // METODO VECTORIZADO AVX 
    start_time = chrono::high_resolution_clock::now();
    c1 = cycles();

    for (int j = 0; j < num_trials; j++) {
        R2 = horner_intrinsic(X, coeficientes, size);
    }

    c2 = cycles();
    end_time = chrono::high_resolution_clock::now();

    chrono::duration<float> duration_simd = end_time - start_time;
    bench_t cycles_simd = c2 - c1;

    cout << "=== METODO VECTORIZADO AVX (FLOAT) ===" << endl;
    cout << "Resultado P(X): " << R2 << endl;
    cout << "Tiempo transcurrido (chrono): " << duration_simd.count() << " segundos" << endl;
    cout << "Ciclos de CPU (rdtsc): " << cycles_simd << " ciclos\n" << endl;

    // Resumen de Ganancia de Rendimiento
    cout << "=== RESUMEN DE RENDIMIENTO ===" << endl;
    cout << "Aceleracion en ciclos (Speedup): " << (double)cycles_scalar / cycles_simd << "x" << endl;

    _mm_free(coeficientes);
    return 0;
}
