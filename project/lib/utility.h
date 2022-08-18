#include <stdio.h>
#include <unistd.h>
#include <Windows.h>
#include <stdbool.h>

// Longitud maxima de una palabra
#define LENGTH 35

// Definicion de nodo
typedef struct node
{
    char word[LENGTH];
    struct node *next;
}node;

// Procedimiento para cambiar el color del fondo y/o pantalla
void setColor(int Background, int Text){ 
    int colorTexto= Text + (Background * 16);        
    HANDLE terminal = GetStdHandle(STD_OUTPUT_HANDLE);  
    SetConsoleTextAttribute(terminal, colorTexto); 
}

// Muestra palabras con tildes y ñ
void imprimir_espanol(char palabra[])
{
    for (int i = 0; palabra[i] != '\0'; i++)
    {
        if (((int) palabra[i]) > 0)
        {
            printf("%c", palabra[i]);
            continue;
        }
        else if (((int) palabra[i]) == -61)
        {
            if (((int) palabra[i+1]) == -95)
            {
                printf("\xA0");
                continue;
            }
            if (((int) palabra[i+1]) == -87)
            {
                printf("\x82");
                continue;
            }
            if (((int) palabra[i+1]) == -83)
            {
                printf("\xA1");
                continue;
            }
            if (((int) palabra[i+1]) == -77)
            {
                printf("\xA2");
                continue;
            }
            if (((int) palabra[i+1]) == -70)
            {
                printf("\xA3");
                continue;
            }
        }
        else if (((int) palabra[i]) == -79)
        {
            printf("\xA4");
            continue;
        }
    }
}
/*
    á   :   -61 -95
    é   :   -61 -87
    í   :   -61 -83
    ó   :   -61 -77
    ú   :   -61 -70
    ñ   :   -61 -79
*/

/*
    Coloca una tilde (caracter multiple) en la posicion suministrada
    @param nuevapalabra String donde se guarda el resultado
    @param palabra  String a evaluar
    @param opcion Define la vocal acentuada a colocar
    @param posicion Posicion de la string para poner la tilde
*/
void ponertilde(char *nuevapalabra, char *palabra, int opcion, int posicion)
{
    for (int i = 0, b = 0; palabra[i] != '\0'; i++, b++)
    {
        if (i == posicion)
        {
            switch (opcion)
            {
            case 1:
                nuevapalabra[b] = -61;
                nuevapalabra[b+1] = -95;
                b++;
                break;
            case 2:
                nuevapalabra[b] = -61;
                nuevapalabra[b+1] = -87;
                b++;
                break;
            case 3:
                nuevapalabra[b] = -61;
                nuevapalabra[b+1] = -83;
                b++;
                break;
            case 4:
                nuevapalabra[b] = -61;
                nuevapalabra[b+1] = -77;
                b++;
                break;
            case 5:
                nuevapalabra[b] = -61;
                nuevapalabra[b+1] = -70;
                b++;
                break;
            case 6:
                nuevapalabra[b] = -61;
                nuevapalabra[b+1] = -79;
                b++;
                break;
            }
            if ((int)palabra[i] == -61)
            {
                i++;
            }
        }
        else
        {
            nuevapalabra[b] = palabra[i];
        }
        if (palabra[i+1] == '\0')
        {
            nuevapalabra[b+1] = '\0';
        }
    }
}

/*
    Operacion inversa al procedimiento poner tilde
*/
void quitartilde(char *nuevapalabra, char *palabra, int opcion, int posicion)
{
    for (int i = 0, b = 0; palabra[i] != '\0'; i++, b++)
    {
        if (i == posicion)
        {
            switch (opcion)
            {
            case 1:
                nuevapalabra[b] = 'a';
                i++;
                break;
            case 2:
                nuevapalabra[b] = 'e';
                i++;
                break;
            case 3:
                nuevapalabra[b] = 'i';
                i++;
                break;
            case 4:
                nuevapalabra[b] = 'o';
                i++;
                break;
            case 5:
                nuevapalabra[b] = 'u';
                i++;
                break;
            default:
                break;
            }
        }
        else
        {
            nuevapalabra[b] = palabra[i];
        }
        if (palabra[i+1] == '\0')
        {
            nuevapalabra[b+1] = '\0';
        }
    }
}

/* 
    Muestra la palabra no encontrada con su contexto
    @param repetir Define si mostrar las palabras del contexto aunque sean iguales a la palabra incorrecta
*/
void showincword(char palabra[],  char anterior[], char posterior[], bool repetir)
{
    setColor(0,15);
    printf("\t- No hay coincidencias para\t:\t");
    if (repetir == true)
    {
        setColor(0,9);
        printf("... ");
        imprimir_espanol(anterior);
        printf(" ");
        setColor(0,12);
        imprimir_espanol(palabra);
        printf(" ");
        setColor(0,9);
        imprimir_espanol(posterior);
        printf(" ...");
    }
    else
    {
        if (strcasecmp(anterior, palabra) != 0)
        {
            setColor(0,9);
            printf("... ");
            imprimir_espanol(anterior);
            printf(" ");
            setColor(0,12);
            imprimir_espanol(palabra);
        }
        else
        {
            setColor(0,12);
            imprimir_espanol(palabra);
            printf(" ");
            setColor(0,9);
            imprimir_espanol(posterior);
            printf(" ...");
        }
    }
    printf("\n");
}

// Menu principal
int presentarMenu()
{
    int seleccion;
    do
    {
        printf("\n\n\t\t\t\tCORRECTOR\n\n");
        setColor(14,0);
        printf("\t1.");
        setColor(0,15);
        printf(" Analizar archivo de texto\n\t");
        setColor(9,0);
        printf("2.");
        setColor(0,15);
        printf(" Acerca de\n\t");
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
    return seleccion;
}

// Permite obtener un numero entero entre 0 y "maximo"
int getoptionMenu(int maximo) 
{
    int seleccion;
    do
    {
        printf("Seleccion: ");
        setColor(0,9);
        fflush(stdin);
        scanf("%d", &seleccion);
    }
    while (seleccion < 0 || seleccion > maximo);
    setColor(0,15);
    return seleccion;
}

// Agrega una string a una lista enlazada
int agregarelementoLista(node **lista, char palabra[])
{
    node *n = malloc(sizeof(node));
    if (n == NULL)
        return 1;
    
    strcpy(n->word, palabra);
    n->next = *lista;
    *lista = n;
    return 0;
}

// Libera el espacio ocupado por los elementos de una lista enlazada
void eliminarlista(node **lista)
{
    while (*lista != NULL)
    {
        node *tmp = (*lista)->next;
        free(*lista);
        *lista = tmp;
    }
}

// Muestra el elemento de una lista enlazada, considerando al primer elemento como el ultimo de la lista
int mostrarelementoLista(node *lista, int posicion, int longitud_lista, char nuevo[])
{
    if (posicion > longitud_lista - 1 || longitud_lista == 0 ||  posicion < 0)
    {
        return 0;
    }
    else
    {
        node *temporal = lista;
        for (int a = 0; a < longitud_lista - posicion - 1; a++)
        {
            temporal = temporal->next;
        }
        imprimir_espanol(temporal->word);
        strcpy(nuevo, temporal->word);
    }
    return 1;
}

// Verifica si un caracter es una vocal
bool esvocal(char caracter)
{
    if (caracter == 'a' || caracter == 'e' || caracter == 'i' || caracter == 'o' || caracter == 'u')
        return true;
    return false;
}
