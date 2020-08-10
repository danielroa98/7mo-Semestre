/* 
Primer programa en C del semestre

Daniel Roa
A01021960
*/

#include <stdio.h>

//  No existe una forma para declarar bool
//  Se modelan como una constante o una enumeración (0 o 1)

int main(int argc, char const *argv[])
{
    
    int entero1;
    int entero2;
    int suma;

    printf("Inserte el primer entero\n");
    scanf("%d", &entero1);

    printf("Inserte el segundo entero\n");
    scanf("%d", &entero2);

    suma = entero1 + entero2;

    printf("La suma es %d\n", suma);

    return 0;
}
