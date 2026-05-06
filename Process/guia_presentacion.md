# 🎤 Guía de Presentación: Compresor de Texto Huffman

Esta guía contiene los puntos de conversación y conceptos técnicos clave que debes explicar durante tu presentación. El objetivo es demostrar que el equipo entiende el "por qué" y el "cómo" detrás del código, justificando las decisiones de diseño arquitectónico.

---

## 1. Arquitectura del Proyecto (No es un código "espagueti")
**Lo que debes decir:**
> *"Al iniciar el proyecto, sabíamos que poner todo en un solo archivo `main.c` iba a ser imposible de mantener. Por eso, diseñamos una arquitectura modular estándar de la industria."*

**Puntos a explicar:**
*   **Separación de responsabilidades:** Dividimos el código en módulos específicos. Tenemos `app.c` para interactuar con el usuario, `compresor.c` para la lógica central, y `bitwise.c` para el manejo de muy bajo nivel.
*   **Directorios:** Explicar que la carpeta `src/` tiene el código fuente, `include/` contiene las cabeceras (`.h`) para conectar los módulos, y `data/` aísla los archivos de prueba para no ensuciar el repositorio.

## 2. Estructuras de Datos: El Corazón de Huffman
**Lo que debes decir:**
> *"Para lograr la compresión, implementamos un flujo de estructuras de datos no lineales."*

**Puntos a explicar:**
1.  **Lista Enlazada Ordenada:** Primero leemos el `.txt` y contamos cuántas veces aparece cada letra. Esos datos los metemos en una lista enlazada que siempre se mantiene ordenada de menor a mayor frecuencia.
2.  **Árbol Binario:** Tomamos los dos nodos con menor frecuencia de la lista y los unimos en un "nodo padre". Repetimos este proceso (usando recursividad) hasta que solo queda un nodo: la raíz del Árbol de Huffman.
3.  **Diccionario:** Recorremos ese árbol. Si vamos a la izquierda es un `0`, si vamos a la derecha es un `1`. Así, las letras más comunes (que quedan cerca de la raíz) reciben códigos muy cortos (ej. `01`), y las letras raras reciben códigos largos.

## 3. El Reto Técnico: Manipulación a Nivel de Bits (`.bin`)
**Lo que debes decir:**
> *"El mayor reto técnico que enfrentamos fue que el lenguaje C, por defecto, escribe y lee 'Bytes' enteros (8 bits), pero nuestros códigos de Huffman son secuencias de bits sueltos (como 3 bits o 5 bits)."*

**Puntos a explicar:**
*   **Bitwise Operators:** Tuvimos que usar operadores a nivel de bits (`<<`, `>>`, `|`) para empaquetar los `0` y `1` manualmente dentro de un *buffer* (un espacio temporal) de tipo `unsigned char`. Solo cuando juntamos exactamente 8 bits, los escribimos en el disco duro.
*   **El Archivo `.bin`:** Por eso el resultado es un archivo `.bin`. Ya no es un texto legible, es información empaquetada. 
*   **El Header (La Cabecera):** Menciona que para poder *descomprimir* en el futuro, tuvieron que ingeniárselas para guardar la tabla de frecuencias al mismísimo inicio del archivo `.bin`, de lo contrario, el descompresor no sabría cómo reconstruir el árbol.

## 4. La Infraestructura en la Nube (GitHub Actions)
**Lo que debes decir:**
> *"Queríamos llevar el proyecto un paso más allá y simular un entorno de desarrollo profesional usando Integración Continua (CI/CD)."*

**Puntos a explicar:**
*   **¿Por qué automatizar?:** En la industria, nadie compila ni prueba las cosas a mano todo el tiempo. 
*   **Python como puente:** Explicar que escribieron un script de Python (`script_compresor.py`) porque Python es excelente para manejar archivos y automatizar comandos. Python actúa como un "robot" que manda a llamar a su ejecutable en C (`compresor_app -c`).
*   **El Flujo:** Cuando alguien hace un *Pull Request* en GitHub, un servidor de GitHub (Ubuntu) descarga el código, lo compila usando `gcc`, busca si subiste algún `.txt` nuevo, y automáticamente te genera el `.bin` comprimido.

## 5. Resultados y Límites Teóricos (Para impresionar al maestro)
**Lo que debes decir:**
> *"Hicimos pruebas de estrés con textos grandes y logramos reducciones del 24% al 30% en el peso de los archivos, lo cual es el límite matemático esperado para este algoritmo puro."*

**Puntos a explicar:**
*   Explicar que la entropía del idioma no permite comprimir una letra individual a menos de ~4.5 bits. 
*   Aclarar que algoritmos como `.zip` comprimen el 80% porque combinan Huffman con algoritmos de diccionario como **LZ77** (que comprimen palabras enteras repetidas, no solo letras). Tu compresor llega al máximo teórico posible para un algoritmo exclusivo de compresión de caracteres.
*   **Archivos pequeños:** Mencionar curiosamente que si comprimes un archivo muy pequeño (ej. 200 bytes), el `.bin` terminará pesando MÁS que el original, porque el "Diccionario" de la cabecera ocupa un espacio fijo. Esto demuestra que entiendes perfectamente el comportamiento del software bajo diferentes escenarios.
