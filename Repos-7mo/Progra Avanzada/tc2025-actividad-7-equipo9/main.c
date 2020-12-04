/*
 *  Actividad 7
 *  Programación avanzada
 *  Vicente Cubells
 * 
 *  Hecho por:
 *  Daniel Roa              -       A01021960
 *  Miguel Monterrubio      -       A01022153   
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>

// Change this value by basing it on the amount of signals you want to try inserting
#define signalAmount 15 

// 
char sigBuffer[signalAmount];
int cont = 0;

void signal_handler(int senial){
    
    char senialChar;

    printf("\nSe obtuvo la señal %d\n", senial);

    if(senial == 2){
        // printf("\n\nSeñal CTRL C\n\n");
        senialChar = (char)senial;
        sigBuffer[cont] = senialChar;
    }else if(senial == 9){
        // printf("\n\nSeñal de CTRL \ \n\n");
        sigBuffer[cont] = senial;
    }else if(senial == 20){
        // printf("\n\nSeñal CTRL Z \n\n");
        sigBuffer[cont] = senial;
    }

    printf("\nContador por el momento es %d\n", cont);
    cont++;
}

int main(int argc, char *const *argv)
{
    int tValue, docVal;
    char buffer[100];
    int dat;
    int grabar = 1;
    char* dirname = "datos";
    FILE *fp;

    //  Value to print the file in the datos directory 
    DIR *d = opendir("./datos");

    struct stat st = {0};
    struct dirent *dir;

    size_t arraySign = sizeof(sigBuffer) / sizeof(sigBuffer[0]);

    while ((dat = getopt(argc, argv, "n:t:")) != -1)
    {
        switch (dat)
        {
            case 'n':
                tValue = atoi(optarg);

                printf("Los documentos por crear son: %d\n", tValue);

                if (stat("./datos", &st) == -1) {
                    mkdir("./datos", 0700);
                    printf("Created datos folder\n");
                }

                else
                {
                    system("exec rm -r ./datos/*");
                    printf("Deleted all files in the 'datos' folder\n");
                }
                break;

            case 't':
                docVal = atoi(optarg);

                if(isdigit(docVal)>0){
                    printf("El tiempo de ejecución es de: %d\n", docVal);
                }else{
                    printf("Inserte un número valido para el tiempo %d\n", docVal);
                }


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
            printf("Correr con argumentos -n <numero de archivos a crear> y -t <tiempo para el temporizador>\n");
                break;
        }
    }

    if (signal(SIGINT, signal_handler) == SIG_ERR)
    {
        printf("ERROR: No se pudo establecer el manejador de la señal\n");
    }

    if (signal(SIGTSTP, signal_handler) == SIG_ERR)
    {
        printf("ERROR: No se pudo establecer el manejador de la señal\n");
    }

    if (signal(SIGQUIT, signal_handler) == SIG_ERR)
    {
        printf("ERROR: No se pudo establecer el manejador de la señal\n");
    }

    for (int i = 0; i < tValue; ++i)
    {
        sprintf(buffer, "datos/a%d", i);
        printf("Creating file called %s\n", buffer);
        fp = fopen(buffer, "w+");
        
        for (int i = docVal; i > 0; --i)
        {
            fputc('x', fp);
            printf("Time: %d\n", i);
            sleep(1);
            //alarm(docVal);
        }

        fputs("\n", fp);
        fputs("Señales usadas:\n", fp);

        for (size_t i = 0; i < arraySign; i++)
        {
            //printf("Position in array %d\n", sigBuffer[i]);
            // fputc(sigBuffer[i], fp);
            fprintf(fp, "%d\n", sigBuffer[i]);
        }

        fclose(fp);
        
        memset (sigBuffer, 0, sizeof(sigBuffer));
        cont = 0;
    }         
    
    printf("\n\nDocumentos en el directorio ./datos/\n");

    if(d){
        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\t", dir->d_name);
        }
        closedir(d);
    }

    printf("\n");

    return 0;
}