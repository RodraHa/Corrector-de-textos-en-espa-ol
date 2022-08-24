#include <stdio.h>
#include <ctype.h>  //tolower
#include <stdlib.h> //malloc 
#include <stdbool.h>
#include <string.h> //strcmp
#include "../lib/utility.h"  //showincword, setColor, definicion de nodo y LENGTH
#include <time.h>
#include <conio.h>  //kbhit, getch

unsigned int hash(char *palabra);       
bool load();                            
bool unload();                          
bool checktext(char *texto_a_corregir, int modo); 
bool analyseword(char palabra[], bool agregar_a_listas);      
bool checkword(char palabra[]);         
void buscarparecidos(char palabra[]);
int mostrarparecidos();

// Tabla hash
node *tabla[2000];

// Lista enlazada (palabras mal escritas)
node *list = NULL;

// Lista enlazada (palabras escritas correctamente)
node *listcorrect = NULL;

long comparaciones = 0;

int main()
{
    int eleccion;
    double time_spent;
    char nombrearchivo[70];

    if(!load())
    {
        return 0;
    }
    system("cls");
    
    setColor(0,15);
    eleccion = presentarMenu();

    do
    {
        if (eleccion == 0)
        {
            setColor(0,15);
            eleccion = presentarMenu();
        }
        else if (eleccion == 1)
        {
            int seleccion = 0;
            printf("\n\tNombre del archivo: ");

            setColor(0,9);
            fflush(stdin);
            fgets(nombrearchivo, 70, stdin);
            printf("\n");

            system("cls");
            
            do
            {
                setColor(11,0);
                printf("\n\n\t\t\t\tElija una opcion\n\n");
                setColor(14,0);
                printf("\t1.");
                setColor(0,15);
                printf(" Corregir en consola\n\t");
                setColor(9,0);
                printf("2.");
                setColor(0,15);
                printf(" Generar un archivo nuevo\n\t");
                setColor(4,0);
                printf("3.");
                setColor(0,15);
                printf(" Salir\n\n\t");
                printf("Seleccion: ");
                setColor(0,9);
                fflush(stdin);
                scanf("%d", &seleccion);
                system("cls");
            } while (seleccion <= 0 || seleccion > 3);

            setColor(0,15);

            if (seleccion == 3)
            {
                system("cls");
                eleccion = 3;
                continue;
            }
            else if (seleccion == 1)
            {
                clock_t begin = clock();
                comparaciones = 0;

                bool estado = checktext(nombrearchivo, 1);
                if (!estado)
                {   
                    setColor(0,15);
                    eleccion == 1;
                    continue;
                }
                clock_t end = clock();

                setColor(0, 10);
                printf("\n\tListo\n");
                setColor(0, 13);
                printf("\n\tEstadisticas:\n\n");
                printf("\t- Se han realizado %li comparaciones\n", comparaciones);

                time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
                printf("\t- Se ha tardado %f segundos\n", time_spent);

                eliminarlista(&listcorrect);

                setColor(0,15);
                printf("\n\t1. Menu principal\n\t2. Buscar parecidos de palabras incorrectas\n\t3. Salir\n\n");
                seleccion = getoptionMenu(3);

                if (seleccion == 3)
                {
                    system("cls");
                    eleccion = 3;
                    eliminarlista(&list);
                    continue;
                }
                else if (seleccion == 1)
                {
                    system("cls");
                    eleccion = 0;
                    eliminarlista(&list);
                    continue;
                }
                else if (seleccion == 2)
                {
                    int estado = mostrarparecidos();
                    if (estado == 0)
                    {
                        system("cls");
                        printf("No existen errores\n");
                        eleccion = 0;
                        continue;
                    }
                    eliminarlista(&list);
                    system("cls");
                    eleccion = presentarMenu();
                    continue;
                }
            }
            else
            {
                clock_t begin = clock();
                bool estado = checktext(nombrearchivo, 2);
                if (!estado)
                {   
                    setColor(0,15);
                    eleccion == 1;
                    continue;
                }

                eliminarlista(&listcorrect);

                setColor(0,10);
                printf("\n\tListo...\n");
                setColor(0,15);
                printf("\n\t1. Menu principal\n\t2. Buscar parecidos de palabras incorrectas\n\t3. Salir\n\n");
                seleccion = getoptionMenu(3);
                if (seleccion == 3)
                {
                    system("cls");
                    eleccion = 3;
                    eliminarlista(&list);
                    continue;
                }
                else if (seleccion == 1)
                {
                    eleccion = 0;
                    eliminarlista(&list);
                    continue;
                }
                else if (seleccion == 2)
                {
                    int estado = mostrarparecidos();
                    if (estado == 0)
                    {
                        system("cls");
                        printf("No existen errores\n");
                        eleccion = 0;
                        continue;
                    }
                }
                eliminarlista(&list);
                system("cls");
                eleccion = presentarMenu();
                continue;
            }
        }
        else if (eleccion == 2)
        {
            printf("\n\n\t\t\t\trodrigo.haro@epn.edu.ec - 2022\n");
            eleccion = presentarMenu();
        }
    }
    while (eleccion != 3);
    unload();
    return 0;
}

