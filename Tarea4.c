#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAMANO_TABLA 100

typedef struct Nodo {
    char *clave;
    int valor;
    struct Nodo *siguiente;
} Nodo;

// Estructura de la tabla hash
typedef struct {
    Nodo *elementos[TAMANO_TABLA];
} TablaHash;

// Función Hash sencilla (Suma los caracteres ASCII tamaño)
unsigned int funcion_hash(const char *clave) {
    unsigned int suma = 0;
    while (*clave) {
        suma += *clave;
        clave++;
    }
    return suma % TAMANO_TABLA;
}

// Inicializar la tabla poniendo sus casilleros en NULL
void inicializar_tabla(TablaHash *tabla) {
    for (int i = 0; i < TAMANO_TABLA; i++) {
        tabla->elementos[i] = NULL;
    }
}

// Insertar un nuevo nodo al inicio de la lista del casillero correspondiente
void insertar(TablaHash *tabla, const char *clave, int valor) {
    unsigned int indice = funcion_hash(clave);

    // Crear el nodo e inicializar sus datos
    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    nuevo->clave = (char *)malloc(strlen(clave) + 1);
    strcpy(nuevo->clave, clave);
    nuevo->valor = valor;

    // Enlazar al inicio del casillero (Maneja colisiones)
    nuevo->siguiente = tabla->elementos[indice];
    tabla->elementos[indice] = nuevo;
}

// Buscar comparando simultáneamente CLAVE y VALOR
int buscar(TablaHash *tabla, const char *clave, int valor) {
    unsigned int indice = funcion_hash(clave);
    Nodo *actual = tabla->elementos[indice];

    while (actual != NULL) {
        if (strcmp(actual->clave, clave) == 0 && actual->valor == valor) {
            return 1; 
        }
        actual = actual->siguiente;
    }
    return 0; 
}

// Liberar la memoria usada por los nodos y cadenas
void liberar_tabla(TablaHash *tabla) {
    for (int i = 0; i < TAMANO_TABLA; i++) {
        Nodo *actual = tabla->elementos[i];
        while (actual != NULL) {
            Nodo *auxiliar = actual;
            actual = actual->siguiente;
            free(auxiliar->clave);
            free(auxiliar);
        }
    }
}

int main() {
    TablaHash mi_tabla;
    inicializar_tabla(&mi_tabla);

    //Insertar elementos con el mismo valor pero clave distinta
    insertar(&mi_tabla, "Alhondra", 500);
    insertar(&mi_tabla, "Fabian", 400);
    insertar(&mi_tabla, "Carmen", 350);

    //Generar una colisión (ambas claves suman 195 ASCII, dando el mismo índice 95)
    insertar(&mi_tabla, "ab", 100);
    insertar(&mi_tabla, "ba", 200);

    //Probar Búsqueda pasando clave y valor
    printf("Buscando ('Alhondra', 500): %s\n", 
            buscar(&mi_tabla, "Alhondra", 500) ? "Encontrado" : "No encontrado");
     printf("Buscando ('Fabian', 500): %s\n", 
            buscar(&mi_tabla, "Fabian", 500) ? "Encontrado" : "No encontrado");
     printf("Buscando ('Fabian', 400): %s\n", 
            buscar(&mi_tabla, "Fabian", 400) ? "Encontrado" : "No encontrado");
     printf("Buscando ('Carmen', 350): %s\n", 
            buscar(&mi_tabla, "Carmen", 350) ? "Encontrado" : "No encontrado");
            
    printf("Buscando ('ba', 200) [tras colision]: %s\n", 
            buscar(&mi_tabla, "ba", 200) ? "Encontrado" : "No encontrado");
    printf("Buscando ('ba', 999) [valor incorrecto]: %s\n", 
            buscar(&mi_tabla, "ba", 999) ? "Encontrado" : "No encontrado");
    // Liberar memoria al finalizar
    liberar_tabla(&mi_tabla);
    printf("Memoria liberada correctamente.\n");
    return 0;
}
