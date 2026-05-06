#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/Huffman.h"
#include "../include/bitwise.h"

void descomprimir_archivo(const char *archivo_entrada, const char *archivo_salida){

    int frecuencias[256] = {0};
    
    //  Lee Header y recupera frecuencias
    int padding = leer_header(archivo_entrada, frecuencias);
    if (padding == -1) {
        printf("No se pudo leer el archivo comprimido.\n");
        return;
    }

    // Reconstruir la lista original
    Nodo *lista = NULL;
    for (int i = 0; i < 256; i++) {
        if (frecuencias[i] > 0) {
            InsertarNodo_ordenado(&lista, (char)i, frecuencias[i]);
        }
    }

    //  Reconstruir el Árbol de Huffman
    Nodo *raiz = construirArbolHuffman(&lista);
    if (raiz == NULL) return;

    //  Lógica de Descompresión
    BitFile *bf = abrir_lectura_bit(archivo_entrada);
    FILE *salida = fopen(archivo_salida, "w");

    if (bf && salida) {
        Nodo *actual = raiz;
        int bit;
        int caracteres_escritos = 0;
        int total_esperado = raiz->frecuencia; // ¡El total de letras reales!


        // ir por el árbol bit por bit hasta llegar a alguna hoja
        while (caracteres_escritos < total_esperado && (bit = leer_bit(bf)) != -1) {

            if (bit == 0) {
                actual = actual->izq;
            } else {
                actual = actual->der;
            }

            // llegando a una hoja
            if (actual->izq == NULL && actual->der == NULL) {
                fputc(actual->caracter, salida); // escribir el carácter en el .txt
                caracteres_escritos++;
                actual = raiz; // Volver a la raíz para el siguiente carácter
            }
        }

        printf("Se recuperaron %d caracteres en '%s'.\n", caracteres_escritos, archivo_salida);
        
        fclose(salida);
        cerrar_lectura_bit(bf);
    }

    liberarArbol(raiz);

}