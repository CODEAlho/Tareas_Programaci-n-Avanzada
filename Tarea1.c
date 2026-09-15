#include <stdio.h>
#include <immintrin.h> 

union Data128 {
    unsigned long long particion64[2];
    unsigned char bytes[16];
};

// Almacenar el resultado completo de 32 bytes (256 bits)
union Data256 {
    unsigned long long particion64[4]; // 4 bloques de 64 bits
    unsigned char bytes[32];
};

int main() {
    union Data128 NumA, NumB;
    union Data256 Resultado = {0}; // Inicializamos en 0

    // Inicializamos los valores
    NumA.particion64[1] = 0x0000000000000005ULL; 
    NumA.particion64[0] = 0x0000000000000003ULL; 

    NumB.particion64[1] = 0x0000000000000008ULL; 
    NumB.particion64[0] = 0x000000000000000AULL; 

    unsigned long long a_lo = NumA.particion64[0];
    unsigned long long a_hi = NumA.particion64[1];
    unsigned long long b_lo = NumB.particion64[0];
    unsigned long long b_hi = NumB.particion64[1];

    // Partes altas y bajas de cada multiplicación
    unsigned long long p0_hi, p0_lo;
    unsigned long long p1_hi, p1_lo;
    unsigned long long p2_hi, p2_lo;
    unsigned long long p3_hi, p3_lo;

    // 1. Calculamos los cuatro particiones
    p0_lo = _mulx_u64(a_lo, b_lo, &p0_hi); // Alo * Blo
    p1_lo = _mulx_u64(a_hi, b_lo, &p1_hi); // Ahi * Blo
    p2_lo = _mulx_u64(a_lo, b_hi, &p2_hi); // Alo * Bhi
    p3_lo = _mulx_u64(a_hi, b_hi, &p3_hi); // Ahi * Bhi

    // 2. Acarreos
    unsigned char acarreo;

    // Bloque 0: La base
    Resultado.particion64[0] = p0_lo;
    Resultado.particion64[1] = p0_hi;

    // Sumamos la particion (desplazado 64 bits)
    acarreo = _addcarry_u64(0, Resultado.particion64[1], p1_lo, &Resultado.particion64[1]);
    acarreo = _addcarry_u64(acarreo, 0, p1_hi, &Resultado.particion64[2]);
    Resultado.particion64[3] = acarreo; 

    // Sumamos la particion 2 (desplazado 64 bits)
    acarreo = _addcarry_u64(0, Resultado.particion64[1], p2_lo, &Resultado.particion64[1]);
    acarreo = _addcarry_u64(acarreo, Resultado.particion64[2], p2_hi, &Resultado.particion64[2]);
    _addcarry_u64(acarreo, Resultado.particion64[3], 0, &Resultado.particion64[3]);

    // Sumamos la particion 3 (desplazado 128 bits)
    acarreo = _addcarry_u64(0, Resultado.particion64[2], p3_lo, &Resultado.particion64[2]);
    _addcarry_u64(acarreo, Resultado.particion64[3], p3_hi, &Resultado.particion64[3]);

    printf("Numero A: 0x%016llX%016llX\n", NumA.particion64[1], NumA.particion64[0]);
    printf("Numero B: 0x%016llX%016llX\n\n", NumB.particion64[1], NumB.particion64[0]);

    printf("Resultado de 256 bits (Hexadecimal):\n");
    printf("0x%016llX %016llX %016llX %016llX\n\n", 
           Resultado.particion64[3], 
           Resultado.particion64[2], 
           Resultado.particion64[1], 
           Resultado.particion64[0]);

    for(int i = 0; i < 32; i++) {
        printf("[%02d] ( %p) = %02X\n", i, (void*)&Resultado.bytes[i], Resultado.bytes[i]);
    }

    return 0;
}

