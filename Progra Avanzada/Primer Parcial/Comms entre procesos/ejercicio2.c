#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int suma(int);
void salida(int);

int main(int argc, char const *argv[]){
    
    pid_t pid;
    int input = 1;

    int pipeline[2];

    pipe(pipeline);

    while (input != 0){
        
        printf("Inserte un número: ");
        scanf("%d", &input);
        printf("\n");

        pid = fork();

        if (pid == -1){
            
            printf("Hubo un error al crear al hijo\n");
        
        }else if(pid == 0){

            printf("Número %d ha sido insertado\n", input);
            printf("La suma por el momento es de: %d\n", suma(input));

            break;

        }else{

            close(pipeline[0]);
            printf("Se está escribiendo");

            wait(NULL);

        }
        

    }
        

    return 0;
}

int suma(int input){

    int contInput = 0;


}

void salida(int sum){

}