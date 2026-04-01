#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Huffman.h"

void InsertarNodo_ordenado (Nodo **cabeza, char caracter, int frecuencia);
void Archivo_Contar(const char *archivo, int frecuencias[]);
void Borrar_memoria(Nodo *actual, Nodo *siguiente_temp);


void imprimirLista(Nodo *cabeza);
void Imprimir_arreglo_temp(int frecuencias[]);

int main(){
    int frecuencias[256]={0};

    Nodo *lista = NULL; // se crea la lista de tipo Nodo en donde se agregaran los Nodos (Inicia en valor NULL porque no hay nada)

    Archivo_Contar("archivotest.txt", frecuencias); // añadir frecuencias de arreglo

        //agregamos la informacion a los nodos
    for (int i = 0; i < 256; i++) {
        if (frecuencias[i] > 0) {
            InsertarNodo_ordenado(&lista, (char)i, frecuencias[i]);
        }
    }

    imprimirLista(lista);

    // Construir el árbol de Huffman 
    Nodo *raiz = construirArbolHuffman(&lista);
    
    if (raiz != NULL) {
        printf("Frecuencia total en la raiz (Total de caracteres): %d\n\n", raiz->frecuencia);

        // 2. Generar Diccionario de Códigos 
        TablaCodigos diccionario = {0}; // Inicializar todos los sub-arreglos en '\0'
        char codigo_temporal[256];
        
        generarCodigos(raiz, codigo_temporal, 0, diccionario);

        printf("--- DICCIONARIO HUFFMAN ---\n");
        for (int i = 0; i < 256; i++) {
            if (diccionario[i][0] != '\0') {
               if (isprint(i) && i != ' ') {
                    printf("Caracter ['%c']: %s\n", (char)i, diccionario[i]);
                } else if (i == ' ') {
                    printf("Caracter ['ESPACIO']: %s\n", diccionario[i]);
                } else if (i == '\n') {
                    printf("Caracter ['\\n']: %s\n", diccionario[i]);
                } else if (i == '\t') {
                    printf("Caracter ['\\t']: %s\n", diccionario[i]);
                } else {
                    printf("Caracter [Byte %d]: %s\n", i, diccionario[i]);
                }
            }
        }
    }

    // 3. Liberar la memoria completa del árbol en lugar de la lista plana
    liberarArbol(raiz);
    printf("\n\n\nMemoria del árbol borrada exitosamente.\n");}



void InsertarNodo_ordenado (Nodo **cabeza, char caracter, int frecuencia){


    Nodo *nuevo = (Nodo*)malloc(sizeof(*nuevo)); //memoria del nuevo nodo

    if (nuevo==NULL) {
        printf("No hay memoria disponible");
        return;
    }

        //Asignamos al primer nodo insertado lo basico (por ser unico debe de apuntar a NULL)
        nuevo->frecuencia=frecuencia;
        nuevo->caracter = caracter;
        nuevo->siguiente = NULL;


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


void imprimirLista(Nodo *cabeza) {
    Nodo *actual = cabeza; // Aqui el nodo actual es la cabeza
    
    printf("Lista enlazadas de Nodos:\n");
    while (actual != NULL) {
        printf("[ '%c' | Frecuencia: %i ] -> ", actual->caracter, actual->frecuencia); 
        actual = actual->siguiente; 
    }
    printf("NULL\n\n");
}

void Borrar_memoria(Nodo *actual, Nodo *siguiente_temp){


    //Borrar memoria
    //Recorre los nodos y libera cada uno
    while (actual != NULL) {
        siguiente_temp = actual->siguiente; 
        free(actual);                      
        actual = siguiente_temp;            
    }
    printf("\n\n\nMemoria borrada");
}

void Archivo_Contar(const char *name_archivo, int frecuencias[]){

    FILE *archivo =fopen(name_archivo, "r");

    if (archivo==NULL) {
        printf("Hubo un error con el archivo al abrirlo o no existe");
        return;
    }
    

    int c;
    while ((c=fgetc(archivo))!=EOF) {
        frecuencias[c]++;
    }

    fclose(archivo);
}

void Imprimir_arreglo_temp(int frecuencias[]){


    for (int i=0; i<256; i++) {
        if (frecuencias[i]>0) {
            switch (i) {
                //caracteres especiales
                case '\n':
                    printf("\nCaracter ['\\n']: %i",frecuencias[i]);
                    break; 
                case '\t':
                    printf("\nCaracter ['\\t']: %i",frecuencias[i]);
                    break;
                case ' ':
                    printf("\nCaracter ['ESPACIO']: %i",frecuencias[i]);
                    break;
                default:
                    // caracteres normales
                    if (isprint(i)) {
                        printf("\nCaracter ['%c']: %i", (char)i, frecuencias[i]);
                    } else {
                        // caracteres raros
                        printf("\nCaracter [Byte especial %i]: %i", i, frecuencias[i]);
                    }
                    break;
            }
        }
    }
    

    
}
