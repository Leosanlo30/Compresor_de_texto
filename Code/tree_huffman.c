#include <stdio.h>
#include <stdlib.h>
#include "Huffman.h"


Nodo* crearNodoArbol(char caracter, int frecuencia, Nodo* izq, Nodo* der);
Nodo* extraerMinimo(Nodo **cabeza);
Nodo* unirNodos(Nodo *izq, Nodo *der);
void insertarNodoLista(Nodo **cabeza, Nodo *nuevo);
Nodo* construirArbolHuffman(Nodo **cabeza);

// Crea un nodo genérico preparado para el árbol y la lista
Nodo* crearNodoArbol(char caracter, int frecuencia, Nodo* izq, Nodo* der) {
    Nodo *nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (nuevo == NULL) {
        printf("Error: No hay memoria suficiente.\n");
        exit(1);
    }
    nuevo->caracter = caracter;
    nuevo->frecuencia = frecuencia;
    nuevo->siguiente = NULL;
    nuevo->izq = izq;
    nuevo->der = der;
    return nuevo;
}

// Función de Extracción: Saca el primer nodo de la lista (que es el de menor frecuencia)
Nodo* extraerMinimo(Nodo **cabeza) {
    if (*cabeza == NULL) {
        return NULL; // La lista está vacía
    }
    Nodo *minimo = *cabeza;           // El primero es el menor (la lista ya está ordenada)
    *cabeza = (*cabeza)->siguiente;   // La cabeza ahora es el segundo nodo
    minimo->siguiente = NULL;         // Desconectamos el nodo extraído de la lista
    return minimo;
}

// Función de Unión: Toma dos nodos y crea un padre que los une
Nodo* unirNodos(Nodo *izq, Nodo *der) {

    // La frecuencia es la suma de las frecuencias de sus hijos
    return crearNodoArbol('*', izq->frecuencia + der->frecuencia, izq, der);
}

// Insertar un nodo ya existente de vuelta a la lista enlazada manteniendo el orden de frecuencia
void insertarNodoLista(Nodo **cabeza, Nodo *nuevo) {
    // Si la lista está vacía o el nuevo nodo tiene menor frecuencia que la cabeza actual
    if (*cabeza == NULL || (*cabeza)->frecuencia >= nuevo->frecuencia) {
        nuevo->siguiente = *cabeza;
        *cabeza = nuevo;
        return;
    }

    // Buscar el lugar adecuado (avanzamos mientras el siguiente sea menor que nuestro nuevo nodo)
    Nodo *actual = *cabeza;
    while (actual->siguiente != NULL && actual->siguiente->frecuencia < nuevo->frecuencia) {
        actual = actual->siguiente;
    }

    // Insertamos entre "actual" y "actual->siguiente"
    nuevo->siguiente = actual->siguiente;
    actual->siguiente = nuevo;
}

// Ciclo principal para construir el Árbol de Huffman
Nodo* construirArbolHuffman(Nodo **cabeza) {
    // Mientras haya más de un nodo en la lista
    while (*cabeza != NULL && (*cabeza)->siguiente != NULL) {
        // Extraer los dos nodos con menor frecuencia (los primeros dos de la lista)
        Nodo *izq = extraerMinimo(cabeza);
        Nodo *der = extraerMinimo(cabeza);

        // Unirlos en un nodo padre
        Nodo *padre = unirNodos(izq, der);

        // Reinsertar el padre en la lista
        insertarNodoLista(cabeza, padre);
    }

    return extraerMinimo(cabeza);
}

// Función recursiva para navegar el árbol y guardar los códigos binarios en la TablaCodigos
void generarCodigos(Nodo *raiz, char *codigo_actual, int profundidad, TablaCodigos diccionario) {
    if (raiz == NULL) return;

    // Si llegamos a una hoja, guardamos el código generado hasta este punto
    if (raiz->izq == NULL && raiz->der == NULL) {
        codigo_actual[profundidad] = '\0'; // Cerramos la cadena C
        
        // Copiamos el código en la fila correspondiente a ese carácter
        for (int i = 0; i <= profundidad; i++) {
            diccionario[(unsigned char)raiz->caracter][i] = codigo_actual[i];
        }
        return;
    }

    // Si nos vamos por la rama izquierda, añadimos un '0' al código temporal
    codigo_actual[profundidad] = '0';
    generarCodigos(raiz->izq, codigo_actual, profundidad + 1, diccionario);

    // Si nos vamos por la rama derecha, añadimos un '1' al código temporal
    codigo_actual[profundidad] = '1';
    generarCodigos(raiz->der, codigo_actual, profundidad + 1, diccionario);
}

// Función recursiva para liberar todos los nodos creados dinámicamente en el Árbol de Huffman
void liberarArbol(Nodo *raiz) {
    if (raiz == NULL) return;
    
    // Liberar en Post-Orden: Primero recorremos a los hijos, luego liberamos al padre
    liberarArbol(raiz->izq);
    liberarArbol(raiz->der);
    
    free(raiz);
}