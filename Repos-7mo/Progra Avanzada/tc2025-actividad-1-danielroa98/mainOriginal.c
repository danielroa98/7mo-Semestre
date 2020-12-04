/*
 *  Actividad Hospital
 *
 *  Daniel Roa
 *  A01021960
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct camasHospitales{
    int noCama;                     //Número de la cama en la que se encuentra un paciente
    int inUse;                  //Valor 1 o 0 dependiendo si está en uso
}cama;

typedef struct registroPacientes{
    char nombre[30];
    char apellidoP[30];
    int edad;
    char noTel[15];
    struct camasHospitales camaActual;
    struct camasHospitales enUso;
} pacientes;

int main(int argc, char const *argv[])
{

    int beds, menu = 0, mas = 0, i = 1, nuevos = 5, bus, bedNo = 0, newPacients, contUsadas = 0, contVacias = 0;
    pacientes * hospital;
    pacientes * sup;

    char adios = '-';

    int vacia = 0;
    int ocupada = 1; 

    printf("Cuántos pacientes tiene registrados? ");
    scanf("%d", &beds);

    hospital = (pacientes *) malloc(sizeof(pacientes)*beds);

    pacientes * fin = hospital + beds;

    for (sup = hospital; sup < fin; ++sup){
        printf("Inserte los datos del paciente\n");

        // sup->nombre = (char *) malloc(sizeof(char) * 30);
        printf("Nombre y apellido (paterno): ");
        scanf("%s %s", sup->nombre, sup->apellidoP);

        printf("Edad: ");
        scanf("%d", &sup->edad);

        printf("Número telefónico: ");
        scanf("%s", sup->noTel);

        printf("Cama en la que se encuentra: ");
        scanf("%d", &sup->camaActual.noCama);

        sup->enUso.inUse=1;

        printf("\n");
    }
    
    do{

    printf("\n\nEscoga una de las siguientes opciones:\n1)Añadir un paciente nuevo\n2)Buscar a un paciente\n3)Dar de alta a un paciente\n4)Ver a los pacientes registrados\n5)Ver la disponibilidad de las camas\nPara salir, presione el botón 0\n");
    scanf("%d", &menu);

        switch (menu)
        {
        case 0:
            printf("Adios\n");
            break;
        
        case 1:
            printf("\nCuántos pacientes desea añadir?\n");
            scanf("%d", &mas);

            printf("\nAutomaticamente se van a añadir 5 lugares nuevos\n");

            hospital = (pacientes *)realloc(hospital, sizeof(pacientes) * (beds + nuevos));

            printf("Ingresa el nombre de los pacientes a insertar\n(deja un espacio en blanco si ya no hay pacientes por insertar)\n");

            fin = hospital + beds + nuevos;

            for (sup = hospital; sup < fin; ++sup)
            {
                printf("\nPaciente #%d\n", i);

                printf("Nombre y apellido (paterno): ");
                scanf("%s %s", sup->nombre, sup->apellidoP);

                printf("Edad: ");
                scanf("%d", &sup->edad);

                printf("Número telefónico: ");
                scanf("%s", sup->noTel);

                printf("Cama en la que se encuentra: ");
                scanf("%d", &sup->camaActual.noCama);

                sup->enUso.inUse = 1;

                i++;
            }

            break;

        case 2:
            printf("Las camas registradas son las siguientes:\n");

            for (sup = hospital; sup < fin; ++sup)
            {
                printf("%d\n", sup->camaActual.noCama);
            }

            printf("Escriba el número de la cama que desea buscar para ver si está siendo ocupada\n");
            scanf("%d", &bus);

            for (sup = hospital; sup < fin; sup++){

                if (sup->enUso.inUse == bus){
                    printf("\nEsa cama está siendo ocupada por: %s %s\n", sup->nombre, sup->apellidoP);
                }else{
                    printf("\nEsa cama está desocupada.\n");
                }
                
            }
            
            break;

        case 3:
            printf("¿Qué paciente desea dar de alta?\n");
            scanf("%d", &bedNo);

            for (sup = hospital; sup < fin; ++sup){
                
                if (sup->camaActual.noCama == bedNo){
                    
                    printf("%s", sup->nombre);

                    sup->enUso.inUse = 0;
                }else{
                    printf("Por favor inserte un número válido.\n");
                }
                
                
            }
            

            break;

        case 4:

            for (sup = hospital; sup < fin; ++sup){
                printf("Nombre: %s %s \t\t Cama en la que se encuentra: %d \n", sup->nombre, sup->apellidoP, sup->camaActual.noCama);
            }
            
            break;

        case 5:

            for (sup = hospital; sup < fin; ++sup){
                
                if (sup->enUso.inUse == 1){
                    contUsadas += 1;
                }else if (sup->enUso.inUse == 0){
                    contVacias += 1;
                }
                
            }
            
            printf("Hay %d camas ocupadas y %d camas vacías", contUsadas, contVacias);

            break;

        default:
            printf("Porfavor, elija una opción valida\n");
            break;
        }
        

    } while (menu != 0);
    
    for (sup = hospital; sup < fin; ++sup){
        free(sup->nombre);
        free(sup->apellidoP);
        free(sup->noTel);
    }
    

    return 0;
}