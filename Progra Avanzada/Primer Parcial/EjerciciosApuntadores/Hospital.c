/*
 *  Actividad Hospital
 *
 *  Daniel Roa
 *  A01021960
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct registroPacientes
{
    char *nombre;
    char *apellido;
    int edad;
    char *noTel;
} pacientes;

typedef struct registroCamas
{
    int noCama;
    int enUso;
    pacientes *paciente;
} camas;

int main(int argc, char const *argv[])
{

    int new = 0,
        registro = 2,
        menu = 0,
        contUsadas = 0,
        contVacias = 0,
        bus = 0,
        adios = 0,
        bedNo = 0,
        statusO = 0,
        controlCamas = 0,
        camasOr = 0;

    camas *cuarto = (camas *)malloc(sizeof(camas) * registro);

    printf("\nPor favor, ingrese los datos de los dos pacientes a continuación.");

    camas *fin = cuarto + registro;

    for (camas *sup = cuarto; sup < fin; ++sup)
    {
        printf("\nInserte los datos que se soliciten a continuación.\n");

        pacientes *tuPac = (pacientes *)malloc(sizeof(pacientes));

        tuPac->nombre = (char *)malloc(sizeof(char) * 30);
        printf("Nombre: ");
        scanf("%s", tuPac->nombre);

        tuPac->apellido = (char *)malloc(sizeof(char) * 30);
        printf("Apellido: ");
        scanf("%s", tuPac->apellido);

        printf("Edad: ");
        scanf("%d", &tuPac->edad);

        tuPac->noTel = (char *)malloc(sizeof(char) * 14);
        printf("Número telefónico: ");
        scanf("%s", tuPac->noTel);

        bedNo += 1;

        printf("Se encuentra en la cama: %d", bedNo);

        sup->noCama = bedNo;

        sup->enUso = 1;
        sup->paciente = tuPac;

        printf("\n");
        controlCamas = bedNo;
    }

    do
    {

        printf("\nEscoga alguna de las siguientes opciones:\n1) Añadir un paciente nuevo\n2) Buscar a un paciente usando el número de la cama\n3) Dar de alta a un paciente\n4) Ver el registro de pacientes\n5) Ver cuantas camas están disponibles\nO presione el 0 para salir\n");
        scanf("%d", &menu);

        switch (menu)
        {
        case 0:
            break;

        case 1:
            printf("Primer caso\n");

            printf("Ingrese los datos del nuevo paciente.\n");

            /* if (controlCamas < bedNo)
            {
                for (camas * cama = 0; cama < fin; cama++)
                {
                    
                }
                
            } */

            /* else */ if (controlCamas == bedNo)
            {

                bedNo += 5;

                cuarto = (camas *)realloc(cuarto, sizeof(camas) * bedNo);

                fin = cuarto + bedNo;

                camasOr = controlCamas;

                for (camas *sup = cuarto + controlCamas; sup < fin; ++sup)
                {
                    sup->enUso = 0;
                    controlCamas++;
                    sup->noCama = bedNo;
                }

                pacientes *tuPac = (pacientes *)malloc(sizeof(pacientes));

                tuPac->nombre = (char *)malloc(sizeof(char) * 30);
                printf("Nombre: ");
                scanf("%s", tuPac->nombre);

                tuPac->apellido = (char *)malloc(sizeof(char) * 30);
                printf("Apellido: ");
                scanf("%s", tuPac->apellido);

                printf("Edad: ");
                scanf("%d", &tuPac->edad);

                tuPac->noTel = (char *)malloc(sizeof(char) * 14);
                printf("Número telefónico: ");
                scanf("%s", tuPac->noTel);

                camasOr++;

                printf("Se encuentra en la cama: %d", camasOr);

                printf("\n");
            }

            break;

        case 2:
            printf("Las camas disponibles son las siguientes:\n");

            for (camas *sup = cuarto; sup < fin; ++sup)
            {
                printf("%d ", sup->noCama);
            }

            printf("\nIngrese el número de cama que desea buscar\n");
            scanf("%d", &bus);

            for (camas *sup = cuarto; sup < fin; ++sup)
            {
                if (bus == sup->noCama)
                {

                    if (sup->enUso == 1)
                    {

                        printf("\nEsa cama está siendo ocupada por: %s %s\n", sup->paciente->nombre, sup->paciente->apellido);
                        break;
                    }
                    else
                    {
                        printf("\nEsa cama está desocupada.\n");
                    }
                }
            }

            break;

        case 3:
            printf("\nLas camas actualmente ocupadas son las siguientes...\n");

            for (camas *sup = cuarto; sup < fin; ++sup)
            {
                printf("%d\t", sup->noCama);
            }

            printf("\n\nA qué paciente desea darle el alta? ");
            scanf("%d", &adios);

            for (camas *sup = cuarto; sup < fin; sup++)
            {

                if (adios == sup->noCama)
                {

                    free(sup->paciente->nombre);
                    free(sup->paciente->apellido);
                    free(sup->paciente->noTel);
                    free(sup->paciente);

                    sup->enUso = 0;
                }
            }

            break;

        case 4:
            // printf("Cuarto caso\n");
            printf("\nNombre y apellido\t \t\t cama asignada\t \t\t Edad\t \t\t Número telefónico\n");

            for (camas *sup = cuarto; sup < fin; ++sup)
            {

                if (sup->enUso == 1)
                {
                    printf("%s %s\t \t\t\t %d\t \t\t\t %d\t \t\t\t %s\n", sup->paciente->nombre, sup->paciente->apellido, sup->paciente->edad, sup->noCama, sup->paciente->noTel);
                }
            }

            break;

        case 5:
            // printf("Caso 5");

            contUsadas = 0;
            contVacias = 0;

            for (camas *sup = cuarto; sup < fin; ++sup)
            {

                if (sup->enUso == 1)
                {
                    contUsadas++;
                }
                else if (sup->enUso == 0)
                {
                    contVacias++;
                }
            }

            printf("\nCamas ocupadas: %d\nCamas vacías: %d\n", contUsadas, contVacias);

            break;

        default:
            break;
        }

    } while (menu != 0);

    for (camas *sup = cuarto; sup < fin; ++sup)
    {
        free(sup->paciente->nombre);
        free(sup->paciente->apellido);
        free(sup->paciente->noTel);
    }

    free(cuarto);

    return 0;
}
