#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/Huffman.h"

int main(int argc, char *argv[]) {
    // Modo automático por línea de comandos (Para GitHub Actions / Automatización)
    if (argc >= 3) {
        if (strcmp(argv[1], "-c") == 0) {
            printf("--- Iniciando Compresion Automatica de '%s' ---\n", argv[2]);
            comprimir_archivo(argv[2]);
            return 0;
        } else if (strcmp(argv[1], "-d") == 0) {
            char archivo_salida[256];
            snprintf(archivo_salida, sizeof(archivo_salida), "%s_recuperado.txt", argv[2]);
            printf("--- Iniciando Descompresion Automatica de '%s' ---\n", argv[2]);
            descomprimir_archivo(argv[2], archivo_salida);
            return 0;
        }
    }

    // Modo interactivo (Menú)
    int opcion;
    char archivo_actual[100] = "data/archivotest.txt"; // Archivo por defecto

    do {
        printf("\n========================================\n");
        printf("           COMPRESOR HUFFMAN            \n");
        printf("========================================\n");
        printf(" Archivo seleccionado: %s\n\n", archivo_actual);
        printf("  [1] Elegir archivo .txt\n");
        printf("  [2] Comprimir archivo\n");
        printf("  [3] Descomprimir archivo\n");
        printf("  [4] Salir\n");
        printf("========================================\n");
        printf("Elige una opcion: ");
        
        // Validamos que el usuario ingrese un número
        if (scanf("%d", &opcion) != 1) {
            while(getchar() != '\n'); // Limpiamos el buffer si ingresa una letra
            opcion = 0;
        }

        switch (opcion) {
            case 1:
                printf("\nEscribe el nombre del archivo (ej. datos.txt): ");
                scanf("%s", archivo_actual);
                break;
                
            case 2:
                printf("\n--- Iniciando Compresion de '%s' ---\n", archivo_actual);
                comprimir_archivo(archivo_actual);
                break;
                
            case 3: {
                char archivo_comprimido[105];
                char archivo_salida[105];
                
                // Generamos los nombres automáticamente basados en tu archivo original
                snprintf(archivo_comprimido, sizeof(archivo_comprimido), "%s.bin", archivo_actual);
                snprintf(archivo_salida, sizeof(archivo_salida), "%s_recuperado.txt", archivo_actual);
                
                printf("\n--- Iniciando Descompresion de '%s' ---\n", archivo_comprimido);
                descomprimir_archivo(archivo_comprimido, archivo_salida);
                break;
            }
            case 4:
                printf("\nSaliendo del programa. ¡Exito en tu proyecto!\n");
                break;
                
            default:
                printf("\nOpcion invalida. Intenta de nuevo.\n");
        }
    } while (opcion != 4);

    return 0;
}