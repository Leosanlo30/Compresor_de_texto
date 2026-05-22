#ifndef HUFFMAN_H
#define HUFFMAN_H

// Estructura del nodo del árbol y de la lista enlazada
typedef struct Nodo {
    int frecuencia;
    char caracter;
    struct Nodo *siguiente; 
    struct Nodo *izq;
    struct Nodo *der;       
} Nodo;

// Tabla de códigos de Huffman (256 caracteres, cada uno con código de hasta 255 bits + terminador nulo)
typedef char TablaCodigos[256][256];

// Funciones del Árbol de Huffman
Nodo* crearNodoArbol(char caracter, int frecuencia, Nodo* izq, Nodo* der);
Nodo* extraerMinimo(Nodo **cabeza);
Nodo* unirNodos(Nodo *izq, Nodo *der);
void insertarNodoLista(Nodo **cabeza, Nodo *nuevo);
Nodo* construirArbolHuffman(Nodo **cabeza);
void generarCodigos(Nodo *raiz, char *codigo_actual, int profundidad, TablaCodigos diccionario);
void liberarArbol(Nodo *raiz);

// Funciones auxiliares de lista y conteo
void InsertarNodo_ordenado(Nodo **cabeza, char caracter, int frecuencia);
void Archivo_Contar(const char *archivo, int frecuencias[]);
void Borrar_memoria(Nodo *actual, Nodo *siguiente_temp);
void imprimirLista(Nodo *cabeza);
void Imprimir_arreglo_temp(int frecuencias[]);

// Funciones principales de compresión y descompresión
void comprimir_archivo(const char *archivo_entrada);
void descomprimir_archivo(const char *archivo_entrada, const char *archivo_salida);

#endif // HUFFMAN_H
