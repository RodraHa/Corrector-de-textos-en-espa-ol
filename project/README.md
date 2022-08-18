# ***Corrector de ortografía (español)***

Tomando como argumento un archivo de texto el programa muestra en la terminal las potenciales faltas ortográficas que se presentan en el mismo. Manejando para ello como válido:

- Verbos y sus conjugaciones
- Las demás categorias gramaticales (sustantivos, adjetivos, adverbios...) recopiladas en el archivo ```"diccionario.txt"```
- Numeros con o sin notacion decimal:
```1234, 12.345, 24%```
- Los anteriores elementos dentro de caracteres especiales como ```( ), [ ], " "```, entre otros.

Sin embargo, nombres propios de personas o lugares se mostrarán como erróneos.

El funcionamiento se describe a continuación:

```
./output/corrector.exe <texto_a_corregir.txt>

Corrigiendo texto_a_corregir.txt...

- Es incorrecto : ...
- Es incorrecto : ...
...
Listo

Estadisticas: 
    - Se han realizado ... comparaciones
    - Se ha tardado ... segundos
```
Las principales funciones del programa están en ```./src/corrector.c``` , mientras que otras adiciones están en ```./lib/utility.h```

### **Potenciales mejoras**
- Disminuir el numero de comparaciones que se realizan
    - Mejorar la distribucion de la funcion hash
    - Incrementar el numero de entradas en la tabla
- Añadir contexto a los errores (entre qué palabras esta el error)
- Guardar la salida del programa en un archivo de texto, donde se resalte los errores
- Extender el manejo de terminos especiales
- Mostrar el porcentaje de error de acuerdo al número de palabras del texto