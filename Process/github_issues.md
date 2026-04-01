# 📝 GitHub Issues - Distribución de Tareas

A continuación se presentan los Issues recomendados para finalizar el compresor de texto. Las tareas han sido divididas de manera equitativa para un equipo de **3 personas**, agrupándolas por módulos que requieran conocimientos relacionados.

---

## 👤 Miembro 1: Árbol de Huffman y Generación del Diccionario
*Este integrante se enfocará en la manipulación y recorrido de la estructura del árbol, basándose en la lista enlazada ya existente.*

### Issue #1: Construcción del Árbol de Huffman (Fase 2)
**Descripción:** 
A partir de la lista enlazada ordenada implementada en [Frecuencia_Nodos.c](file:///Users/misael30/Desktop/Project-PES/Compresor_de_texto/Code/Frecuencia_Nodos.c), se debe implementar la lógica para construir el árbol.
**Tareas:**
- [ ] Crear la función que extraiga los 2 nodos con menor frecuencia.
- [ ] Crear el "nodo padre" sumando las frecuencias.
- [ ] Reinsertar el nodo padre en la lista manteniendo el orden.
- [ ] Repetir el proceso hasta obtener el nodo raíz.

### Issue #2: Generación del Diccionario de Códigos (Fase 3)
**Descripción:** 
Recorrer el Árbol de Huffman construido en el Issue #1 para generar los nuevos códigos binarios de cada carácter.
**Tareas:**
- [ ] Crear la estructura `TablaCodigos`.
- [ ] Implementar una función que recorra el árbol (recursividad recomendada).
- [ ] Asignar `0` para caminos a la izquierda y `1` a la derecha.
- [ ] Almacenar el código binario generado por carácter en memoria.

---

## 👤 Miembro 2: Manejo de Bits y Cabecera de Archivos (Header)
*Este integrante será responsable del trabajo a bajo nivel (punteros, operadores de bits y preparación de los archivos de salida).*

### Issue #3: Implementación del Módulo I/O de Bits
**Descripción:** 
C carece de una herramienta estándar para leer/escribir `bits` individuales. Se necesita gestionar buffers a nivel de 8 bits (1 byte) usando operadores a nivel de bits.
**Tareas:**
- [ ] Implementar función `escribir_bit()` con un buffer `unsigned char` y contadores de turnos lógicos.
- [ ] Implementar función `leer_bit()` para la descompresión.
- [ ] Crear la funcionalidad de escritura del buffer remanente (flush) al finalizar.

### Issue #4: Serialización para el Header del Archivo Comprimido
**Descripción:** 
Para que el archivo pueda descomprimirse, el árbol o las frecuencias iniciales deben estar contenidas dentro del mismo.
**Tareas:**
- [ ] Definir una estructura binaria o de texto que almacene las frecuencias directamente en la primera sección del archivo de salida.
- [ ] Implementar la función de extracción que construya la misma lista enlazada base a partir de dicho header.

---

## 👤 Miembro 3: Lógica de Compresión y Descompresión (Core)
*Este integrante se encargará del flujo final del programa uniendo el trabajo de los miembros 1 y 2 para lograr el ciclo completo.*

### Issue #5: Lógica de Compresión (Encoder)
**Descripción:** 
Crear el archivo comprimido `.bin` reemplazando los caracteres del archivo de entrada original por los de su nuevo equivalente en el Diccionario.
**Tareas:**
- [ ] Invocar la escritura del Header (creado por el Miembro 2).
- [ ] Recorrer el archivo `.txt` original por segunda vez carácter a carácter.
- [ ] Buscar en la `TablaCodigos` y enviar la secuencia individual al módulo de escritura de `bits`.
- [ ] Cerrar y limpiar canales de datos.

### Issue #6: Lógica de Descompresión (Decoder)
**Descripción:** 
Leer un archivo ya comprimido, reconstruir la base teórica y regresar a los datos originales `.txt`.
**Tareas:**
- [ ] Leer el Header y rearmar la raíz del Árbol de Huffman (apoyado en Miembro 1).
- [ ] Leer el archivo a bits usando las utilidades del Miembro 2 y navegar sobre el Árbol desde la raíz a las hojas por cada `0/1` hallado.
- [ ] Al llegar a la hoja, imprimir el carácter original en el archivo `descomprimido.txt`.
- [ ] Cargar validaciones de alcance final (padding/bits basura).
