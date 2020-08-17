/*
 *  Actividad Hospital
 *
 *  Daniel Roa
 *  A01021960
 */
#include <stdio.h>
#include <string.h>

int beds = 2;

typedef struct{
    char nombre[30];
    char apellidoP[30];
    char apellidoM[30];
    int edad;
    char noTel[12];
    int cama;
} pacientes;

int main(int argc, char const *argv[])
{


    pacientes paciente[beds];

    int menu;

    for (int i = 0; i < beds; i++)
    {
        printf("Inserte los datos del paciente\n");

        printf("Nombre: ");
        scanf("%s", paciente[i].nombre);

        printf("Apellido paterno: ");
        scanf("%s", paciente[i].apellidoP);

        printf("Apellido materno: ");
        scanf("%s", paciente[i].apellidoM);

        printf("Edad: ");
        scanf("%d", &paciente[i].edad);

        printf("Número telefónico: ");
        scanf("%s", paciente[i].noTel);

        printf("Cama en la que se encuentra: ");
        scanf("%d", &paciente[i].cama);
    }
    
    do
    {
        printf("\nPorfavor, seleccione una de las siguientes opciones\n1)Agregar un nuevo paciente\n2)Checar la ocupación de las camas\n3)Dar de alta a un paciente\n4)Ver a los pacientes\n5)Ver disponibilidad de camas\n");
        scanf("%d", &menu);

        printf("Escogió la opción: %d", menu);

        switch (menu)
        {
        case 0:
            printf("\nAdiós\n");
            break;
        
        case 1:
            break;

        case 2:
            break;

        case 3:
            break;

        case 4:
            break;

        case 5:
            break;

        default:
            printf("Escoga una opción válida!");
            break;
        }

    } while (menu != 0);
    

    return 0;
}
