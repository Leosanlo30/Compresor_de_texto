#ifndef BITWISE_H
#define BITWISE_H

#include <stdio.h>
#include <stdlib.h>

// Estructura para leer/escribir archivos a nivel de bits
typedef struct {
    FILE *archivo;        
    unsigned char buffer; 
    int contador;         
} BitFile;

// Funciones de escritura de bits y guardado de encabezados
BitFile* abrir_escritura_bit(const char *nombre);
void guardar_header(const char *nombre, int frecuencias[256]);
void escribir_bit(int bit, BitFile *bf);
void cerrar_escritura_bit(BitFile *bf);

// Funciones de lectura de bits y lectura de encabezados
BitFile* abrir_lectura_bit(const char *nombre);
int leer_bit(BitFile *bf);
void cerrar_lectura_bit(BitFile *bf);
int leer_header(const char *nombre, int frecuencias[256]);

#endif // BITWISE_H
