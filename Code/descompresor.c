#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Huffman.h"
#include "bitwise.h"

void InsertarNodo_ordenado_desc (Nodo **cabeza, char caracter, int frecuencia);

int main(int argc, char *argv[]){

    if (argc < 3) {
        printf("Uso: %s <archivo_comprimido.bin> <salida_descomprimida.txt>\n", argv[0]);
        return 1;
    }

    int frecuencias[256] = {0};
    
    //  Lee Header y recupera frecuencias
    int padding = leer_header(argv[1], frecuencias);
    if (padding == -1) {
        printf("No se pudo leer el archivo comprimido.\n");
        return 1;
    }

    // Reconstruir la lista original
    Nodo *lista = NULL;
    for (int i = 0; i < 256; i++) {
        if (frecuencias[i] > 0) {
            InsertarNodo_ordenado_desc(&lista, (char)i, frecuencias[i]);
        }
    }

    //  Reconstruir el Árbol de Huffman
    Nodo *raiz = construirArbolHuffman(&lista);
    if (raiz == NULL) return 1;

    //  Lógica de Descompresión
    BitFile *bf = abrir_lectura_bit(argv[1]);
    FILE *salida = fopen(argv[2], "w");

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

        printf("Se recuperaron %d caracteres en '%s'.\n", caracteres_escritos, argv[2]);
        
        fclose(salida);
        cerrar_lectura_bit(bf);
    }

    liberarArbol(raiz);

}













void InsertarNodo_ordenado_desc (Nodo **cabeza, char caracter, int frecuencia){


    Nodo *nuevo = (Nodo*)malloc(sizeof(*nuevo)); //memoria del nuevo nodo

        //Asignamos al primer nodo insertado lo basico (por ser unico debe de apuntar a NULL)
        nuevo->frecuencia=frecuencia;
        nuevo->caracter = caracter;
        nuevo->siguiente = NULL;
        nuevo->izq=NULL;
        nuevo->der=NULL;


    // creamos nuestros auxiliares Actual y Anterior

    Nodo *actual = *cabeza; //Por ser el primero debe ser cabeza
    Nodo *anterior = NULL;  // No existe anterior por lo tanto es NULL

    //Minetras no sea un valor nulo y la frecuencia actual sea menor que la nueva
    while (actual != NULL && actual->frecuencia < frecuencia) {
        //se avanza de posicion Nodo
        anterior = actual;          
        actual = actual->siguiente;     
    }

    nuevo->siguiente = actual;  //Ahora el nuevo se agarra del actual           Anterior -> Nuevo -> Actual
    
    if (anterior == NULL) {
        //en caso de que el nuevo nodo sea menor que todos (debido a q anterior es NULL) la nueva cabeza sera el nuevo Nodo
        *cabeza = nuevo; 
    } else {
        //
        anterior->siguiente = nuevo; 
    }

}