#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura para representar un nodo en la lista
typedef struct nodo_persona {
    char nombre_completo[50];
    char ciudad_origen[50];
    int  edad_persona;
    int  id_registro; 
    struct nodo_persona *siguiente;
    struct nodo_persona *anterior;
} Persona; 

// Control de la lista
typedef struct lista_doble {
    Persona *cabeza;
    Persona *cola;
    int total_elementos;
} ListaDoble;

// Asigna memoria para un nuevo registro vacío
Persona *crear_persona() {
    Persona *nueva = (Persona *)malloc(sizeof(Persona));
    if (nueva != NULL) {
        nueva->siguiente = NULL;
        nueva->anterior = NULL;
    }
    return nueva;
}

// Inicializa los nodos centinela de la lista
void inicializar_lista(ListaDoble *lista) {
    Persona *centinela_inicio = crear_persona();
    Persona *centinela_fin = crear_persona();
    
    lista->cabeza = centinela_inicio;
    lista->cola = centinela_fin;
    lista->total_elementos = 0;
    
    lista->cabeza->siguiente = centinela_fin;
    lista->cabeza->anterior = NULL;
    
    lista->cola->anterior = centinela_inicio;
    lista->cola->siguiente = NULL;
}

// Agrega una persona al final de la lista (antes del centinela cola)
int insertar_elemento(ListaDoble *lista, int id, const char *nombre, const char *ciudad, int edad) {
    Persona *nueva_persona = crear_persona();
    
    if (nueva_persona == NULL) return 0;
    
    strcpy(nueva_persona->nombre_completo, nombre);
    strcpy(nueva_persona->ciudad_origen, ciudad);
    nueva_persona->edad_persona = edad;
    nueva_persona->id_registro = id;
    
    // Enlazar el nuevo nodo antes del centinela final
    nueva_persona->anterior = lista->cola->anterior;
    nueva_persona->siguiente = lista->cola;

    lista->cola->anterior->siguiente = nueva_persona;    
    lista->cola->anterior = nueva_persona;
    
    lista->total_elementos++;
    return 1;
}

// Extracción tipo COLA (FIFO)
Persona* desencolar_fifo(ListaDoble *lista) {
    if (lista->cabeza->siguiente == lista->cola) {
        return NULL; // Lista vacía
    }
    
    Persona *primer_nodo = lista->cabeza->siguiente;
    
    // Desconectar el primer nodo
    lista->cabeza->siguiente = primer_nodo->siguiente;
    primer_nodo->siguiente->anterior = lista->cabeza;
    
    lista->total_elementos--;
    return primer_nodo;
}

// Extracción tipo PILA (LIFO)
Persona* desapilar_lifo(ListaDoble *lista) {
    if (lista->cola->anterior == lista->cabeza) {
        return NULL; // Lista vacía
    }
    
    Persona *ultimo_nodo = lista->cola->anterior;
    
    // Desconectar el último nodo
    lista->cola->anterior = ultimo_nodo->anterior;
    ultimo_nodo->anterior->siguiente = lista->cola;
    
    lista->total_elementos--;
    return ultimo_nodo;
}

int main() {
    ListaDoble estructura_cola;
    ListaDoble estructura_pila;
    Persona *persona_extraida;
    
    char nom_aux[50];
    char ciu_aux[50];

    printf("========================================\n");
    printf("        ESTRUCTURA FIFO (COLA)          \n");
    printf("========================================\n");
    inicializar_lista(&estructura_cola);

    // Inserción de datos en la cola
    strcpy(nom_aux, "José");
    strcpy(ciu_aux, "Tamaulipas");
    insertar_elemento(&estructura_cola, 1, nom_aux, ciu_aux, 68);

    strcpy(nom_aux, "Carmen");
    strcpy(ciu_aux, "Chiapas");
    insertar_elemento(&estructura_cola, 2, nom_aux, ciu_aux, 62);

    strcpy(nom_aux, "Walter");
    strcpy(ciu_aux, "Tuxtla Gutierrez");
    insertar_elemento(&estructura_cola, 3, nom_aux, ciu_aux, 42);

    strcpy(nom_aux, "Isaias");
    strcpy(ciu_aux, "San Cristobal");
    insertar_elemento(&estructura_cola, 4, nom_aux, ciu_aux, 36);

    strcpy(nom_aux, "Ulises");
    strcpy(ciu_aux, "Tuxtla Gutierrez");
    insertar_elemento(&estructura_cola, 5, nom_aux, ciu_aux, 26);
    
    printf("Elementos en Cola: %d\n\n", estructura_cola.total_elementos);

    while ((persona_extraida = desencolar_fifo(&estructura_cola)) != NULL) {
        printf("ID: %-2d | Nombre: %-10s | Ciudad: %-12s | Edad: %2d años | Restantes: %d\n",  
               persona_extraida->id_registro, persona_extraida->nombre_completo, 
               persona_extraida->ciudad_origen, persona_extraida->edad_persona, 
               estructura_cola.total_elementos);
        free(persona_extraida); 
    }

    printf("\n========================================\n");
    printf("        ESTRUCTURA LIFO (PILA)          \n");
    printf("========================================\n");
    inicializar_lista(&estructura_pila);

    // Inserción de datos en la pila
    strcpy(nom_aux, "Jose");
    strcpy(ciu_aux, "Tamaulipas");
    insertar_elemento(&estructura_pila, 1, nom_aux, ciu_aux, 68);

    strcpy(nom_aux, "Carmen");
    strcpy(ciu_aux, "Chiapas");
    insertar_elemento(&estructura_pila, 2, nom_aux, ciu_aux, 62);

    strcpy(nom_aux, "Walter");
    strcpy(ciu_aux, "Tuxtla Gutierrez");
    insertar_elemento(&estructura_pila, 3, nom_aux, ciu_aux, 42);

    strcpy(nom_aux, "Isaias");
    strcpy(ciu_aux, "San Cristobal");
    insertar_elemento(&estructura_pila, 4, nom_aux, ciu_aux, 36);

    strcpy(nom_aux, "Ulises");
    strcpy(ciu_aux, "Tuxtla Gutierrez");
    insertar_elemento(&estructura_pila, 5, nom_aux, ciu_aux, 26);
    
    printf("Elementos en la Pila: %d\n\n", estructura_pila.total_elementos);

    while ((persona_extraida = desapilar_lifo(&estructura_pila)) != NULL) {
        printf("ID: %-2d | Nombre: %-10s | Ciudad: %-12s | Edad: %2d años | Restantes: %d\n",
               persona_extraida->id_registro, persona_extraida->nombre_completo, 
               persona_extraida->ciudad_origen, persona_extraida->edad_persona, 
               estructura_pila.total_elementos);
        free(persona_extraida); 
    }

    // Liberación de los nodos centinela
    free(estructura_cola.cabeza); free(estructura_cola.cola);
    free(estructura_pila.cabeza); free(estructura_pila.cola);

    return 0;
}
