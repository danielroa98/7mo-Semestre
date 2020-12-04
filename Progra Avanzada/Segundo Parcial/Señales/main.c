#include <stdio.h>
#include <signal.h>

void manager(int ids){
    printf("--- Presionaste Ctrl + C ...\n");
}

int main(int argc, char const *argv[])
{
    if(signal(SIGKILL, manager) == SIG_ERR){
        printf("ERROR: no se pudo establecer al manejador de la señal.\n");
    }

    int k = 10;

    while (k --> 10)
    {
        /* code */
    }
    

    return 0;
}
