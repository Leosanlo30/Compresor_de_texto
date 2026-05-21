#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



typedef struct Nodo {
    int frecuencia;
    char caracter;
    struct Nodo *siguiente; 
    struct Nodo *izq;
    struct Nodo *der;       
} Nodo;


typedef char TablaCodigos[256][256];

typedef struct {
    FILE *archivo;        
    unsigned char buffer; 
    int contador;         
} BitFile;


// Funciones del Arbol de Huffman

Nodo* crearNodoArbol(char caracter, int frecuencia, Nodo* izq, Nodo* der);
Nodo* extraerMinimo(Nodo **cabeza);
Nodo* unirNodos(Nodo *izq, Nodo *der);
void insertarNodoLista(Nodo **cabeza, Nodo *nuevo);
Nodo* construirArbolHuffman(Nodo **cabeza);
void generarCodigos(Nodo *raiz, char *codigo_actual, int profundidad, TablaCodigos diccionario);
void liberarArbol(Nodo *raiz);

// Funciones de manejo de Bits (Bitwise)

void guardar_header(const char *nombre, int frecuencias[256]);
BitFile* abrir_escritura_bit(const char *nombre);
void escribir_bit(int bit, BitFile *bf);
void cerrar_escritura_bit(BitFile *bf);

BitFile* abrir_lectura_bit(const char *nombre);
int leer_bit(BitFile *bf);
void cerrar_lectura_bit(BitFile *bf);
int leer_header(const char *nombre, int frecuencias[256]);

// Funciones de compresion y descompresion

void InsertarNodo_ordenado(Nodo **cabeza, char caracter, int frecuencia);
void Archivo_Contar(const char *archivo, int frecuencias[]);
void Borrar_memoria(Nodo *actual, Nodo *siguiente_temp);
void imprimirLista(Nodo *cabeza);
void Imprimir_arreglo_temp(int frecuencias[]);
void comprimir_archivo(const char *archivo_entrada);
void descomprimir_archivo(const char *archivo_entrada, const char *archivo_salida);



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

//Toma dos nodos y crea un padre que los une
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


//Manejo de los bits

//Función para abrir el la estructura de bits 
BitFile* abrir_escritura_bit(const char *nombre) {

    BitFile *bf = (BitFile *)malloc(sizeof(BitFile));
    if (!bf) {
        return NULL;
    }
    bf->archivo = fopen(nombre, "r+b"); 
    bf->buffer = 0;
    bf->contador = 0;
    //Saltamos el header: 1024 (frecuencias) + 4 (espacio para padding)
    fseek(bf->archivo, (256 * sizeof(int)) + sizeof(int), SEEK_SET);
    return bf;
}


  //El Header guarda las frecuencias | 256 enteros (1024 bytes) que representan la frecuencia de los caracteres ASCII.
 
void guardar_header(const char *nombre, int frecuencias[256]) {
    FILE *f = fopen(nombre, "wb"); // 'wb': Escritura Binaria. Evita traducciones de caracteres de Windows ya que luego se aloca, esto de hecho solo pasa en windows

    if (!f) {
        return;
    }
    
    //escribe el bloque completo de frecuencias
    fwrite(frecuencias, sizeof(int), 256, f);
    
    //Dejamos un espacio de 4 bytes (int) para el Padding
    //Al cerrar el archivo sabremos cuántos bits sobraron y volveremos aquí a anotarlo
    int padding_inicial = 0;
    fwrite(&padding_inicial, sizeof(int), 1, f);
    fclose(f);
}

//  empaqueta los datos
void escribir_bit(int bit, BitFile *bf) {

    if (!bf || !bf->archivo) {
        return;
    }
    
    //Bitwise shift (<<): Desplazamos los bits actuales a la izquierda para abrir espacio
    //Bitwise or |: Si el bit es 1, encendemos la posición más a la derecha.
    bf->buffer = (bf->buffer<< 1) | (bit ? 1 : 0);
    bf->contador++;

    
    if (bf->contador==8) {
        fputc(bf->buffer, bf->archivo); // Mandamos el byte completo al disco
        bf->buffer = 0;   // Limpiamos el buffer lleno
        bf->contador = 0; // Reinicio del conteo
    }
}

/**
 El Padding.
  Si el mensaje termina y solo tenemos 3 bits en el buffer, no se escribirían al disco
  Esta función empuja esos bits al final y guarda cuántos bits de relleno (ceros) añadimos
  había otra forma de hacerlo, pero no se especificaba cuales bits eran de relleno, lo cual podría traer problemas 
 */
void cerrar_escritura_bit(BitFile *bf) {
    if (!bf || !bf->archivo) {
        return;
    }
    
    int padding_final = 0;
    if (bf->contador> 0) {
        padding_final = 8 - bf->contador;
        bf->buffer <<= padding_final; // movemos los bits válidos a la izquierda
        fputc(bf->buffer, bf->archivo);
    }
    
    // regreso al header (fseek):
    // saltamos los 1024 bytes de frecuencias para actualizar el valor real del padding
    fseek(bf->archivo, 256 * sizeof(int), SEEK_SET);
    fwrite(&padding_final, sizeof(int), 1, bf->archivo);
    
    fclose(bf->archivo);
    free(bf);
}

  //Extraemos el bit usando desplazamientos a la derecha.
int leer_bit(BitFile *bf) {

    if (!bf || !bf->archivo) {
        return -1;
    }
    // Si el buffer está vacío, leemos el siguiente byte del archivo
    if (bf->contador == 0) {
        int byte = fgetc(bf->archivo);

        if (byte == EOF) {
            return -1;
        }
        
        bf->buffer = (unsigned char)byte;
        bf->contador = 8;
    }
    bf->contador--;

    // Usamos bitwise shift (>>) y una máscara (& 1) para aislar el bit que queremos

    int bit = (bf->buffer >> bf->contador) & 1;

    return bit;
}

