/*
 *  Actividad 10
 *  Programación avanzada
 *  Vicente Cubells
 * 
 *  Hecho por:
 *      Daniel Roa              -       A01021960
 *      Miguel Monterrubio      -       A01022153 
 * 
 *  Se trabajó con los miembros del equipo 10; 
 *      Sergio Hernández
 *      Antonio Junco  
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

// Cantidad de Threads a crear
#define ENANOS 7

pthread_mutex_t mesa = PTHREAD_MUTEX_INITIALIZER;

int comida_lista = 0;
int enanos_en_mesa = 0;
int enanos_satisfechos = 0;

void *blancanieves_esta_chambeando(void *);
void *vete_a_la_mesa(void *);

int main(int argc, char *const *argv[])
{
    pthread_t *enanitos = (pthread_t)malloc(sizeof(pthread_t) * ENANOS);
    pthread_t *blancanieves = (pthread_t)malloc(sizeof(pthread_t) * 1);

    int contEnanos = 0;

    pthread_create(blancanieves, NULL, blancanieves_esta_chambeando, NULL);

    while (contEnanos < ENANOS)
    {
        pthread_create(enanitos + contEnanos, NULL, vete_a_la_mesa, NULL);
        contEnanos++;
    }

    for (pthread_t *sup = enanitos; sup < (enanitos + ENANOS); ++sup)
    {
        pthread_join(*sup, NULL);
    }

    free(blancanieves);
    free(enanitos);

    return 0;
}

// Enano llega a la mesa puede que no se haya sentado
void *vete_a_la_mesa(void *id)
{
    int flagEnanos = 1;

    printf("(M) Enano llega a la mesa.\n");

    srand((unsigned int)time(NULL));

    // While usado para sentarse en la mesa
    while (flagEnanos)
    {
        pthread_mutex_lock(&mesa);

        if (enanos_en_mesa < 4)
        {
            //Indica que ya está sentado
            printf("(S) Enano se sienta en la mesa.\n");

            ++enanos_en_mesa;

            if (enanos_en_mesa == 4)
            {
                printf("Mesa llena\n");
            }

            flagEnanos = 0;
        }

        pthread_mutex_unlock(&mesa);
    }

    //While usado para comer la comida de Blancanieves
    flagEnanos = 1;

    while (flagEnanos){
        pthread_mutex_lock(&mesa);

        if(comida_lista >= 1){
            printf("(C) Enano está comiendo el platillo.\n");
            --comida_lista;
            
            flagEnanos = 0;
            
            enanos_satisfechos++;
            
            printf("(M) Enano regresa a la mina.\n");
            
            enanos_en_mesa--;
        }
    
        
        pthread_mutex_unlock(&mesa);
    }

}

// Enano come la comida
void *blancanieves_esta_chambeando(void *id){

    int flagSW = 1;

    while (flagSW){
        
        pthread_mutex_lock(&mesa);
        //Indica si le sirve comida a un enano
        if((enanos_en_mesa >= 1) && (comida_lista < enanos_en_mesa)){
            
            printf("(SW) Blancanieves esta preparando un platillo.\n");
            
            ++comida_lista;
        
        }

        if(enanos_satisfechos == 7){
            printf("(SW) Blancanieves sale a dar una vuelta.\n");

            flagSW = 0;
        }

        pthread_mutex_unlock(&mesa);
    }
    
}
