# Reporte del Estado Actual y Módulos Faltantes

## 1. Estado Actual del Repositorio
Basado en el análisis de [README.md](file:///Users/misael30/Desktop/Project-PES/Compresor_de_texto/README.md) y [Code/Frecuencia_Nodos.c](file:///Users/misael30/Desktop/Project-PES/Compresor_de_texto/Code/Frecuencia_Nodos.c), el proyecto se encuentra en la **Fase 1** (Tabla de frecuencias).

Actualmente se ha implementado:
- Lectura de un archivo de texto (`archivotest.txt`).
- Conteo de la frecuencia de aparición de cada carácter (función [Archivo_Contar](file:///Users/misael30/Desktop/Project-PES/Compresor_de_texto/Code/Frecuencia_Nodos.c#112-129)).
- Creación de una lista enlazada ordenada ascendentemente por frecuencia mediante la inserción estructurada de nodos ([InsertarNodo_ordenado](file:///Users/misael30/Desktop/Project-PES/Compresor_de_texto/Code/Frecuencia_Nodos.c#47-86)).
- Funcionalidad básica para la liberación de la memoria y la impresión de la lista de caracteres.

El código actual sirve como la base perfecta para el inicio de la Fase 2 contemplada en la documentación teórica del proyecto.

---

## 2. Módulos Faltantes para un Compresor Funcional

Para que el compresor de archivos `.txt` funcione mediante el Algoritmo de Huffman, faltan los siguientes módulos clave:

### A. Módulo de Construcción del Árbol de Huffman (Fase 2)
- **Funcionalidad:** Extraer los dos nodos con menor frecuencia de la lista enlazada, fusionarlos en un nuevo nodo padre que contenga la suma de sus frecuencias, y volver a insertarlo en la lista de forma ordenada.
- **Objetivo:** Repetir el proceso hasta que quede un único nodo en la lista, el cual será la raíz del Árbol de Huffman.

### B. Módulo de Generación del Diccionario de Códigos (Fase 3)
- **Funcionalidad:** Realizar un recorrido (típicamente recursivo) a través del Árbol de Huffman desde la raíz hasta las hojas.
- **Objetivo:** Asignar un `0` cuando el recorrido vaya a la izquierda y un `1` a la derecha, para construir el código binario de longitud variable de cada carácter original (hoja) y guardarlo en una tabla de búsqueda (e.g., usando la estructura `TablaCodigos`).

### C. Módulo de Gestión de E/S a Nivel de Bits (I/O Bits)
- **Funcionalidad:** Dado que en C las funciones estándar de escritura/lectura trabajan a nivel de `byte` (8 bits), es necesario implementar operaciones que permitan acumular y escribir/leer bits individualmente (`0`s y `1`s).
- **Objetivo:** Manejar buffers de 8 bits usando operadores bit a bit (`<<`, `>>`, `|`, `&`) para escribir el archivo comprimido eficientemente y leerlo sin desperdicio de memoria.

### D. Módulo de Compresión (Encoder)
- **Funcionalidad:** Abrir nuevamente el archivo de texto original, leer carácter por carácter, buscar su equivalente binario en el Diccionario de Códigos y escribir esa secuencia de bits en el archivo `.bin` o comprmido de salida.
- **Objetivo:** Además, guardar una representación del Árbol de Huffman o la Tabla de Frecuencias en la "cabecera" (header) del archivo comprimido, lo cual es estrictamente necesario para que el archivo pueda ser descomprimido posteriormente.

### E. Módulo de Descompresión (Decoder)
- **Funcionalidad:** Abrir el archivo comprimido, reconstruir el Árbol de Huffman leyendo la cabecera (header) y luego procesar la secuencia de bits comprimida.
- **Objetivo:** Partiendo de la raíz del árbol, navegar hacia la izquierda (si se lee un `0`) o a la derecha (si se lee un `1`) hasta llegar a una hoja. Al encontrar la hoja, imprimir su carácter en un archivo `.txt` de salida (el archivo descomprimido) y volver a la raíz por cada nuevo carácter.
