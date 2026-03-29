# 🔄 Flujo de Trabajo: GitHub Issues + Projects

Esta guía explica cómo organizar el equipo utilizando **GitHub Issues** para el seguimiento de tareas y **GitHub Projects (Kanban)** para visualizar el progreso del Sprint.

---

## 🏗️ 1. Preparación del Entorno
1. **Crear el Proyecto (Kanban):**
   - Ve a la pestaña **Projects** en tu repositorio de GitHub y crea un nuevo proyecto estilo "Board" (Tablero Kanban).
   - Crea tres columnas básicas: **To Do** (Por hacer), **In Progress** (En progreso), **In Review** (En revisión), y **Done** (Terminado).
2. **Crear los Issues:**
   - Ve a la pestaña **Issues**, haz clic en "New Issue" y copia la información de las tareas que definimos anteriormente (Issues #1 al #6).
   - Asigna a la persona encargada en el lado derecho (`Assignees`).
   - Agrega el Issue recién creado al proyecto Kanban que creaste en el paso anterior (`Projects`).

---

## 🚀 2. El Flujo de Trabajo (Día a Día)

Cada vez que un miembro del equipo vaya a trabajar en una tarea, debe seguir este proceso:

### Paso 1: Tomar la Tarea
- Entra al tablero de **GitHub Projects** y arrastra la tarjeta de tu Issue desde **To Do** hacia **In Progress**. Esto le avisa a todo el equipo en qué estás trabajando.

### Paso 2: Crear una Rama de Trabajo (Branch)
- En tu terminal, asegúrate de estar en la rama principal y tener la última versión:
  ```bash
  git checkout main
  git pull origin main
  ```
- Crea una rama nueva para tu issue. Es una buena práctica nombrarla con el prefijo `feature/` y el número de issue:
  ```bash
  git checkout -b feature/issue-1-arbol
  ```

### Paso 3: Codificar y Guardar (Commits)
- Escribe tu código en C, realiza pruebas locales.
- Guarda tus cambios con mensajes descriptivos:
  ```bash
  git add .
  git commit -m "Implementa funcion para fusionar nodos de frecuencia"
  ```
- Sube tu rama a GitHub:
  ```bash
  git push -u origin feature/issue-1-arbol
  ```

### Paso 4: Pull Request (PR) y Code Review
- Entra a GitHub. Verás un botón verde solicitando crear un **"Compare & pull request"**.
- Al crear el PR, en la descripción escribe la palabra clave **`Closes #1`** (reemplaza '1' con tu número de Issue). Esto le dice a GitHub que cierre tu Issue automáticamente cuando el código sea aceptado.
- Pídele a otro miembro del equipo que revise tu código y apruebe los cambios (Approve).
- Nunca fusiones (Merge) tu propio código sin usar un PR.

### Paso 5: Merge y Limpieza
- Una vez aprobado, haz clic en **"Merge pull request"**.
- El código se integrará a la rama [main](file:///Users/misael30/Desktop/Project-PES/Compresor_de_texto/Code/Frecuencia_Nodos.c#21-44).
- La magia de GitHub: tu Issue se cerrará automáticamente y, en tu tablero de Projects, la tarjeta se moverá sola a la columna **Done**.
- Elimina la rama localmente y actualiza tu [main](file:///Users/misael30/Desktop/Project-PES/Compresor_de_texto/Code/Frecuencia_Nodos.c#21-44):
  ```bash
  git checkout main
  git pull origin main
  git branch -d feature/issue-1-arbol
  ```

---

## 🎯 Reglas de Oro del Equipo
* **Nadie trabaja directamente en [main](file:///Users/misael30/Desktop/Project-PES/Compresor_de_texto/Code/Frecuencia_Nodos.c#21-44):** Todo desarrollo ocurre en ramas independientes.
* **Commits Pequeños:** Sube cambios lógicos pequeños, no esperes a terminar todo el módulo para hacer un commit.
* **Comunicación:** Si te atoras con el manejo de memoria u operaciones binarias, avisa al equipo en los comentarios del Issue o del PR.
