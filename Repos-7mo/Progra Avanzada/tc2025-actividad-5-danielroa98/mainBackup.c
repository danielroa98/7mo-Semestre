/*
 *  Actividad 5
 *  Programación avanzada
 * 
 *  Daniel Roa
 *  A01021960
 * 
 *  Este ejercicio se trabajó con Sergio Hernández
 */
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void manual();

int main(int argc, char *const *argv)
{

    char *numberVal = NULL;
    int dat, updateValue = 0, man = 0, cont = 0, childCount = 0, contExit = 1, darth;
    pid_t pid;
    char witness = 'T';
    char wBoys;

    while ((dat = getopt(argc, argv, "n:m")) != -1)
    {
        switch (dat)
        {
        case 'n':
            numberVal = optarg;

            if (isdigit(*numberVal) > 0)
            {
                updateValue = atoi(numberVal);

                if (updateValue > 0)
                {
                    printf("Se van a crear %d procesos\n\n", updateValue);
                }
                else if (updateValue == 0)
                {
                    printf("Inserte un valor mayor a %d\n", updateValue);
                    exit(0);
                }
            }
            else
            {
                printf("Inserte un valor numerico valido.\n");
            }

            break;

        case 'm':

            manual();

            break;

        case '?':
            if (optopt == 'n')
            {
                fprintf(stderr, "Inserte un argumento a la opción -%c\n", optopt);
            }
            else if (isprint(optopt))
            {
                fprintf(stderr, "La opción -%c no es válida.\n", optopt);
            }
            else
            {
                fprintf(stderr, "Opción desconocida '\\x%x'.", optopt);
            }

            break;

        default:
            abort();
        }
    }

    // int *pipeline = (int *)malloc(sizeof(int) * (updateValue * 2));
    int pipeline[updateValue][2]; 

    for (; cont < updateValue; cont++){
        pipe(pipeline[cont]);
    }

    childCount = 1;
    printf("PRUEBA\n");

    close(pipeline[0][0]);
    write((pipeline[0][1]), &witness, sizeof(char));

    printf("<-- Soy el proceso padre (%d) y envié el testigo %c.\n", getpid(), witness);


    for (; childCount < updateValue; ++childCount)
    {
        pid = fork();

        if (pid == -1)
        {

            printf("Hubo un error al crear el proceso hijo %d.\n", childCount);
            break;
        }
        else if (pid == 0)
        {

            while (1)
            {
                
            
            close(*(pipeline + (2 * childCount - 1)));
            read(*(pipeline + (2 * childCount - 2)), &wBoys, sizeof(char));

            printf("—-> Soy el proceso con PID %d y recibí el testigo %c, el cual tendré por 5 segundos.\n", getpid(), wBoys);

            sleep(5);

            close(*(pipeline + (2 * childCount)));
            write(*(pipeline + 1), &wBoys, sizeof(char));

            printf("<-- Soy el proceso con PID %d y acabo de enviar el testigo %c.\n", getpid(), wBoys);
            }
        }
        else
        {

            if (childCount == 1)
            {
                close(*(pipeline));
                write(*(pipeline + 1), &witness, sizeof(char));

                printf("<-- Soy el proceso padre (%d) y envié el testigo %c.\n", getpid(), witness);

                darth = getpid();
                //printf("%d\n", darth[0]);
            }
            //exit(0);
        }
    }

    close(*(pipeline + ((updateValue - 1) * 2)));
    read(*(pipeline + (((updateValue - 1)*2+1))), &wBoys, sizeof(char));

    printf("—-> Soy el proceso con PID %d y recibí el testigo %c, el cual tendré por 5 segundos.\n", getpid(), wBoys);

    sleep(5);

    close(*(pipeline + (2 * childCount)));
    write(*(pipeline + 1), &wBoys, sizeof(char));

    printf("<-- Soy el proceso con PID %d y acabo de enviar el testigo %c.\n", getpid(), wBoys);

    waitpid(pid, NULL, 0);

    free(pipeline);

    return 0;
}

void manual()
{
    printf("Usage: ./a.out <options> <numeric value>\n");
    printf("The possible options are:\n");
    printf("-n\t\tAllows user to input a number in the numeric value field\n-m\t\tAllows the user to view this manual.\n");
}