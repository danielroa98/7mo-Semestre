/*
Autór: Sergio Hernandez Castillo
Matrícula: A01025210
Descripción: Actividad 5 - IPC

Nota: Trabajé junto con Daniel Roa en este ejercicio
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

int main(int argc, char * const * argv){
    int dato;
    int cantidadDeHijos = 0;
    char * cvalue = NULL;

    opterr = 0;

    while ((dato = getopt(argc, argv, "n:")) != -1){
        switch (dato){
            case 'n':
                cvalue = optarg;

                if (isdigit(*cvalue) > 0){
                    cantidadDeHijos = atoi(cvalue);
                    printf("Cantidad de hijos: %d\n\n", cantidadDeHijos);
                }

                else {
                    printf("No se proporcionó un valor numérico\n");
                }
            break;

            case '?':
                if (optopt == 'n'){
                    fprintf(stderr, "Opción -%c requiere un argumento.\n", optopt);
                }
                    
                else if (isprint (optopt)){
                    fprintf (stderr, "Opción desconocida '-%c'.\n", optopt);
                }
                    
                else {
                    fprintf (stderr, "Opción desconocida '\\x%x'.\n", optopt);
                }
            
                return 1;
            break;

            default:
                abort();
            break;
        }
    }

    if (cantidadDeHijos == 0){
        printf("No se pueden crear %d hijos\n", cantidadDeHijos);

        return 0;
    }

    else if (cantidadDeHijos > 0){
        int * tubos = (int *) malloc(sizeof(int) * (cantidadDeHijos * 2));
        int j = 0;
        pid_t pid;
        char testigo = 'T';
        char caracter;

        for (; j < cantidadDeHijos; ++j){
            pipe((tubos + (2 * j)));
        }

        j = 0;

        for (; j <= cantidadDeHijos; ++j){
            pid = fork();

            if (pid == -1){
                printf("Hubo un error al crear el proceso hijo %d\n", j);

                break;
            }

            else if (pid == 0){
                close(*(tubos + (2 * j - 1)));
                read(*(tubos + (2 * j - 2)), &caracter, sizeof(char));

                printf("--> Soy el proceso con PID %d y recibí el testigo '%c', el cual tendré por 5 segundos.\n", getpid(), caracter);

                sleep(5);

                close(*(tubos + (2 * j)));
                write(*(tubos + (2 * j + 1)), &caracter, sizeof(char));

                printf("<-- Soy el proceso con PID %d y acabo de enviar el testigo '%c'.\n", getpid(), caracter);
            }

            else {
                if (j == 0){
                    close(*(tubos));
                    write(*(tubos + 1), &testigo, sizeof(char));

                    printf("<-- Soy el proceso padre con PID %d y acabo de enviar el testigo '%c'.\n", getpid(), testigo);
                }
            }
            
        }

        waitpid(pid, NULL, 0);


        free(tubos);
        return 0;
}