// Asigna un identificador (entero positivo 0-1999) a "palabra"
unsigned int hash(char *palabra)
{
    int result = 0;
    for (int i = 0; palabra[i] != '\0'; i++)
    {
        if (i % 2 == 0)
            result += 2 * ((int) palabra[i]) + 10;
        else 
            result += 3 * ((int) palabra[i]) + 5;
    }
    result = result / 2;
    if (result < 0)
        result = 0;
    if (result > 1999)
        result = 1999;
    return result;
}

// Carga el diccionario (diccionario.txt) en la tabla hash
bool load()
{
    char temporal[LENGTH];
    int hashresult;
    
    FILE *dictionary = fopen("./src/diccionario.txt", "r");

    if (dictionary == NULL)
    {
        dictionary = fopen("../src/diccionario.txt", "r");
        if (dictionary == NULL)
        {
            return false;
        }
    }

    do
    {   
        node *tmpnode = malloc(sizeof(node));
        
        fscanf(dictionary, "%s", temporal);
        strcpy(tmpnode->word, temporal);
        tmpnode->next = NULL;
        hashresult = hash(temporal);
        
        //Verificar si el espacio único esta vacio
        if (tabla[hashresult] != NULL)
        {  
            tmpnode->next = tabla[hashresult];
            tabla[hashresult] = tmpnode;
        }
        else 
        {
            tabla[hashresult] = tmpnode;
        }
    }
    while (fgetc(dictionary) != EOF);
    fclose(dictionary);
    return true;
}

