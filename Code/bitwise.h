#ifndef BITWISE_IO_H
#define BITWISE_IO_H

#include <stdio.h>


typedef struct {
    FILE *archivo;        
    unsigned char buffer; 
    int contador;         
} BitFile;


void guardar_header(const char *nombre, int frecuencias[256]);
BitFile* abrir_escritura_bit(const char *nombre);
void escribir_bit(int bit, BitFile *bf);
void cerrar_escritura_bit(BitFile *bf);


BitFile* abrir_lectura_bit(const char *nombre);
int leer_bit(BitFile *bf);
void cerrar_lectura_bit(BitFile *bf);
int leer_header(const char *nombre, int frecuencias[256]);

#endif
