#include "bitwise.h"

// Función para abrir el flujo de escritura a nivel de bits
BitFile* abrir_escritura_bit(const char *nombre) {
    BitFile *bf = (BitFile *)malloc(sizeof(BitFile));
    if (!bf) {
        return NULL;
    }
    bf->archivo = fopen(nombre, "r+b"); 
    if (!bf->archivo) {
        free(bf);
        return NULL;
    }
    bf->buffer = 0;
    bf->contador = 0;
    // Saltamos el header: 256 enteros (frecuencias) + 4 bytes (espacio para padding)
    fseek(bf->archivo, (256 * sizeof(int)) + sizeof(int), SEEK_SET);
    return bf;
}

// Guarda las frecuencias y deja espacio para el padding de bits al inicio del archivo binario
void guardar_header(const char *nombre, int frecuencias[256]) {
    FILE *f = fopen(nombre, "wb"); // 'wb': Escritura Binaria
    if (!f) {
        return;
    }
    
    // Escribe el bloque completo de frecuencias (1024 bytes)
    fwrite(frecuencias, sizeof(int), 256, f);
    
    // Dejamos un espacio de 4 bytes (int) para el Padding
    // Al cerrar el archivo sabremos cuántos bits sobraron y volveremos aquí a anotarlo
    int padding_inicial = 0;
    fwrite(&padding_inicial, sizeof(int), 1, f);
    fclose(f);
}

// Empaqueta los bits individuales en bytes y los escribe al disco cuando el buffer se llena (8 bits)
void escribir_bit(int bit, BitFile *bf) {
    if (!bf || !bf->archivo) {
        return;
    }
    
    // Bitwise shift (<<): Desplazamos los bits actuales a la izquierda para abrir espacio
    // Bitwise or |: Si el bit es 1, encendemos la posición más a la derecha.
    bf->buffer = (bf->buffer << 1) | (bit ? 1 : 0);
    bf->contador++;

    if (bf->contador == 8) {
        fputc(bf->buffer, bf->archivo); // Mandamos el byte completo al disco
        bf->buffer = 0;   // Limpiamos el buffer lleno
        bf->contador = 0; // Reinicio del conteo
    }
}

// Empuja los bits restantes del buffer, calcula y escribe el padding real en el encabezado
void cerrar_escritura_bit(BitFile *bf) {
    if (!bf || !bf->archivo) {
        return;
    }
    
    int padding_final = 0;
    if (bf->contador > 0) {
        padding_final = 8 - bf->contador;
        bf->buffer <<= padding_final; // movemos los bits válidos a la izquierda
        fputc(bf->buffer, bf->archivo);
    }
    
    // Regreso al header para actualizar el valor real del padding
    // saltamos los 1024 bytes de frecuencias para posicionarnos en el padding
    fseek(bf->archivo, 256 * sizeof(int), SEEK_SET);
    fwrite(&padding_final, sizeof(int), 1, bf->archivo);
    
    fclose(bf->archivo);
    free(bf);
}

// Extrae un bit individual a la vez desplazando los bytes leídos a la derecha
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

// Cierra el flujo de lectura y libera la memoria
void cerrar_lectura_bit(BitFile *bf) {
    if (bf) {
        if (bf->archivo) {
            fclose(bf->archivo);
        }
        free(bf);
    }
}

// Lee las frecuencias y el padding desde el encabezado del archivo
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

// Abre el flujo de bits sin el encabezado, listándose para la lectura secuencial de bits
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
    bf->contador = 0; // leer_bit llenará el contador al leer
    
    // Nos saltamos los 1024 bytes de frecuencias + 4 bytes del padding
    fseek(bf->archivo, (256 * sizeof(int)) + sizeof(int), SEEK_SET);
    
    return bf;
}
