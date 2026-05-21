#!/bin/bash

echo "   Compilando "


# Comando de compilación

gcc main.c -o compresor_app

# Verificar si la compilación fue exitosa
if [ $? -eq 0 ]; then
    echo " Compilación correcta "
    echo ""
    echo "Para ejecutar el programa con el menú interactivo, escribe:"
    echo "  ./compresor_app"
    echo ""
    echo "Para ejecutarlo directo por terminal:"
    echo "  ./compresor_app -c archivo.txt "
    echo "  ./compresor_app -d archivo.bin archivo_salida.txt "
else
    echo " Compilación incorrecta "
fi
