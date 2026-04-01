#ifndef HUFFMAN_H
#define HUFFMAN_H

// Estructura unificada del Nodo (Sirve para la lista y para el árbol)
typedef struct Nodo {
    int frecuencia;
    char caracter;
    struct Nodo *siguiente; 
    struct Nodo *izq;
    struct Nodo *der;       
} Nodo;

// Definición de la Tabla de Códigos (diccionario bidimensional)
// [256] posiciones para los caracteres ASCII, [256] char max longitud
typedef char TablaCodigos[256][256];

// Declaraciones de funciones compartidas
Nodo* construirArbolHuffman(Nodo **cabeza);
void generarCodigos(Nodo *raiz, char *codigo_actual, int profundidad, TablaCodigos diccionario);
void liberarArbol(Nodo *raiz);

#endif // HUFFMAN_H
