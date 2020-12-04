/*
Autór: Sergio Hernandez Castillo
Matrícula: A01025210
Descripción: Actividad 4 - Procesos
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

typedef struct {
    int id;
    int promedio;
} Hijo;

void leer(int * fd);
void escribir(int * fd);

int main(int argc, char * const * argv){
    int dato;
    int estado;
    int cantidadDeHijos;
    char * cvalue = NULL;

    opterr = 0;

    while ((dato = getopt(argc, argv, "n:")) != -1){
        switch (dato){
            case 'n':
                cvalue = optarg;

                if (isdigit(*cvalue) > 0){
                    cantidadDeHijos = atoi(cvalue);
                    printf("Cantidad de hijos: %d\n", cantidadDeHijos);
                }

                else {
                    printf("No proporcionó un valor numérico\n");
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

            default:
                abort();
            break;
        }
    }

    Hijo * hijos = (Hijo *) malloc(sizeof(Hijo) * cantidadDeHijos);
    Hijo * fin = hijos + cantidadDeHijos;
    Hijo * h = hijos;
    int i = 0;
    int tuberia[2];
    int hijosCreados = 0;
    pid_t pid;

    pipe(tuberia);

    while ((h < fin) && (i < cantidadDeHijos)){
        pid = fork();

        if (pid == -1){
            printf("Hubo un error al crear el proceso hijo %d\n", i);
            printf("Procesos hijos creados hasta el momento: %d\n", hijosCreados);

            break;
        }

        else if (pid == 0){
            printf("Estamos en el proceso hijo con PID = %d y su padre es PPID = %d\n", getpid(), getppid());
            escribir(tuberia);
            // exit(0);
        }

        else {
            printf("Estamos en el proceso padre con PID = %d\n", getpid());

            if (waitpid(pid, &estado, 0) != -1){
                if (WIFEXITED(estado)){
                    printf("Ya termino el hijo con PID %d con valor de retorno %d\n", pid, WEXITSTATUS(estado));
                    ++hijosCreados;
                    leer(tuberia);
                }
            }

            printf("\n");
        }

        ++h;
        ++i;
    }

    // printf("Hijos creados: %d", hijosCreados);

    free(hijos);

    return 0;
}

void leer(int * fd){
    int c;

    while (1){
        close(fd[1]);
        read(fd[0], &c, sizeof(int));
        printf("--- Recibí un promedio de %d\n", c);
    }
}

void escribir(int * fd){
    int idPadre = getppid();
    int idHijo = getpid();
    int promedio = (idPadre + idHijo) / 2;

    close(fd[0]);
    printf("+++ Envío un promedio de %d\n", promedio);
    write(fd[1], &promedio, sizeof(int));
}