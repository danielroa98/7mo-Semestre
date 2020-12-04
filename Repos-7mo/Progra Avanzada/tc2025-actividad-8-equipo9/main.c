/*
 *  Actividad 8
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
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define BSIZE 150
#define N 150

#define PRODUCTORES 1
#define CONSUMIDORES 8
#define GENERAL 100
#define EMPRESARIOS 50

typedef struct
{
    int id;
    int tipoCliente;
} cliente;

cliente elementos[BSIZE];

int in = 0;
int out = 0;

int total = 0;
int max = 0;

int producidos = 0, consumidos = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t consume_t = PTHREAD_COND_INITIALIZER;
pthread_cond_t produce_t = PTHREAD_COND_INITIALIZER;

void *productor(void *);
void *consumidor(void *);
int encuentraSiguiente(int, int);

int main(int argc, const char *argv[])
{

    srand((int)time(NULL));

    int nhilos = PRODUCTORES + CONSUMIDORES;

    pthread_t *obreros = (pthread_t *)malloc(sizeof(pthread_t) * nhilos);

    int indice = 0;

    pthread_t *aux;

    printf("\nNOTA\nLas operaciones están catalogadas como sigue:\n0 --> General\n1 --> Empresarial\n\n");

    /* Crear los productores */
    for (aux = obreros; aux < (obreros + PRODUCTORES); ++aux)
    {
        printf("(I) Creando el productor %d\n", ++indice);
        pthread_create(aux, NULL, productor, (void *)indice);
    }

    /* Crear los consumidores */
    indice = 0;

    for (; aux < (obreros + nhilos); ++aux)
    {
        printf("(I) Creando el consumidor %d\n", ++indice);
        pthread_create(aux, NULL, consumidor, (void *)indice);
    }

    /* Adjuntar los hilos */
    for (aux = obreros; aux < (obreros + nhilos); ++aux)
    {
        pthread_join(*aux, NULL);
    }

    free(obreros);

    return 0;
}

void *productor(void *arg)
{
    int id = (int)arg;
    int genCounter = 0;
    int empCounter = 0;

    printf("(P) Inicia productor %d\n", id);
    srand(time(0));
    while (producidos < N)
    {
        int tipo = 2;

        /* Asumir que trabajan a diferentes velocidades */
        if ((((rand() % (1 + 1) - 0) + 0) == 0) && (genCounter < GENERAL))
        {
            //General
            sleep((rand() % (22 - 5 + 1)) + 5);
            tipo = 0;
            genCounter++;
        }
        else if (empCounter < EMPRESARIOS)
        {
            //Empresarial
            sleep((rand() % (34 - 9 + 1)) + 9);
            tipo = 1;
            empCounter++;
        }
        else
        {
            //General
            sleep((rand() % (22 - 5 + 1)) + 5);
            tipo = 0;
            genCounter++;
        }

        pthread_mutex_lock(&mutex);

        if (total < BSIZE)
        {

            /* Produce un elemento */

            elementos[in].id = producidos;
            elementos[in].tipoCliente = tipo;

            printf("+++ (Productor %d) Llego un cliente%d con la transacción de tipo %d\n", id, elementos[in].id, elementos[in].tipoCliente);

            ++max;

            ++producidos;

            ++in;
            in %= BSIZE;
            ++total;

            if (total == 1)
            {
                pthread_cond_broadcast(&consume_t);
            }
        }
        else if (producidos < N)
        {
            /* El buffer está lleno, se va a dormir */

            printf("-------------- Productor %d se durmió ------------\n", id);
            pthread_cond_wait(&produce_t, &mutex);
            printf("-------------- Productor %d se despertó ------------\n", id);
        }

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void *consumidor(void *arg)
{
    int id = (int)arg;

    int clientesContados = 0;

    printf("(C) Inicia proceso del consumidor #%d\n", id);

    while (consumidos < N)
    {
        if (clientesContados == 5)
        {
            printf("--- (Cajero %d) El cajero esta en reposo\n", id);
            sleep(3);
            clientesContados = 0;
        }

        /* Asumir que trabajan a diferentes velocidades */

        //Empieza zona critica
        pthread_mutex_lock(&mutex);
        int cliente = elementos[out].tipoCliente;

        if (total > 0)
        {
            /* Consume un elemento */
            if (cliente == 2)
            {
                //Este elemento ya fue leido, sigamos al SIGUIENTE
                ++out;
                out %= BSIZE;

                if (total == (BSIZE - 1))
                {
                    pthread_cond_broadcast(&produce_t);
                }
            }
            //--------------------------------Cajeros 0- 4-----------------------
            else if (id < 5)
            {

                //Solo atiende a General
                int pos = encuentraSiguiente(0, max);
                //Existe general-----------------------------------
                if (pos != -1)
                {
                    //Encontramos a un general

                    printf("--- (Cajero %d) El cajero atiende a %d con la operacion %d\n", id, elementos[pos].id, elementos[pos].tipoCliente);
                    ++clientesContados;
                    ++consumidos;
                    --total;

                    //Este elemento ya lo leimos
                    elementos[pos].tipoCliente = 2;
                }
                //Existe general end-----------------------------------
                //No hay general-----------------------------------
                else
                {
                    pthread_cond_wait(&consume_t, &mutex);
                }
                //No hay general end-----------------------------------
            }
            //--------------------------------Cajeros 0- 4 end-----------------------
            //--------------------------------Cajeros 5-7-----------------------
            else
            {
                //Puede atender a general y empresarial
                //BUSCAR AL SIGUIENTE EMPRESARIO
                int pos = encuentraSiguiente(1, max);
                //Existe empresario-----------------------------------
                if (pos != -1)
                {
                    //Encontramos a un empresario
                    printf("--- (Cajero %d) El cajero atiende a %d con la operacion %d\n", id, elementos[pos].id, elementos[pos].tipoCliente);
                    ++clientesContados;
                    ++consumidos;
                    --total;
                    //Este elemento ya lo leimos
                    elementos[pos].tipoCliente = 2;
                }
                //Existe empresario end-----------------------------------
                //No hay empresario Utilizar a uno general-----------------------------------
                else
                {
                    printf("--- (Cajero %d) El cajero atiende a %d con la operacion %d\n", id, elementos[out].id, elementos[out].tipoCliente);
                    ++clientesContados;
                    elementos[out].tipoCliente = 2;

                    ++consumidos;
                    ++out;
                    out %= BSIZE;
                    --total;

                    if (total == (BSIZE - 1))
                    {
                        pthread_cond_broadcast(&produce_t);
                    }
                }
                //No hay empresario end-----------------------------------
            }
            //--------------------------------Fin cajeros 5-7-----------------------
        }
        else if (consumidos < N)
        {
            /* El buffer está vacío, se va a dormir */
            pthread_cond_wait(&consume_t, &mutex);
        }

        //Salimos de la region critica
        pthread_mutex_unlock(&mutex);
        //Atiende en promedio de 3-5 segundos
        sleep((rand() % (5 - 3 + 1)) + 3);
    }

    pthread_exit(NULL);
}

int encuentraSiguiente(int identificador, int maximo)
{
    int pos = -1;
    for (int i = out; i < maximo; i++)
    {
        if (elementos[i].tipoCliente == identificador)
        {
            pos = i;
            i = BSIZE;
        }
    }
    return pos;
}
