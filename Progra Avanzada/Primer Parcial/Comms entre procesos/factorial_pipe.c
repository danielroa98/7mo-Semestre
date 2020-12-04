#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int factorial(int);
void escritura(int);

int main(int argc, const char * argv[]) {
    
    pid_t pid;
    int numero = 1;
    
    int pipeline[2];

    pipe(pipeline);

    while (numero != 0) {
        printf("Entre un número: ");
        scanf("%d", &numero);
        
        pid = fork();
        
        if (pid == -1)
        {
            printf("Error al crear el proceso hijo \n");
        }
        else if (pid == 0)
        {
            // Estamos en el hijo
            
            /* Calcular el factorial de número */
            
            printf("%d! = %d\n", numero, factorial(numero));
            
            break;
        }
        else {
            // Estamos en el padre

            close(pipeline[0]);
            printf("Actualmente se está escribiendo\n");
            
            wait(NULL);
        }
    }
    
    return 0;
}

int factorial(int n)
{
    int f = 2;
    int i = 3;
    
    if (n == 0 || n == 1) {
        return 1;
    }
    
    for (; i <= n; f *= i, ++i);
    
    return f;
}

void escritura(int fac){
    FILE *file;
    file = fdopen(fac, "w");

    sleep(10);

    int n = 0;
     
    

}