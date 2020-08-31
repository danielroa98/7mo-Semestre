#include <stdio.h>
#include <stdlib.h>

long int longitudAA(char *);
long int longitudContador(char *);
long int longitudCombinada(char *);

int main(int argc, char const *argv[])
{
    char * cadena;
    int longitud;

    printf("Inserta la longitud máxima: ");
    scanf("%d", &longitud);

    cadena = (char *) malloc(sizeof(char)*longitud);

    printf("Entra una cadena: ");
    scanf(" %[^\n]", cadena);

    //Obtener la longitud de una cadena con un contador
    printf("La longitud con un contador es %ld \n", longitudContador(cadena));

    //Obtener la longitud con AA
    printf("La longitud con aritmética de apuntadores es %ld \n", longitudAA(cadena));

    printf("La longitud combinada es %ld\n", longitudCombinada(cadena));

    free(cadena);

    return 0;
}

long int longitudAA(char * cadena){

    char * p = cadena;
    while (*p++ != '\0'){};

    return(p - cadena - 1);
    
}

long int longitudContador(char * cadena){
    
    int c = 0;
    char * p = cadena;

    while (*p++ != '\0')
        ++c;

    return c;
    
}

long int longitudCombinada(char *cadena){
    int c = 0;
    char *p = cadena;

    while (* (cadena + c) != '\0')
        ++c;

    return c;
}