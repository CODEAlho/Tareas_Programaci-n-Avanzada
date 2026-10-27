#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

// --- PARÁMETROS DEL ALGORITMO ---
#define TAM_GRUPO 50
#define TOTAL_ITERACIONES 100
#define TASA_INTERCAMBIO 0.90
#define TASA_ALTERACION 0.01
#define PI 3.14159265358979323846

// --- ESTRUCTURA DEL CANDIDATO ---
typedef struct {
    uint64_t adn; 
    double puntaje;      
} Candidato;

// --- PROTOTIPOS DE FUNCIONES ---

uint64_t obtener_aleatorio_64();
void crear_grupo_inicial(Candidato grupo_actual[]);
double convertir_a_real(uint32_t valor_binario);
double evaluar_funcion_objetivo(double x, double y);
void calificar_grupo(Candidato grupo_actual[], int cantidad);
void combinar(Candidato progenitor1, Candidato progenitor2, Candidato *creacion1, Candidato *creacion2);
void alterar(Candidato *sujeto);
int ordenar_por_puntaje(const void *a, const void *b);
void elegir_mejores(Candidato grupo_base[], Candidato descendencia[]);
void mostrar_bits(uint64_t adn);

// --- FUNCIÓN PRINCIPAL ---
int main() {
    srand((unsigned int)time(NULL));

    Candidato grupo_actual[TAM_GRUPO];
    Candidato descendencia[TAM_GRUPO];

    crear_grupo_inicial(grupo_actual);
    calificar_grupo(grupo_actual, TAM_GRUPO);
    
    FILE *documento = fopen("Tarea5.csv", "w");
    fprintf(documento, "X,Y,Puntaje\n");
    
    for (int ciclo = 0; ciclo < TOTAL_ITERACIONES; ciclo++) {
        for (int i = 0; i < TAM_GRUPO; i += 2) {
            int p1 = rand() % TAM_GRUPO;
            int p2 = rand() % TAM_GRUPO;
            
            double probabilidad = (double)rand() / RAND_MAX;
            if (probabilidad < TASA_INTERCAMBIO) {
                combinar(grupo_actual[p1], grupo_actual[p2], &descendencia[i], &descendencia[i+1]);
            } else {
                descendencia[i] = grupo_actual[p1];
                descendencia[i+1] = grupo_actual[p2];
            }

            alterar(&descendencia[i]);
            alterar(&descendencia[i+1]);
        }

        calificar_grupo(descendencia, TAM_GRUPO);
        elegir_mejores(grupo_actual, descendencia);
        
        printf("Iteracion %d | Mejor puntaje: %f\n", ciclo + 1, grupo_actual[0].puntaje);
        printf("ADN: ");
        mostrar_bits(grupo_actual[0].adn);
        printf("\n");
        
        uint32_t mitad_x = (uint32_t)(grupo_actual[0].adn >> 32);
        uint32_t mitad_y = (uint32_t)(grupo_actual[0].adn & 0xFFFFFFFF);
        double valor_x = convertir_a_real(mitad_x);
        double valor_y = convertir_a_real(mitad_y);

        fprintf(documento, "%f,%f,%f\n", valor_x, valor_y, grupo_actual[0].puntaje);
    }
    fclose(documento);
    return 0;
}

// --- DESARROLLO DE FUNCIONES ---

uint64_t obtener_aleatorio_64() {
    uint64_t bloque1 = (uint64_t)rand() & 0xFFFF;
    uint64_t bloque2 = (uint64_t)rand() & 0xFFFF;
    uint64_t bloque3 = (uint64_t)rand() & 0xFFFF;
    uint64_t bloque4 = (uint64_t)rand() & 0xFFFF;
    return (bloque1 << 48) | (bloque2 << 32) | (bloque3 << 16) | bloque4;
}

void crear_grupo_inicial(Candidato grupo_actual[]) {
    for (int i = 0; i < TAM_GRUPO; i++) {
        grupo_actual[i].adn = obtener_aleatorio_64();
        grupo_actual[i].puntaje = 0.0; 
    }
}

double convertir_a_real(uint32_t valor_binario) {
    double rango_min = -5.12; 
    double rango_max = 5.12;
    double divisor = (double)UINT32_MAX; 
    
    double proporcion = (double)valor_binario / divisor;
    return rango_min + (proporcion * (rango_max - rango_min));
}

double evaluar_funcion_objetivo(double x, double y) {
    double constante = 10.0;
    double parte_x = (x * x) - constante * cos(2.0 * PI * x);
    double parte_y = (y * y) - constante * cos(2.0 * PI * y);
    return 20.0 + parte_x + parte_y;
}

void calificar_grupo(Candidato grupo_actual[], int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        uint32_t mitad_x = (uint32_t)(grupo_actual[i].adn >> 32);
        uint32_t mitad_y = (uint32_t)(grupo_actual[i].adn & 0xFFFFFFFF);
        
        double valor_x = convertir_a_real(mitad_x);
        double valor_y = convertir_a_real(mitad_y);
        
        grupo_actual[i].puntaje = evaluar_funcion_objetivo(valor_x, valor_y);
    }
}

void combinar(Candidato progenitor1, Candidato progenitor2, Candidato *creacion1, Candidato *creacion2) {
    uint64_t filtro_superior = 0xFFFFFFFF00000000ULL; 
    uint64_t filtro_inferior = ~filtro_superior; 

    creacion1->adn = (progenitor1.adn & filtro_superior) | (progenitor2.adn & filtro_inferior);
    creacion2->adn = (progenitor2.adn & filtro_superior) | (progenitor1.adn & filtro_inferior);
}

void alterar(Candidato *sujeto) {
    for (int i = 0; i < 64; i++) {
        double suerte = (double)rand() / RAND_MAX;
        if (suerte < TASA_ALTERACION) {
            uint64_t bit_objetivo = 1ULL << i;
            sujeto->adn = sujeto->adn ^ bit_objetivo;
        }
    }
}

int ordenar_por_puntaje(const void *a, const void *b) {
    Candidato *cand_A = (Candidato *)a;
    Candidato *cand_B = (Candidato *)b;
    
    if (cand_A->puntaje < cand_B->puntaje) return -1;
    else if (cand_A->puntaje > cand_B->puntaje) return 1;
    else return 0;
}

void elegir_mejores(Candidato grupo_base[], Candidato descendencia[]) {
    Candidato grupo_combinado[TAM_GRUPO * 2];
    
    for (int i = 0; i < TAM_GRUPO; i++) {
        grupo_combinado[i] = grupo_base[i];
        grupo_combinado[i + TAM_GRUPO] = descendencia[i];
    }
    
    qsort(grupo_combinado, TAM_GRUPO * 2, sizeof(Candidato), ordenar_por_puntaje);
    
    for (int i = 0; i < TAM_GRUPO; i++) {
        grupo_base[i] = grupo_combinado[i];
    }
}

void mostrar_bits(uint64_t adn) {
    for (int i = 63; i >= 0; i--) {
        int bit_actual = (adn >> i) & 1;
        printf("%d", bit_actual);
        
        if (i == 32) {
            printf(" | ");
        } 
        else if (i % 8 == 0 && i != 0) {
            printf(" ");
        }
    }
    printf("\n");
}
