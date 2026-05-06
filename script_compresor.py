import sys
import subprocess
import os

def main():
    # Los archivos a comprimir se pasarán como argumentos (ej. python script_compresor.py archivo1.txt)
    files_to_compress = sys.argv[1:]
    
    if not files_to_compress:
        print("No se proporcionaron archivos para comprimir.")
        return

    # Ruta al ejecutable generado por GCC
    executable = os.path.join("Code", "build", "compresor_app")
    
    if not os.path.exists(executable):
        print(f"Error: El ejecutable no se encuentra en {executable}.")
        print("Asegúrate de compilar el proyecto antes de correr este script.")
        sys.exit(1)

    archivos_procesados = 0

    for file_path in files_to_compress:
        # Solo procesamos archivos .txt que no sean archivos recuperados
        if file_path.endswith('.txt') and not file_path.endswith('_recuperado.txt'):
            print(f"\n[Python] Iniciando compresión de: {file_path}")
            try:
                # Invocamos al programa en C pasando el flag -c y el archivo
                result = subprocess.run(
                    [executable, "-c", file_path], 
                    check=True, 
                    capture_output=True, 
                    text=True,
                    errors='replace'
                )
                print(result.stdout)
                print(f"[Python] ✅ Éxito: Se ha generado el archivo binario para {file_path}")
                archivos_procesados += 1
            except subprocess.CalledProcessError as e:
                print(f"[Python] ❌ Error al intentar comprimir {file_path}")
                print(e.stderr)

    if archivos_procesados == 0:
        print("[Python] No se encontró ningún archivo .txt válido para comprimir en la lista proporcionada.")

if __name__ == "__main__":
    main()
