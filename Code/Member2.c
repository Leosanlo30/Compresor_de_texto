#include "bitwise.h"
#include <stdlib.h>



/**
  El Header garantiza la persistencia.
  guarda 256 enteros (1024 bytes) que representan la frecuencia de los caracteres ASCII.
  Esto permite reconstruir el arbol en cualquier compu.
 */
void guardar_header(const char *nombre, int frecuencias[256]) {
    FILE *f = fopen(nombre, "wb"); // 'wb': Escritura Binaria. Evita traducciones de caracteres de Windows ya que luego se aloca, esto de hecho solo pasa en windows
    if (!f) return;
    
    // escribimos el bloque completo de frecuencias
    fwrite(frecuencias, sizeof(int), 256, f);
    
    // Dejamos un espacio de 4 bytes (int) para el Padding
    // Al cerrar el archivo sabremos cuántos bits sobraron y volveremos aquí a anotarlo
    int padding_inicial = 0;
    fwrite(&padding_inicial, sizeof(int), 1, f);
    fclose(f);
}

/**
  EXPLICACIÓN: La función central de compresión.
  Aquí aplicamos los operadores Bitwise para empaquetar datos.
 */
void escribir_bit(int bit, BitFile *bf) {
    if (!bf || !bf->archivo) return;
    
    //  Bitwise shift (<<): Desplazamos los bits actuales a la izquierda para abrir espacio
    //  Bitwise or |: Si el bit es 1, encendemos la posición más a la derecha.
    bf->buffer = (bf->buffer << 1) | (bit ? 1 : 0);
    bf->contador++;
    //usamos esto para que no se rompa el flujo, solo acepte bin 

    // Cuando el contador llega a 8 nos indica que el buffer está full
    if (bf->contador == 8) {
        fputc(bf->buffer, bf->archivo); // Mandamos el byte completo al disco
        bf->buffer = 0;   // Limpiamos el buffer debido a que ya está full
        bf->contador = 0; // Reiniciamos el conteo 
    }
}

/**
 El Padding.
  Si el mensaje termina y solo tenemos 3 bits en el buffer, no se escribirían al disco
  Esta función empuja esos bits al final y guarda cuántos bits de relleno (ceros) añadimos
  había otra forma de hacerlo, pero no se especificaba cuales bits eran de relleno, lo cual podría traer problemas 
 */
void cerrar_escritura_bit(BitFile *bf) {
    if (!bf || !bf->archivo) return;
    
    int padding_final = 0;
    if (bf->contador > 0) {
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

/**
  leer_bit es el proceso inverso. 
  Extraemos el bit más significativo (MSB) usando desplazamientos a la derecha.
 */
int leer_bit(BitFile *bf) {
    if (!bf || !bf->archivo) return -1;
    // Si el buffer está vacío, leemos el siguiente byte del archivo
    if (bf->contador == 0) {
        int byte = fgetc(bf->archivo);
        if (byte == EOF) return -1;
        
        bf->buffer = (unsigned char)byte;
        bf->contador = 8;
    }

    bf->contador--;
    // Usamos bitwise shift (>>) y una máscara (& 1) para aislar el bit que queremos.
    int bit = (bf->buffer >> bf->contador) & 1;
    
    return bit;
}

void cerrar_lectura_bit(BitFile *bf) {
    if (bf) {
        if (bf->archivo) fclose(bf->archivo);
        free(bf);
    }
}