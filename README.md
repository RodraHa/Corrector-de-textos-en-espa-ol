# ***Corrector de ortografía (español)***

Permite evaluar las potenciales faltas ortográficas de archivos de texto.

 Manejando para ello como válido:

- Verbos y sus conjugaciones
- Las demás categorias gramaticales (sustantivos, adjetivos, adverbios...) recopiladas en el archivo ```"diccionario.txt"```
- Numeros con o sin notacion decimal:
```1234, 12.345, 24%```
- Los anteriores elementos dentro de caracteres especiales como ```( ), [ ], " "```, entre otros.

Sin embargo, nombres propios de personas o lugares se mostrarán como erróneos.

Posee dos modos:
1. **Corrección en consola**: Muestra los potenciales errores con su contexto en la terminal (palabras entre las que está)
2. **Corrección en un archivo**: Genera un nuevo archivo titulado `correccion.txt`, donde se copia los contenidos del texto indicado y los errores se colocan entre `[...]`

En ambos casos es posible obtener las correcciones de las palabras mal escritas, seleccionando una opción en el menu.


El funcionamiento se describe a continuación:

```
./output/corrector.exe 

                    CORRECTOR
    
    1. Analizar archivo de texto
    2. Acerca de
    3. Salir

    Seleccion: 
```
Con la primera opción sucede lo siguiente
```

    Nombre del archivo: 

// tras escribir el nombre

                    Elija una opcion

    1. Corregir en consola
    2. Generar un archivo nuevo
    3. Salir

// opcion 1

Corrigiendo texto_a_corregir.txt...

    - No hay coincidencias para : ... anterior palabra posterior...
    - No hay coincidencias para : ... anterior palabra posterior...
    ...

    Listo

    Estadisticas: 
        
        - Se han realizado ... comparaciones
        - Se ha tardado ... segundos

    1. Menu principal
    2. Buscar parecidos de palabras incorrectas
    3. Salir

// opcion 2 en correccion en consola

            "a" hacia atras         "d" hacia adelante

    Su palabra: ...

                    Coincidencias: 

    - ...
    - ...
    
// opcion 2 tras escribir el nombre del archivo

    Listo...

    1. Menu principal
    2. Buscar parecidos de palabras incorrectas
    3. Salir

Seleccion: 

// con la opcion 2 se procede del mismo modo que en la corrección en consola
```
Si en el menu principal se elije la segunda opcion
```
    rodrigo.haro@epn.edu.ec     2022
```
Las principales funciones del programa están en ```./src/corrector.c``` , mientras que otras adiciones están en ```./lib/utility.h```
