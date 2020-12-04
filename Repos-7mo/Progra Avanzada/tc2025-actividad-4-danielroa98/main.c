/*
 *  Actividad 4
 *  Programación avanzada
 * 
 *  Daniel Roa
 *  A01021960
 * 
 *  Páginas de soporte:
 *  https://www.gnu.org/software/libc/manual/html_node/Using-Getopt.html
 *  http://www.cplusplus.com/reference/cstdlib/atoi/
 * 
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void easter_egg();
void instruction();

typedef struct procceses{
    int childP;
    int avg;
} procDat;

int main(int argc, char * const *argv)
{
    char * numberVal = NULL;
    int dat, itSup = 0, convNumber = 0, contPID = 0, contChildren = 0, state, avg = 0, contGlobal = 0, index;
    pid_t pid;
    
    //Error value - opterr se convierte en ? cuando marca un error
    opterr = 0;

    while ((dat = getopt(argc, argv, "n:em")) != -1){
        switch (dat)
        {

        //Caso principal, donde el usuario insertó n números
        case 'n':
            numberVal = optarg;

            if(isdigit(*numberVal) > 0){
                
                //Convert char to int
                convNumber = atoi(numberVal);
            
                printf("Insertó %d procesos para crearse\n\n", convNumber);
            }else{
                printf("Inserte un valor numerico.\n");
            }
            break;
        
        //En caso que el usuario quiera ver el manual de uso del programa
        case 'm':
            itSup = 1;
            break;

        //Valor de prueba, para asegurarme que funcionara con mas de dos 'flags'
        case 'e':
            easter_egg();
            break;

        case '?':
            if (optopt == 'n'){
            
                fprintf(stderr, "Inserte un argumento a la opción -%c\n",optopt);
            
            }else if(isprint(optopt)){
            
                fprintf(stderr, "La opción -%c no es válida.\n", optopt);
            
            }else{

                fprintf(stderr, "Opción desconocida '\\x%x'.",optopt);
            
            }
            
            
            break;

        //Para la ejecución del programa
        default:
            abort();
        }
    }

    pid_t * procs = malloc(sizeof(pid_t) * convNumber);
    int * procsTot = procs + convNumber;
    int * contProcs = procs;

    while ((contProcs < procsTot) && (contGlobal < convNumber))
    {
        pid = fork();
        *contProcs = pid;

        if (pid == -1){

            printf("Error al crear hijo.\n");
            printf("Se alcanzaron a crear %d procesos hijos.", contChildren);
        }else if(pid == 0){
            //Dentro del proceso hijo
            //printf("El proceso hijo es %d y su padre es %d\n", getpid(), getppid());

            int top = getppid();
            int bot = getpid();

            avg = (top+bot)/2;
            printf("Soy el proceso hijo con PID = %d y mi promedio es = %d\n", bot, avg);

            sleep(1);
            exit(avg);

        }else{
            //Dentro del proceso padre
            //printf("\nDentro del proceso padre\n");
            contChildren+=1;
        }
        ++contGlobal;
        ++contProcs;
    }

    contProcs = procs;

    procDat *proc = (procDat *)malloc(sizeof(procDat) * convNumber);
    procDat *fin = proc + convNumber;
    procDat *helper = proc;
    procDat *cont = proc;

    while ((helper < fin) && (contProcs < procsTot)){

        if (waitpid(*contProcs, &index, 0) != -1){

            if (WIFEXITED(index)){

                helper->childP = *contProcs;
                helper->avg = WEXITSTATUS(index);

            }
        }

        ++helper;
        ++contProcs;
    }
    
    
    procDat * accountant = proc;
        

        for (; accountant < fin; ++accountant){
            printf("\nPID Hijo \t Promedio \t Histograma");
            
        /*  printf("\nEste es un demo de impresión %d avg = %d\n", accountant->childP, accountant->avg); */

            int pidHijo = accountant->childP;
            int childAvg = accountant->avg;

            printf("\n%d \t\t %d \t\t ", pidHijo, childAvg);

            int fixedAvg = childAvg/100;

            for (int ast = 0; ast < childAvg; ++ast)
            {
                printf("*");
            }
            
        }

        printf("\n");

    if(itSup == 1){

        instruction();

    }

    return 0;
}

void easter_egg(){
    printf("Congrats! You just found me!\n\nPlease mark me with a great grade!\n:)\n");
}

void instruction(){
    printf("Usage: ./a.out <options> <numeric value>\n");
    printf("The possible options are:\n");
    printf("-n\t\tAllows user to input a number in the numeric value field\n-m\t\tAllows the user to view this manual\n-e\t\tRun it to find out!\n");
    printf("\nThe asterisks (*) being printed are based on the average being divided by 100.\n");
}