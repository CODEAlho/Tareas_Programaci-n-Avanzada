# Multiplicación de 128 bits a 256 bits en C

Multiplicación de dos números enteros de 128 bits, capturando el resultado completo de 256 bits sin pérdida de datos (sin desbordamiento).

Utiliza operaciones a nivel de bits y funciones intrínsecas de hardware de Intel (`_mulx_u64` y `_addcarry_u64`) para manejar la aritmética de alta precisión y el encadenamiento de acarreos (carries).

## Cómo compilar y ejecutar

Dado que el código utiliza instrucciones intrínsecas modernas de x86, necesitas compilarlo con la bandera `-mbmi2` y `-adx`.

```bash
gcc main.c -o multiplicacion -mbmi2
./multiplicacion
