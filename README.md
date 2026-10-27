# Repositorio de Alhondra Gómez Vázquez

## Descripción
Tarea 1: Multiplicación de dos números enteros de 128 bits, capturando el resultado completo de 256 bits sin pérdida de datos (sin desbordamiento).

Utiliza operaciones a nivel de bits y funciones intrínsecas de hardware de Intel (`_mulx_u64` y `_addcarry_u64`) para manejar la aritmética de alta precisión y el encadenamiento de acarreos (carries).

Tarea 2: Implementación de Pila (LIFO) y Cola (FIFO) en C
Estructura Dinámica: Manejo dinámico de memoria mediante `malloc` y `free`.
Nodos Centinela:Utilización de nodos de inicio (`cabeza`) y fin (`cola`) para simplificar la inserción y extracción de elementos.
Doble Enlace:Cada nodo cuenta con punteros `siguiente` y `anterior` para facilitar la manipulación de la lista.
Comportamiento Dual: Demostración de comportamiento FIFO (First In, First Out) y LIFO (Last In, First Out) sobre la misma estructura base.

Tarea 3: Se evalua un polinomio matemático P(x) de dos maneras diferentes para medir cu´al es más rípido en la CPU, usando el Método escalar y el Método SIMD/AVX (vectorizado)

Tarea 4: Tabla Hash de 100 posiciones con resolución de colisiones mediante listas enlazadas. Utiliza una función hash que suma los valores ASCII de la clave para calcular su casilla (0 a 99). Al insertar, asigna memoria dinámica para cada nodo y su clave, guardándolo al inicio del casillero correspondiente, lo que permite almacenar claves distintas con el mismo valor sin perder datos. La búsqueda recorre la lista del casillero comprobando al mismo tiempo la clave y el valor especificado. Finalmente, la función de liberación limpia todos los nodos y cadenas mediante free() para evitar fugas de memoria.

Tarea 5: Este código implementa un Algoritmo Genético en lenguaje C diseñado para encontrar el valor mínimo de una función matemática compleja (la función de Rastrigin) simulando el proceso de evolución natural.En lugar de probar números al azar, el programa codifica las posibles soluciones (coordenadas X e Y) dentro de un "ADN" de 64 bits. A lo largo de múltiples iteraciones, el algoritmo evalúa qué candidatos tienen el mejor puntaje, los combina dividiendo y mezclando sus bits, y los altera mediante mutaciones aleatorias para generar una nueva descendencia cada vez más apta. Finalmente, registra el progreso de la mejor solución de cada generación en la terminal y exporta los datos a un archivo resultados.csv para su posterior análisis.

## Cómo compilar y ejecutar

Tarea 1: Dado que el código utiliza instrucciones intrínsecas modernas de x86, necesitas compilarlo con la bandera `-mbmi2` y `-adx`.

```bash
*Tarea 1: gcc main.c -o multiplicacion -mbmi2
./multiplicacion
*Tarea 2: gcc -o Tarea2.o Tarea2.c
./Tarea2.o
*Tarea 3: g++ -O3 -mavx2 Tarea3.cpp -o bench_horner
./bench_horner
*Tarea 4: gcc Tarea4.c -o Tarea4
./Tarea4
*Tarea 5: gcc Tarea5.c -o Tarea5 -lm
./Tarea5



