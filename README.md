# Repositorio de Alhondra Gómez Vázquez

## Descripción
Tarea 1: Multiplicación de dos números enteros de 128 bits, capturando el resultado completo de 256 bits sin pérdida de datos (sin desbordamiento).

Utiliza operaciones a nivel de bits y funciones intrínsecas de hardware de Intel (`_mulx_u64` y `_addcarry_u64`) para manejar la aritmética de alta precisión y el encadenamiento de acarreos (carries).

Tarea 2: Implementación de Pila (LIFO) y Cola (FIFO) en C
Estructura Dinámica: Manejo dinámico de memoria mediante `malloc` y `free`.
Nodos Centinela:Utilización de nodos de inicio (`cabeza`) y fin (`cola`) para simplificar la inserción y extracción de elementos.
Doble Enlace:Cada nodo cuenta con punteros `siguiente` y `anterior` para facilitar la manipulación de la lista.
Comportamiento Dual: Demostración de comportamiento FIFO (First In, First Out) y LIFO (Last In, First Out) sobre la misma estructura base.
## Cómo compilar y ejecutar

Tarea 1: Dado que el código utiliza instrucciones intrínsecas modernas de x86, necesitas compilarlo con la bandera `-mbmi2` y `-adx`.

```bash
*Tarea 1: gcc main.c -o multiplicacion -mbmi2
./multiplicacion
*Tarea 2: gcc -o Tarea2.o Tarea2.c
./Tarea2.o





