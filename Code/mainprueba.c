#include <stdio.h>
#include <stdlib.h>
#include "bitwise.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <archivo_a_procesar>\n", argv[0]);
        return 1;
    }

    int frecs[256] = {0};
    FILE *entrada = fopen(argv[1], "rb");
    if (!entrada) {
        printf("Error: No se pudo abrir el archivo de entrada.\n");
        return 1;
    }

    //Esto nos sirve para analizar el archivo  (Contar frecuencias reales)
    int c;
    while ((c = fgetc(entrada)) != EOF) {
        frecs[c]++;
    }
    rewind(entrada); // Regresamos al inicio del archivo para leerlo de nuevo

    printf("Procesando '%s'...\n", argv[1]);
    
    //   Crea el archivo .huff 
    guardar_header("resultado.huff", frecs);
    
    // Inicia el flujo de bits 
    BitFile *bf = abrir_escritura_bit("resultado.huff");
    if (!bf) return 1;

    
    // Aquí simulamos que escribimos bits 
    while ((c = fgetc(entrada)) != EOF) {
        escribir_bit(1, bf); // Simulación: escribimos un '1' por cada carácter
        escribir_bit(0, bf); // Simulación: escribimos un '0' por cada carácter
    }

    // 5. Cerrar y aplicar padding 
    cerrar_escritura_bit(bf);
    fclose(entrada);
    
    printf("¡Exito! Se genero .huff basado en '%s'.\n", argv[1]);
}