/*
 *  Actividad Hospital
 *
 *  Daniel Roa
 *  A01021960
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char nombre[30];
    char apellidoP[30];
    char apellidoM[30];
    int edad;
    char noTel[15];
    int cama;
} pacientes;

int main(int argc, char const *argv[])
{

    pacientes * hospital;
    pacientes * sup;

    int beds;
    int menu;

    printf("Cuántos pacientes tiene registrados?");
    scanf("%d", &beds);

    hospital = (pacientes *) malloc(sizeof(pacientes)*beds);

    pacientes * fin = hospital + beds;

    for (sup = hospital; sup < fin; ++sup){
        printf("Inserte los datos del paciente\n");

        printf("Nombre: ");
        scanf("%s", sup->nombre);

        printf("Apellido paterno: ");
        scanf("%s", sup->apellidoP);

        printf("Apellido materno: ");
        scanf("%s", sup->apellidoM);

        printf("Edad: ");
        scanf("%d", &sup->edad);

        printf("Número telefónico: ");
        scanf("%s", sup->noTel);

        printf("Cama en la que se encuentra: ");
        scanf("%d", &sup->cama);
    }
    

    return 0;
}