// Libera el espacio ocupado por los nodos de la tabla hash creados con malloc 
bool unload()
{
    for (int i = 0; i < 2000; i++)
    {
        node *cursor = tabla[i];
        node *tmp;
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}

/*
    Abre el archivo de texto, lee cada string contenida en el y la pasa a la funcion analyseword
    @param texto_acorregir Direccion del archivo de texto a evaluar
    @param modo Define si la salida se muestra en consola o se guarda en un archivo 
*/
bool checktext(char *texto_a_corregir, int modo)
{
    for (int i = 0; texto_a_corregir[i] != '\0'; i++)
    {
        if (texto_a_corregir[i] == '\n')
        {
            texto_a_corregir[i] = '\0';
            break;
        }
    }
    
    quitarespacios(texto_a_corregir);

    FILE *texto = fopen(texto_a_corregir, "r");
    
    if (texto == NULL)
    {
        char nombreArchivo[75];
        strcpy(nombreArchivo, "../");
        strcat(nombreArchivo, texto_a_corregir);
        texto = fopen(nombreArchivo, "r");
        if (texto == NULL)
        {
            system("cls");
            setColor(0, 15);
            printf("\n\nNo se encontro el texto a corregir: ");
            setColor(0, 10);
            printf("\"%s\"\n", texto_a_corregir);
            return false;
        }
    }
    if (modo == 1)
    {
        char anterior[LENGTH];
        char posterior[LENGTH];
        char palabra[LENGTH];
        char prueba[LENGTH];
        int estado = 0;
        int hashresult;
        bool excepcion;
        
        setColor(0, 10);
        printf("\n\nCorrigiendo %s...\n\n", texto_a_corregir);

        while(fgetc(texto) != EOF)
        {
            fseek(texto, -1, SEEK_CUR);
            if (estado == 0)
            {
                fscanf(texto, "%34s", palabra);
                strcpy(anterior, palabra);
                if (fgetc(texto) != EOF)
                {
                    fseek(texto, -1, SEEK_CUR);
                    fscanf(texto, "%34s", posterior);
                    if(!analyseword(palabra, true))
                        showincword(palabra, anterior, posterior, false);
                    estado = 1;
                    continue;
                }
                else if(!analyseword(palabra, true))
                {
                    showincword(palabra, anterior, posterior, false);
                    continue;
                }
            }
            if (estado == 1)
            {
                strcpy(palabra, posterior);
                fscanf(texto, "%34s", posterior);
                if(!analyseword(palabra, true))
                    showincword(palabra, anterior, posterior, true);
                estado = 2;
                continue;
            }
            else if (estado == 2)
            {
                strcpy(anterior, palabra);
                strcpy(palabra, posterior);
                fscanf(texto, "%34s", posterior);
                if(!analyseword(palabra, true))
                    showincword(palabra, anterior, posterior, true);
            }
        }
        if (estado != 0)
        {
            strcpy(anterior, palabra);
            strcpy(palabra, posterior);
            if(!analyseword(palabra, true))
                showincword(palabra, anterior, posterior, false);
            fclose(texto);
        }
        return true;
    }
    else if (modo == 2)
    {
        char palabra[LENGTH];
        FILE *textocorregido = fopen("correccion.txt", "w");
        while (fgetc(texto) != EOF)
        {
            fseek(texto, -1, SEEK_CUR);
            switch (fgetc(texto))
            {
            case ' ':
                fputc(' ', textocorregido);
                break;
            case '\n':
                fputc('\n', textocorregido);
                break;
            default:
                break;
            }
            fseek(texto, -1, SEEK_CUR);

            fscanf(texto, "%34s", palabra);
            if (!analyseword(palabra, true))
            {
                fputc('[', textocorregido);
                fputs(palabra, textocorregido);
                fputc(']', textocorregido);
            }
            else
            {
                fputs(palabra, textocorregido);
            }
        }
        fclose(texto);
        fclose(textocorregido);
        return true;
    }
    fclose(texto);
    return true;
}

/*
    Devuelve "true" si existe una variacion de la string en el diccionario usando la funcion checkword (plural, conjugacion, etc.).
    Si no existe devuelve "false"
    @param palabra string a evaluar
    @param agregar_a_listas Define si la palabra se guarda en las listas enlazadas
*/
bool analyseword(char palabra[], bool agregar_a_listas)
{
    char potencialplural[LENGTH];
    char potencialplural2[LENGTH];
    char potencialplural3[LENGTH];
    char potencialconju[LENGTH];
    char conjugacion_lo[LENGTH];
    int caseplural = 0;
    int correction = 0;
    int conjugacion = 0;
    int conjugacion_lo_status = 0;

    /*\
    convierte la palabra a minuscula, elimina caracteres especiales al leer la string
    identifica el caso potencial de que se trate del plural de una palabra, de un numero
    o de una conjugacion
    */

    for (int i = 0, repeticion = 0; palabra[i] != '\0'; i++)
    {
        if (palabra[i] == '.' || palabra[i] == ',' || palabra[i] == ':' || palabra[i] == ';'|| palabra[i] == '-'
        || palabra[i] == '?' || palabra[i] == '!'|| palabra[i] == ')' || palabra[i] == '\"'|| palabra[i] == '(')
        {
            if (palabra[i+1] != '\0')
                continue;
        }
        
        if (palabra[i+1] == '\0')
        {
            if (palabra[i] == '.' || palabra[i] == ',' || palabra[i] == ':' || palabra[i] == ';'|| palabra[i] == '-'
            || palabra[i] == '?' || palabra[i] == '!'|| palabra[i] == ')' || palabra[i] == '\"'|| palabra[i] == '(')
            {
                palabra[repeticion] = '\0';   
            }
            else
            {
                palabra[repeticion] = (char) tolower(palabra[i]);
                palabra[repeticion + 1] = '\0';
                correction = 1;
            }

            // Vertifica si se trata de un numero
            if (isdigit(palabra[0]) == 1)
            {
                for (int c = 1; palabra[c] != '\0'; c++)
                {
                    if (isdigit(palabra[c]) == 0 && palabra[c] != '%')
                    {
                        return false;
                    }
                }
                return true;
            }

            // busca en la lista de palabras incorrectas
            for (node *tmp = list; tmp != NULL; tmp = tmp->next)
            {
                if (strcmp(palabra, tmp->word) == 0)
                {
                    return false;
                }
            }

            // Verifica si se trata de un verbo de la forma "quererlo"
            if (palabra[repeticion-1+correction] == 'o' && palabra[repeticion-2+correction] == 'l')
            {
                for (int a=0; a<repeticion-2+correction;a++)
                {
                    conjugacion_lo[a] = palabra[a];
                    if (palabra[a+1] == 'l' && palabra[a+2] == 'o')
                        conjugacion_lo[a+1] = '\0';
                }
                conjugacion_lo_status = 1;
                break;
            }

            // Veritifica si se trata de un verbo de la forma "esforzarse"
            if (palabra[repeticion-1+correction] == 'e' && palabra[repeticion-2+correction] == 's')
            {
                for (int a=0; a<repeticion-2+correction;a++)
                {
                    potencialconju[a] = palabra[a];
                    if (palabra[a+1] == 's' && palabra[a+2] == 'e')
                        potencialconju[a+1] = '\0';
                }
                conjugacion = 1;
                break;
            }

            // Verifica si se trata del plural de una palabra terminado en "s"
            if (palabra[repeticion-1+correction] == 's')
            {
                for (int a=0; a<repeticion-1+correction;a++)
                {
                    potencialplural[a] = palabra[a];
                    if (palabra[a+1] == 's')
                        potencialplural[a+1] = '\0';
                }
                caseplural=1;
            }

            // Verifica si se trata del plural de una palabra terminada en "es"
            if (palabra[repeticion-1+correction] == 's' && palabra[repeticion-2+correction] == 'e')
            {
                for (int a=0; a<repeticion-2+correction;a++)
                {
                    potencialplural2[a] = palabra[a];
                    if (palabra[a+1] == 'e' && palabra[a+2] == 's')
                        potencialplural2[a+1] = '\0';
                }
                caseplural=2;
            }
            // Verifica si se trata del plural de una palabra terminada en "nes"
            if (palabra[repeticion-1+correction] == 's' && palabra[repeticion-2+correction] == 'e'
            && palabra[repeticion-3+correction] == 'n')
            {
                for (int a=0; a<repeticion-4+correction;a++)
                {
                    potencialplural3[a] = palabra[a];
                    if (palabra[a+1] == 'o' && palabra[a+2] == 'n' && palabra[a+3] == 'e')
                        potencialplural3[a+1] = (char) -61;
                        potencialplural3[a+2] = (char) -77;
                        potencialplural3[a+3] = 'n';
                        potencialplural3[a+4] = '\0';
                }
                caseplural=3;
            }
            break;
        }
        palabra[repeticion] = (char) tolower(palabra[i]);
        repeticion++;
    }
    if (conjugacion_lo_status != 0)
    {
        if (!checkword(palabra) && !checkword(conjugacion_lo))
        {
            if (agregar_a_listas)
                agregarelementoLista(&list, palabra);
            return false;
        }
    }
    else if (conjugacion != 0)
    {
        if (!checkword(palabra) && !checkword(potencialconju))
        {
            if (agregar_a_listas)
                agregarelementoLista(&list, palabra);
            return false;
        }
    }
    else if (caseplural == 0)
    {
        if (!checkword(palabra))
        {
            if (agregar_a_listas)
                agregarelementoLista(&list, palabra);
            return false;
        }
    }
    else if (caseplural == 1)
    {
        if (!checkword(palabra) && !checkword(potencialplural))
        {
            if (agregar_a_listas)
                agregarelementoLista(&list, palabra);
            return false;
        }
    }
    else if (caseplural == 2)
    {
        if (!checkword(palabra) && !checkword(potencialplural) && !checkword(potencialplural2))
        {
            if (agregar_a_listas)
                agregarelementoLista(&list, palabra);
            return false;
        }
    }
    else 
    {
        if (!checkword(palabra) && !checkword(potencialplural) && !checkword(potencialplural2)
        && !checkword(potencialplural3))
        {
            if (agregar_a_listas)
                agregarelementoLista(&list, palabra);
            return false;
        }
    }
    if (!agregar_a_listas)
        agregarelementoLista(&listcorrect, palabra);
    return true;
}

/*
    Busca la string en la tabla hash, si la encuentra devuelve "true", de lo contrario "false"
    @param palabra String a evaluar
*/
bool checkword(char palabra[])
{
    int hashresult = hash(palabra);
    for (node *search = tabla[hashresult]; search != NULL; search = search->next)
    {
        comparaciones++;
        if (strcmp(palabra, search->word) == 0)
        {
            return true;
        }
    }
    return false;
}

/*
    Modifica los caracteres de la string y verifica si son variaciones de palabras existentes, evaluandolas con la funcion
    analyseword. Si lo son las guarda en la lista enlazada de palabras correctas y las muestra en consola
    @param palabra String a evaluar
*/
void buscarparecidos(char palabra[])
{    
    char iterador[LENGTH];  // comparacion
    char original[LENGTH];  // palabra original

    strcpy(original, palabra);
    strcpy(iterador, palabra);
    
    for (int i = 0; original[i] != '\0'; i++)
    {
        if (esvocal(original[i]))
        {
            strcpy(iterador, original);
            iterador[i] = 'a';
            analyseword(iterador, false);
            iterador[i] = 'e';
            analyseword(iterador, false);
            iterador[i] = 'i';
            analyseword(iterador, false);
            iterador[i] = 'o';
            analyseword(iterador, false);
            iterador[i] = 'u';
            analyseword(iterador, false);
            ponertilde(iterador, original, 1, i);
            analyseword(iterador, false);
            ponertilde(iterador, original, 2, i);
            analyseword(iterador, false);
            ponertilde(iterador, original, 3, i);
            analyseword(iterador, false);
            ponertilde(iterador, original, 4, i);
            analyseword(iterador, false);
            ponertilde(iterador, original, 5, i);
            analyseword(iterador, false);
            ponertilde(iterador, original, 6, i);
            analyseword(iterador, false);
        }
        else if ((int)original[i] < 0)
        {
            ponertilde(iterador, original, 1, i);
            analyseword(iterador, false);
            ponertilde(iterador, original, 2, i);
            analyseword(iterador, false);
            ponertilde(iterador, original, 3, i);
            analyseword(iterador, false);
            ponertilde(iterador, original, 4, i);
            analyseword(iterador, false);
            ponertilde(iterador, original, 5, i);
            analyseword(iterador, false);
            ponertilde(iterador, original, 6, i);
            analyseword(iterador, false);
            quitartilde(iterador, original, 1, i);
            analyseword(iterador, false);
            quitartilde(iterador, original, 2, i);
            analyseword(iterador, false);
            quitartilde(iterador, original, 3, i);
            analyseword(iterador, false);
            quitartilde(iterador, original, 4, i);
            analyseword(iterador, false);
            quitartilde(iterador, original, 5, i);
            analyseword(iterador, false);
            i++;
        }
        else if (original[i] == 'c' || original[i] == 's')
        {
            strcpy(iterador, original);
            iterador[i] = 'c';
            analyseword(iterador, false);
            iterador[i] = 's';
            analyseword(iterador, false);
        }
        else if (original[i] == 'b' || original[i] == 'v')
        {
            strcpy(iterador, original);
            iterador[i] = 'b';
            analyseword(iterador, false);
            iterador[i] = 'v';
            analyseword(iterador, false);
        }
        else if (original[i] == 'j' || original[i] == 'g')
        {
            strcpy(iterador, original);
            iterador[i] = 'j';
            analyseword(iterador, false);
            iterador[i] = 'g';
            analyseword(iterador, false);
        }
    }
    node *tmp = listcorrect;
    if (tmp == NULL)
    {
        printf("\tNo se encontraron coincidencias\n");
    }
    while (tmp != NULL)
    {
        printf("\t. ");
        imprimir_espanol(tmp->word);
        printf("\n");
        tmp = tmp->next;
    }
}

/*
    Muestra una interfaz en consola y permite moverse a traves de la lista enlazada de palabras incorrectas.
    En dicha posicion llama a la funcion buscarparecidos y muestra los resultados en consola
*/
int mostrarparecidos()
{
    int longitud_list = 0;
    int posicion = 0;
    int retorno;
    char busqueda[LENGTH];
    node *temporal = NULL;

    for (node *tmp = list; tmp != NULL; tmp = tmp->next)
    {
        longitud_list++;
    }

    if (longitud_list == 0)
    {
        eliminarlista(&list);
        return 0;
    }

    system("cls");
    setColor(14,0);
    printf("\n\n\t\t\"a\" hacia atras\t\t\"d\" hacia adelante\n\n");
    setColor(0,15);

    while (true)
    {
        if (kbhit())
        {
            system("cls");
            setColor(14,0);
            printf("\n\n\t\t\"a\" hacia atras\t\t\"d\" hacia adelante\n\n");
            setColor(0,15);
            
            char ch = getch();
            if ((int) ch == 100)
            {
                if (posicion < longitud_list - 1)
                    posicion++;
                setColor(0,15);
                printf("\tSu palabra: ");
                setColor(0,9);
                retorno = mostrarelementoLista(list, posicion, longitud_list, busqueda);
                if (retorno == 1)
                {
                    setColor(10,0);
                    printf("\n\n\t\t\tCoincidencias:\n\n");
                    setColor(0,15);
                    buscarparecidos(busqueda);
                    eliminarlista(&listcorrect);
                }
                continue;
            }
            else if ((int) ch == 97)
            {
                if (posicion > 0)
                    posicion--;
                setColor(0,15);
                printf("\tSu palabra: ");
                setColor(0,9);
                retorno = mostrarelementoLista(list, posicion, longitud_list, busqueda);
                if (retorno == 1)
                {
                    setColor(10,0);
                    printf("\n\n\t\t\tCoincidencias:\n\n");
                    setColor(0,15);
                    buscarparecidos(busqueda);
                    eliminarlista(&listcorrect);
                }
                continue;
            }
            eliminarlista(&listcorrect);
            break;
        }
    }
    return 1;
}

/*
    gcc ./src/corrector.c -o ./output/corrector.exe
    ./output/corrector.exe
*/