#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "huffman.h"
#include "bitwise.h"

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
        return NULL; // lista vacia
    }
    Nodo *minimo = *cabeza;           // El primero es el menor
    *cabeza = (*cabeza)->siguiente;   // La cabeza ahora es el segundo nodo
    minimo->siguiente = NULL;         // Desconectamos el nodo extraído de la lista
    return minimo;
}

// Toma dos nodos y crea un padre que los une
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

// Ciclo para construir el Árbol de Huffman
Nodo* construirArbolHuffman(Nodo **cabeza) {
    // mientras haya más de un nodo en la lista
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

// Liberar memoria de los nodos
void liberarArbol(Nodo *raiz) {
    if (raiz == NULL) {
        return;
    }
    
    // Liberar en Post-Orden: Izq-Der-raiz
    liberarArbol(raiz->izq);
    liberarArbol(raiz->der);
    
    free(raiz);
}

// Inserta un nuevo nodo creado a partir de caracter y frecuencia de forma ordenada
void InsertarNodo_ordenado (Nodo **cabeza, char caracter, int frecuencia) {
    Nodo *nuevo = (Nodo*)malloc(sizeof(*nuevo)); // memoria del nuevo nodo
    if (nuevo == NULL) {
        printf("No hay memoria disponible\n");
        return;
    }

    // Asignamos al primer nodo insertado lo básico
    nuevo->frecuencia = frecuencia;
    nuevo->caracter = caracter;
    nuevo->siguiente = NULL;
    nuevo->izq = NULL;
    nuevo->der = NULL;

    // Creamos nuestros auxiliares Actual y Anterior
    Nodo *actual = *cabeza; // Por ser el primero debe ser cabeza
    Nodo *anterior = NULL;  // No existe anterior por lo tanto es NULL

    // Mientras no sea un valor nulo y la frecuencia actual sea menor que la nueva
    while (actual != NULL && actual->frecuencia < frecuencia) {
        // se avanza de posicion Nodo
        anterior = actual;          
        actual = actual->siguiente;     
    }

    nuevo->siguiente = actual;  // Ahora el nuevo se agarra del actual (Anterior -> Nuevo -> Actual)
    
    if (anterior == NULL) {
        // en caso de que el nuevo nodo sea menor que todos, la nueva cabeza será el nuevo Nodo
        *cabeza = nuevo; 
    } else {
        // se añade normal
        anterior->siguiente = nuevo; 
    }
}

// Imprime la lista enlazada de nodos
void imprimirLista(Nodo *cabeza) {
    Nodo *actual = cabeza; // Aquí el nodo actual es la cabeza
    
    printf("Lista enlazadas de Nodos:\n");
    while (actual != NULL) {
        printf("[ '%c' | Frecuencia: %i ] -> ", actual->caracter, actual->frecuencia); 
        actual = actual->siguiente; 
    }
    printf("NULL\n\n");
}

// Libera memoria de la lista enlazada de forma secuencial
void Borrar_memoria(Nodo *actual, Nodo *siguiente_temp) {
    while (actual != NULL) {
        siguiente_temp = actual->siguiente; 
        free(actual);                      
        actual = siguiente_temp;            
    }
    printf("\n\n\nMemoria borrada");
}

// Abre un archivo y cuenta las frecuencias de cada carácter ASCII
void Archivo_Contar(const char *name_archivo, int frecuencias[]) {
    FILE *archivo = fopen(name_archivo, "r");
    if (archivo == NULL) {
        printf("Hubo un error con el archivo al abrirlo o no existe\n");
        return;
    }
    
    int c;
    while ((c = fgetc(archivo)) != EOF) {
        frecuencias[c]++;
    }

    fclose(archivo);
}

// Imprime las frecuencias almacenadas para depuración
void Imprimir_arreglo_temp(int frecuencias[]) {
    for (int i = 0; i < 256; i++) {
        if (frecuencias[i] > 0) {
            switch (i) {
                case '\n':
                    printf("\nCaracter ['\\n']: %i", frecuencias[i]);
                    break; 
                case '\t':
                    printf("\nCaracter ['\\t']: %i", frecuencias[i]);
                    break;
                case ' ':
                    printf("\nCaracter ['ESPACIO']: %i", frecuencias[i]);
                    break;
                default:
                    if (isprint(i)) {
                        printf("\nCaracter ['%c']: %i", (char)i, frecuencias[i]);
                    } else {
                        printf("\nCaracter [Byte especial %i]: %i", i, frecuencias[i]);
                    }
                    break;
            }
        }
    }
}

// Comprime un archivo utilizando el algoritmo de Huffman
void comprimir_archivo(const char *archivo_entrada) {
    int frecuencias[256] = {0};
    Nodo *lista = NULL; // creamos la raíz de la lista

    Archivo_Contar(archivo_entrada, frecuencias); // lee archivo y añade frecuencias al arreglo

    // Agregamos la información a los nodos
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

        // Generar Diccionario de etiquetas 
        TablaCodigos diccionario = {0}; 
        char codigo_temporal[256];
        
        generarCodigos(raiz, codigo_temporal, 0, diccionario);

        // Escritura de bits
        char archivo_salida[256];
        snprintf(archivo_salida, sizeof(archivo_salida), "%s.bin", archivo_entrada);
        
        guardar_header(archivo_salida, frecuencias);

        FILE *original = fopen(archivo_entrada, "r");
        BitFile *bf = abrir_escritura_bit(archivo_salida);

        if (original && bf) {
            int c;
            while ((c = fgetc(original)) != EOF) {
                // Busca la etiqueta dentro del diccionario
                char *codigo = diccionario[(unsigned char)c];
                
                // Manda los 0's y 1's a la escritura de bits
                for (int i = 0; codigo[i] != '\0'; i++) {
                    escribir_bit(codigo[i] == '1', bf);
                }
            }
            
            // Cerrar archivo y escritura
            fclose(original);
            cerrar_escritura_bit(bf);
            printf("Archivo comprimido exitosamente a: %s\n", archivo_salida);
        }

        printf("Diccionario Huffman\n");
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

    // Liberar la memoria completa del árbol
    liberarArbol(raiz);
    printf("\nMemoria del árbol borrada\n");
}

// Descomprime un archivo binario comprimido con Huffman
void descomprimir_archivo(const char *archivo_entrada, const char *archivo_salida) {
    int frecuencias[256] = {0};
    
    // Lee Header y recupera frecuencias
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

    // Reconstruir el Árbol de Huffman
    Nodo *raiz = construirArbolHuffman(&lista);
    if (raiz == NULL) {
        return;
    }

    // Descompresión
    BitFile *bf = abrir_lectura_bit(archivo_entrada);
    FILE *salida = fopen(archivo_salida, "w");

    if (bf && salida) {
        Nodo *actual = raiz;
        int bit;
        int caracteres_escritos = 0;
        int total_esperado = raiz->frecuencia; // total de letras recuperadas

        // Ir por el árbol bit por bit hasta llegar a alguna hoja
        while (caracteres_escritos < total_esperado && (bit = leer_bit(bf)) != -1) {
            if (bit == 0) {
                actual = actual->izq;
            } else {
                actual = actual->der;
            }

            // Llega a una hoja
            if (actual->izq == NULL && actual->der == NULL) {
                fputc(actual->caracter, salida); // escribir el carácter en el archivo de salida
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