void cerrar_lectura_bit(BitFile *bf) {
    if (bf) {
        if (bf->archivo) fclose(bf->archivo);
        free(bf);
    }

    
}


// Lee las frecuencias que guardamos al principio
int leer_header(const char *nombre, int frecuencias[256]) {


    FILE *f = fopen(nombre, "rb");
    if (!f) {
        return -1;
    }
    
    fread(frecuencias, sizeof(int), 256, f);
    
    int padding = 0;
    fread(&padding, sizeof(int), 1, f);
    fclose(f);
    
    return padding;
}

// Abre el flujo de bits sin el header
BitFile* abrir_lectura_bit(const char *nombre) {
    BitFile *bf = (BitFile*)malloc(sizeof(BitFile));

    if (!bf) {
        return NULL;
    }
    
    bf->archivo = fopen(nombre, "rb");
    if (!bf->archivo) {
        free(bf);
        return NULL;
    }
    
    bf->buffer = 0;
    bf->contador = 0; //leer bit llenara el contador
    
    // Nos saltamos los 1024 bytes de frecuencias + 4 bytes del padding
    fseek(bf->archivo, (256 * sizeof(int)) + sizeof(int), SEEK_SET);
    
    return bf;
}



void comprimir_archivo(const char *archivo_entrada){
    int frecuencias[256]={0};

    Nodo *lista = NULL; // creamos la raiz de la lista

    Archivo_Contar(archivo_entrada, frecuencias); // lee archivo y añadie frecuencias de arreglo

        //agregamos la informacion a los nodos
    for (int i = 0; i < 256; i++) {
        if (frecuencias[i]>0) {
            InsertarNodo_ordenado(&lista, (char)i, frecuencias[i]);
        }
    }

    imprimirLista(lista);

    // Construir el árbol de Huffman 
    Nodo *raiz = construirArbolHuffman(&lista);

    
    
    if (raiz != NULL) {
        printf("Frecuencia total en la raiz (Total de caracteres): %d\n\n", raiz->frecuencia);

        //  Generar Diccionario de etiquetas 
        TablaCodigos diccionario = {0}; 
        char codigo_temporal[256];
        
        generarCodigos(raiz, codigo_temporal, 0, diccionario);

        //Escritura de bits
        char archivo_salida[256];
        snprintf(archivo_salida, sizeof(archivo_salida), "%s.bin", archivo_entrada);
        
        guardar_header(archivo_salida, frecuencias);

        FILE *original = fopen(archivo_entrada, "r");
        BitFile *bf = abrir_escritura_bit(archivo_salida);

        if (original && bf) {
            int c;
            while ((c = fgetc(original)) != EOF) {

            // busca la etiqueta dentro del dinccionario
            char *codigo = diccionario[(unsigned char)c];
            
            // manda los 0's y 1's a la escritura de bits
            for (int i = 0; codigo[i] != '\0'; i++) {
                escribir_bit(codigo[i] == '1', bf);
                }
        }
        
        //cerrar archivo y escritura
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
                } else if (i == '\t') 
                {
                    printf("Caracter ['\\t']: %s\n", diccionario[i]);
                } else {
                    printf("Caracter [Byte %d]: %s\n", i, diccionario[i]);
                }
            }
        }
    }

    

    //Liberar la memoria completa del árbol
    liberarArbol(raiz);
    printf("\nMemoria del árbol borrada\n");

}



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
        //se añade normal
        anterior->siguiente = nuevo; 
    }

}


void imprimirLista(Nodo *cabeza) {
    Nodo *actual = cabeza; //Aqui el nodo actual es la cabeza
    
    printf("Lista enlazadas de Nodos:\n");
    while (actual != NULL) {
        printf("[ '%c' | Frecuencia: %i ] -> ", actual->caracter, actual->frecuencia); 
        actual = actual->siguiente; 
    }
    printf("NULL\n\n");
}

void Borrar_memoria(Nodo *actual, Nodo *siguiente_temp){


    //Borrar memoria
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
        printf("Hubo un error con el archivo al abrirlo o no existe\n");
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

    //Descompresión
    BitFile *bf = abrir_lectura_bit(archivo_entrada);
    FILE *salida = fopen(archivo_salida, "w");

    if (bf && salida) {
        Nodo *actual = raiz;
        int bit;
        int caracteres_escritos = 0;
        int total_esperado = raiz->frecuencia; // total de letras recuperadas


        // ir por el árbol bit por bit hasta llegar a alguna hoja
        while (caracteres_escritos < total_esperado && (bit = leer_bit(bf)) != -1) {

            if (bit == 0) {
                actual = actual->izq;
            } else {
                actual = actual->der;
            }

            // llega a una hoja
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


int main(int argc, char *argv[]) {
    printf("=======================================\n");
    printf("       Compresor de Texto Huffman      \n");
    printf("=======================================\n\n");

    if (argc >= 4 && strcmp(argv[1], "-d") == 0) {
        printf("-> Modo Descompresion por Terminal\n");
        descomprimir_archivo(argv[2], argv[3]);
        return 0;
    } else if (argc >= 3 && strcmp(argv[1], "-c") == 0) {
        printf("-> Modo Compresion por Terminal\n");
        comprimir_archivo(argv[2]);
        return 0;
    }

    // Si no hay argumentos, mostrar el menu interactivo
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
