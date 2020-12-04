//
//  main.c
//  invertir_cadena
//
//  Created by Vicente Cubells Nonell on 17/08/15.
//  Copyright (c) 2015 Vicente Cubells Nonell. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void invertir(char *);

int main(int argc, char **argv) {
    char * cadena;
    int longitud;
    
    printf("Dame la longitud máxima: ");
    scanf("%d", &longitud);
    
    cadena = (char *) malloc(sizeof(char) * longitud);
    
    printf("Entre una cadena: ");
    scanf(" %[^\n]", cadena);

    //fgets(cadena, longitud, stdin);
    
    
    // Invertir una cadena
    invertir(cadena);
    
    printf("Cadena invertida = %s \n", cadena);
    
    free(cadena);
    
    return 0;
}

void invertir(char * cadena)
{
    char l, *p, *d;
    long int c = strlen(cadena);
    p = cadena;
    d = p + (c - 1);
    while (p < d)
    {
        l = *d;
        *d = *p;
        *p = l;
        ++p;
        --d;
    }
    
}
