# 📁 Documentación Técnica - Compresor de Texto (Huffman)

## 1. Arquitectura del Sistema
El proyecto está escrito enteramente en lenguaje **C** y diseñado para utilizar el menor grado de dependencias externas.
Se trata de una aplicación de línea de comandos cuyo núcleo (Core Engine) es el algoritmo de **Codificación de Huffman**, un método de compresión de longitud variable y sin pérdida (Lossless).

---

## 2. Abstracción y Estructuras de Datos

### 2.1 Tabla / Arreglo de Frecuencias
Se utiliza un diseño muy ligero en el conteo de frecuencias:
```c
int frecuencias[256] = {0};
```
Cada índice corresponde nominalmente a los 256 valores posibles definidos por la tabla **ASCII Extendida**.

### 2.2 Nodo de Lista Enlazada (Cola de Prioridad)
Utilizado para iterar y extraer siempre los dos valores más ligeros.
```c
typedef struct Nodo{
    int frecuencia;
    char caracter;
    struct Nodo *siguiente;
} Nodo;
```
La función especializada [InsertarNodo_ordenado](file:///Users/misael30/Desktop/Project-PES/Compresor_de_texto/Code/Frecuencia_Nodos.c#47-86) garantiza inserciones `O(N)`, manteniendo el arreglo siempre listo para consultas ávidas.

### 2.3 Nodo del Árbol (Propuesto)
Será utilizado durante las Fases 2 y 3.
```c
typedef struct NodoHuffman {
    char caracter;               // Valor de la hoja (o nulo para interiores)
    unsigned int frecuencia;     // Suma de hijos
    struct NodoHuffman *izq;     // Bit 0
    struct NodoHuffman *der;     // Bit 1
} NodoHuffman;
```

---

## 3. Algoritmos Principales

### 3.1 Conteo Inicial Automático
A través de la función [Archivo_Contar](file:///Users/misael30/Desktop/Project-PES/Compresor_de_texto/Code/Frecuencia_Nodos.c#112-129):
1. Se inicializa el flujo base por medio de un puntero `FILE *` en modo "Lectura" (`"r"`).
2. Se utiliza el motor de captura de caracteres `fgetc`.
3. Para cada lectura antes del valor `EOF`, se incrementa la contidad de la posición correspondiente en memoria del arreglo principal.

### 3.2 El Árbol de Huffman (En Desarrollo)
Es un **Árbol Binario Estricto** construido de forma Invertida (de las Hojas a la Raíz). 
El uso de la propiedad principal (caracteres de menor frecuencia hasta abajo en el plano) asegura que a sus codificaciones pre-establecidas (`0` y `1`) se les asigne una mayor representación binaria local.

### 3.3 El Generador del Diccionario (En Desarrollo)
Consistirá en un recorrido **Pre-Orden Recursivo** (Padre ➜ Izquierdo ➜ Derecho). A medida que la pila de memoria baje por las ramificaciones, se apilarán bits al temporal de su cadena; si se encuentra una punta (Nodo Hoja), se grabará la memoria actual dentro de la matriz plana de **256 Tablas de Código**.

---

## 4. Gestión de Archivos y Especificaciones de Compresión
- **Archivos de Entrada Esperados:** Textos planos sin decodificaciones estrictas o documentos con extensiones basadas en texto (ej. `.txt`, `.csv`).
- **Archivos de Salida Proyectados:** Se propone utilizar extensiones binarias como `.huf`, `.bin` o `.hcc`.
- **Estructura del Header Interno:** (A implementar). Para lograr la descompresión sin el programa anfitrión, el archivo generado DEBE contener las frecuencias o el mapeo de Huffman generado originalmente en los primeros N bytes antes de la trama comprimida real.
