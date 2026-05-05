#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaramos las firmas de las funciones que harán el trabajo pesado
void comprimir_archivo(const char *archivo_entrada);
void descomprimir_archivo(const char *archivo_entrada, const char *archivo_salida);

int main() {
    int opcion;
    char archivo_actual[100] = "archivotest.txt"; // Archivo por defecto

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
                snprintf(archivo_salida, sizeof(archivo_salida), "recuperado_%s", archivo_actual);
                
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