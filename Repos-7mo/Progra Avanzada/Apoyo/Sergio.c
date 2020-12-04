/*
Autór: Sergio Hernandez Castillo
Matrícula: A01025210
Descripción: Actividad 1 - Hospital

1. El hospital cuenta con 5 camas. 
2. Se llena el hospital hasta 5 camas con un for al iniciar el programa.
3. Abrir un switch:
3a. Incorpora mas pacientes (los que se requieran), si se llenan las camas, hay
    que hacer espacio. 
3b. Ver que paciente esta asignado a una cama, o si esta libre. 
3c. Dar de alta a un paciente por el numero de cama (se borra ese paciente del 
    hospital y su cama queda libre).
3d. Mostrar la lista de todos los pacientes del hospital. 
3e. Conocer cuantas camas disponibles hay y cuantas estan ocupadas. 
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int enUso;
} cama;

typedef struct {
    char * nombre;
    char * apellidos;
    int edad;
    char * telefono;
    cama c;
} paciente;

int main(){
    paciente * hospital;
    paciente * p;
    paciente * totalHospital;
    cama * camas;
    cama * c;
    cama * totalCamas;
    int limiteDeCamas = 3;
    int opcion;
    int cantidad = 0; 
    int contadorCamas;

    hospital = (paciente *) malloc(sizeof(paciente) * limiteDeCamas);
    camas = (cama *) malloc(sizeof(cama) * limiteDeCamas);
    totalHospital = hospital + limiteDeCamas;
    totalCamas = camas + limiteDeCamas;

    printf("\nA continuación, proporcione los datos de los primeros 3 pacientes: ");

    p = hospital;
    c = camas;

    while ((p < totalHospital) && (c < totalCamas)){
        p->nombre = (char *) malloc(sizeof(char) * 25);
        printf("\nProporcione el nombre del paciente: ");
        scanf("%s", p->nombre);

        p->apellidos = (char *) malloc(sizeof(char) * 25);
        printf("Proporcione el apellido(s) del paciente: ");
        scanf("%[˄\n]", p->apellidos);

        printf("Proporcione la edad del paciente: ");
        scanf("%d", &p->edad);

        p->telefono = (char *) malloc(sizeof(char) * 15);
        printf("Proporcione el numero telefónico del paciente: ");
        scanf("%s", p->telefono);

        contadorCamas++;
        c->id = contadorCamas;
        printf("Cama: %d", c->id);

        c->enUso = 1;

        ++p;
        ++c;
    }

    printf("\n--Hospital General 'Dr. Sergio Hernandez Castillo'-- \n1. Agregar pacientes \n2. Checar que paciente está usando que cama \n3. Dar de alta a un paciente \n4. Ver listado de pacientes \n5. Numeros de camas disponibles y usadas \n0. Salir");
    
    printf("\nElija una opción: ");
    scanf("%d", &opcion);

    switch (opcion){
        case 1:
            printf("Proporcione la cantidad de pacientes que desea agregar: ");
            scanf("%d", &cantidad);

            while (cantidad < 1){
                printf("ERROR: Solo se puede agregar un numero positivo de pacientes.");

                printf("Proporcione la cantidad de pacientes que desea agregar: ");
                scanf("%d", &cantidad);
            }

            if (cantidad + contadorCamas >= limiteDeCamas){

                printf("Dentro del if");

                hospital = (paciente *) realloc(hospital, sizeof(paciente) * (limiteDeCamas + 5));
                camas = (cama *) realloc(camas, sizeof(cama) * (limiteDeCamas + 5));
                totalHospital = hospital + contadorCamas + cantidad;
                totalCamas = camas + contadorCamas + cantidad;

                p = hospital + contadorCamas;
                c = camas + contadorCamas;

                while ((p < totalHospital) && (c < totalCamas)){
                    p->nombre = (char *) malloc(sizeof(char) * 25);
                    printf("\nProporcione el nombre del paciente: ");
                    scanf("%s", p->nombre);

                    p->apellidos = (char *) malloc(sizeof(char) * 25);
                    printf("Proporcione el apellido(s) del paciente: ");
                    scanf("%[˄\n]", p->apellidos);

                    printf("Proporcione la edad del paciente: ");
                    scanf("%d", &p->edad);

                    p->telefono = (char *) malloc(sizeof(char) * 15);
                    printf("Proporcione el numero telefónico del paciente: ");
                    scanf("%s", p->telefono);

                    contadorCamas++;
                    c->id = contadorCamas;
                    printf("Cama: %d", c->id);

                    c->enUso = 1;

                    ++p;
                    ++c;
                }

                limiteDeCamas = contadorCamas + cantidad + (5 - cantidad);
            }

            if (cantidad + contadorCamas < limiteDeCamas){
                totalHospital = hospital + contadorCamas + cantidad;
                totalCamas = camas + contadorCamas + cantidad;

                p = hospital + contadorCamas;
                c = camas + contadorCamas;

                while ((p < totalHospital) && (c < totalCamas)){
                    p->nombre = (char *) malloc(sizeof(char) * 25);
                    printf("\nProporcione el nombre del paciente: ");
                    scanf("%s", p->nombre);

                    p->apellidos = (char *) malloc(sizeof(char) * 25);
                    printf("Proporcione el apellido(s) del paciente: ");
                    scanf("%[˄\n]", p->apellidos);

                    printf("Proporcione la edad del paciente: ");
                    scanf("%d", &p->edad);

                    p->telefono = (char *) malloc(sizeof(char) * 15);
                    printf("Proporcione el numero telefónico del paciente: ");
                    scanf("%s", p->telefono);

                    contadorCamas++;
                    c->id = contadorCamas;
                    printf("Cama: %d", c->id);

                    c->enUso = 1;

                    ++p;
                    ++c;
                }
            }

        break;
    }

    return 0;
}