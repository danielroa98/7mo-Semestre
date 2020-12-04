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

#define N 7
#define ROBOTS 5
#define PESO 10

int tiendas[N];
int pesoTiendas[N];

pthread_mutex_t tiendasMutex[N] = PTHREAD_MUTEX_INITIALIZER;

int robot_entra = 0;

void *robo_orders(void *);

int main(int argc, char *const *argv[])
{

    srand((unsigned int)time(NULL));

    pthread_t *robots = (pthread_t)malloc(sizeof(pthread_t) * ROBOTS);

    int contRobots = 0;
    int initStore = 0;

    while (initStore < N)
    {
        tiendas[initStore] = ((rand() % 5) + 2 * N + PESO);

        initStore++;
    }

    while (contRobots < ROBOTS)
    {
        pthread_create(robots + contRobots, NULL, robo_orders, NULL);
        contRobots++;
    }

    for (pthread_t *aux = robots; aux < (robots + ROBOTS); ++aux)
    {
        pthread_join(*aux, NULL);
    }

    free(robots);

    return 0;
}

void *robo_orders(void *id)
{
    int roboPeso = PESO;
    int tiendasVisitadas = 0;
    int flagDoneShopping = 0;

    int roboID = rand() % 500;

    while (tiendasVisitadas < 7)
    {
        //If 0, no entra OR If 1, si entra
        int flagEntraTienda = rand() % 2;

        int flagPrint = 1;

        if (flagEntraTienda == 1)
        {
            printf("(R) Robot %d va a entrar a la tienda %d\n", roboID, tiendasVisitadas);

            int flagSalida = 1;

            while (flagSalida)
            {
                pthread_mutex_lock(&tiendasMutex[tiendasVisitadas]);

                //Revisa si el peso de la tienda/sección actual + el peso del robot es menor al peso de la tienda
                if ((pesoTiendas[tiendasVisitadas] + roboPeso) < tiendas[tiendasVisitadas]){
                    
                    pesoTiendas[tiendasVisitadas] += roboPeso;
                    
                    pthread_mutex_unlock(&tiendasMutex[tiendasVisitadas]);
                    
                    //Acceso abierto a mas robots
                    int pesoObjeto = (rand() % 2) + 1;
                    printf("(C) El robot %d entró y actualizó su peso de %d a %d\t<------>\tLa tienda tiene una capacidad de máxima de %d y su actual es de %d\n", roboID, roboPeso, (roboPeso + pesoObjeto), tiendas[tiendasVisitadas], pesoTiendas[tiendasVisitadas]);
                    
                    roboPeso += pesoObjeto;

                    flagSalida = 0;

                    usleep(5000);

                    pthread_mutex_lock(&tiendasMutex[tiendasVisitadas]);
                    //Robot sale de la tienda
                    pesoTiendas[tiendasVisitadas] += (-roboPeso + pesoObjeto);
                    printf("(S) El robot %d salió de la tienda %d\nEl peso de la tienda descendió a %d\n", roboID, tiendasVisitadas, pesoTiendas[tiendasVisitadas]);
                }else{
                    if(flagPrint == 1){

                        printf("(R) Robot %d no va a entrar a la tienda %d por que la tienda no lo aguanta.\nEl peso del robot es de %d y la capacidad de la tienda es de %d\n", roboID, tiendasVisitadas, roboPeso, tiendas[tiendasVisitadas]);

                        flagPrint = 0;
                    }
                }

                pthread_mutex_unlock(&tiendasMutex[tiendasVisitadas]);
            }
            tiendasVisitadas++;
        }
        else
        {
            tiendasVisitadas++;
        }
    }
    printf("\n(R) Robot %d completó sus compras\n", roboID);
}