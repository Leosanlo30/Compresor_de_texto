#include <stdio.h>
#include <string.h>
#include "Librerias/huffman.h"

int main(int argc, char *argv[]) {
    // Si se pasan argumentos por consola
    if (argc >= 4 && strcmp(argv[1], "-d") == 0) {
        printf("-> Modo Descompresion por Terminal\n");
        descomprimir_archivo(argv[2], argv[3]);
        return 0;
    } else if (argc >= 3 && strcmp(argv[1], "-c") == 0) {
        printf("-> Modo Compresion por Terminal\n");
        comprimir_archivo(argv[2]);
        return 0;
    }

    // Si no hay argumentos, mostrar el menú interactivo
    int opcion;
    char archivo_in[256];
    char archivo_out[256];
    
    printf("Selecciona una opcion:\n");
    printf("  1. Comprimir archivo\n");
    printf("  2. Descomprimir archivo\n");
    printf("  3. Salir\n");
    printf("\nOpcion: ");
    
    if (scanf("%d", &opcion) != 1) {
        printf("Error al leer la opcion.\n");
        return 1;
    }
    
    switch (opcion) {
        case 1:
            printf("\nIngresa el nombre del archivo a comprimir (ej. texto.txt): ");
            scanf("%s", archivo_in);
            comprimir_archivo(archivo_in);
            break;
        case 2:
            printf("\nIngresa el nombre del archivo a descomprimir (ej. texto.txt.bin): ");
            scanf("%s", archivo_in);
            printf("Ingresa el nombre del archivo de salida (ej. texto_recuperado.txt): ");
            scanf("%s", archivo_out);
            descomprimir_archivo(archivo_in, archivo_out);
            break;
        case 3:
            printf("\nSaliendo\n");
            break;
        default:
            printf("\nOpcion invalida\n");
            break;
    }

    return 0;
}
