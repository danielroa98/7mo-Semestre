/*
 *  Actividad 9
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

#define MUJERES 15
#define HOMBRES 15
#define METODOS 4

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Para la región crítica (baño)
pthread_mutex_t mujer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t hombre = PTHREAD_MUTEX_INITIALIZER;

/* Para Linux */
sem_t * mujeres = MUJERES;
sem_t * hombres = HOMBRES;

/* Para MacOS */
// sem_t *mujeres = MUJERES;
// sem_t *hombres = HOMBRES;

int mujeres_en_espera = 0;
int hombres_en_espera = 0;

int mujeres_que_salen = 0;
int hombres_que_salen = 0;

int mujeres_en_banio = 0;
int hombres_en_banio = 0;

int total = 0;     // Total de personas dentro del baño
int sanitario = 0; // 0 = está vacio, 1 = hay mujeres, 2 = hay hombres

void *mujer_quiere_entrar(void *);
void *hombre_quiere_entrar(void *);
void *mujer_sale(void *);
void *hombre_sale(void *);

int main()
{
    /* Inicializar semáforos en Linux */
    sem_init(&mujeres, 0, MUJERES);
    sem_init(&hombres, 0, HOMBRES);

    /* Para MacOS */
    // mujeres = sem_open("mujeres", O_CREAT, 777, MUJERES);
    // hombres = sem_open("hombres", O_CREAT, 777, HOMBRES);

    // Lista de los 4 threads, un thread por método
    pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * METODOS);

    printf("Sanitario vacio\n");

    // Se crean los threads
    pthread_create(threads, NULL, mujer_quiere_entrar, NULL);
    pthread_create((threads + 1), NULL, hombre_quiere_entrar, NULL);
    pthread_create((threads + 2), NULL, mujer_sale, NULL);
    pthread_create((threads + 3), NULL, hombre_sale, NULL);

    // Se adjuntan los threads
    for (pthread_t *aux = threads; aux < (threads + METODOS); ++aux)
    {
        pthread_join(*aux, NULL);
    }

    free(threads);

    /* Liberar los recursos en Linux */
    sem_destroy(&mujeres);
    sem_destroy(&hombres);

    /* En MacOS*/
    // sem_close(mujeres);
    // sem_close(hombres);

    return 0;
}

// Llega y entra
void *mujer_quiere_entrar(void *id)
{
    srand((unsigned int)time(NULL));
    int i = 0;

    //Cantidad de mujeres que están llegando
    //Solo se checan cada 5 veces
    while (i < MUJERES)
    {
        ++mujeres_en_espera;
        printf("-> Llega una mujer (%d en espera).\n", mujeres_en_espera);

        sem_wait(&mujeres);
        pthread_mutex_lock(&mutex); // Cierra la región crítica
        //printf("\nABRE ZONA CRÍTICA 1\n");

        if ((sanitario == 0) || (mujeres_en_banio >= 1))
        {
            --mujeres_en_espera;

            printf("-> Entra una mujer (%d en espera).\n", mujeres_en_espera);

            ++mujeres_en_banio;
            ++total;

            if (sanitario == 0)
            {
                printf("SANITARIO OCUPADO POR MUJERES\n");
            }

            sanitario = 1;

            int numero = (rand() % 3000) + 5000; // Rango: 5 - 8
            // printf("Tiempo para soltar diarrea explosiva: %d\n", numero);
            usleep(numero);
        }

        ++i;

        //printf("\nCIERRA ZONA CRÍTICA 1\n");
        pthread_mutex_unlock(&mutex); // Abre la región crítica
        sem_post(&mujeres);

    }
    
    //While que va comprobar si se puede 
    while(mujeres_en_espera > 0){
        sem_wait(&mujeres);
        pthread_mutex_lock(&mutex); // Cierra la región crítica
        //printf("\nABRE ZONA CRÍTICA 1\n");

        if ((sanitario == 0) || (mujeres_en_banio >= 1))
        {
            --mujeres_en_espera;

            printf("-> Entra una mujer (%d en espera).\n", mujeres_en_espera);

            ++mujeres_en_banio;
            ++total;

            if (sanitario == 0)
            {
                printf("SANITARIO OCUPADO POR MUJERES\n");
            }

            sanitario = 1;

            int numero = (rand() % 3000) + 5000; // Rango: 5 - 8
            // printf("Tiempo para soltar diarrea explosiva: %d\n", numero);
            usleep(numero);
        }

        ++i;

        //printf("\nCIERRA ZONA CRÍTICA 1\n");
        pthread_mutex_unlock(&mutex); // Abre la región crítica
        sem_post(&mujeres);
    }

    pthread_exit(NULL);
}

// Solo sale
void *mujer_sale(void *id)
{
    srand((unsigned int)time(NULL));
    int i = 0;

    while (i < MUJERES)
    {
        sem_wait(&mujeres);
        pthread_mutex_lock(&mutex); // Se cierra la región crítica
        //printf("\nABRE ZONA CRÍTICA 1.1\n");

        int numero = (rand() % 1000) + 7000; // Rango: 1 - 7
        usleep(numero);

        if (mujeres_en_banio >= 1)
        {
            printf("Sale una mujer.->\n");

            --mujeres_en_banio;
            ++mujeres_que_salen;
            --total;
            ++i;

            if (mujeres_en_banio == 0)
            {
                sanitario = 0;
                printf("Sanitario vacio.\n");
            }
        }

        //printf("\nCIERRA ZONA CRÍTICA 1.1\n");
        pthread_mutex_unlock(&mutex); // Se abre la región crítica
        sem_post(&mujeres);
    }

    pthread_exit(NULL);
}

// Llega y entra
void *hombre_quiere_entrar(void *id)
{
    srand((unsigned int)time(NULL));
    int i = 0;

    int contHombres_enter = 0;

    while (contHombres_enter < HOMBRES)
    {
        ++hombres_en_espera;
        printf("-> Llega un hombre (%d en espera).\n", hombres_en_espera);

        sem_wait(&hombres);
        pthread_mutex_lock(&mutex); // Cierra la región crítica
        //printf("\nABRE ZONA CRÍTICA 2\n");

        //printf("\nAfuera del if de hombre_quiere_entrar\nEl valor de sanitario es de: %d\nEl valor de hombres_en_banio es de %d\n", sanitario, hombres_en_banio);

        if ((sanitario == 0) || (hombres_en_banio >= 1))
        {
            //printf("\nDentro del if de hombre_quiere_entrar\n");

            --hombres_en_espera;

            printf("-> Entra un hombre (%d en espera).\n", hombres_en_espera);

            hombres_en_banio += 1;
            ++total;

            if (sanitario == 0)
            {
                printf("SANITARIO OCUPADO POR HOMBRES\n");
            }

            sanitario = 2;

            int numero = (rand() % 3000) + 5000; // Rango: 5 - 8
            // printf("Tiempo para soltar diarrea explosiva: %d\n", numero);
            usleep(numero);
        }

        //++i;
        contHombres_enter += 1;

        //printf("\nCIERRA ZONA CRÍTICA 2\tValor actual de contHombres_enter: %d\n", contHombres_enter);
        pthread_mutex_unlock(&mutex); // Abre la región crítica
        sem_post(&hombres);
    }

    while(hombres_en_espera > 0){
        sem_wait(&hombres);
        pthread_mutex_lock(&mutex); // Cierra la región crítica
        //printf("\nABRE ZONA CRÍTICA 2\n");

        //printf("\nAfuera del if de hombre_quiere_entrar\nEl valor de sanitario es de: %d\nEl valor de hombres_en_banio es de %d\n", sanitario, hombres_en_banio);

        if ((sanitario == 0) || (hombres_en_banio >= 1))
        {
            //printf("\nDentro del if de hombre_quiere_entrar\n");

            --hombres_en_espera;

            printf("-> Entra un hombre (%d en espera).\n", hombres_en_espera);

            hombres_en_banio += 1;
            ++total;

            if (sanitario == 0)
            {
                printf("SANITARIO OCUPADO POR HOMBRES\n");
            }

            sanitario = 2;

            int numero = (rand() % 3000) + 5000; // Rango: 5 - 8
            // printf("Tiempo para soltar diarrea explosiva: %d\n", numero);
            usleep(numero);
        }

        //++i;

        //printf("\nCIERRA ZONA CRÍTICA 2\tValor actual de contHombres_enter: %d\n", contHombres_enter);
        pthread_mutex_unlock(&mutex); // Abre la región crítica
        sem_post(&hombres);
    }

    pthread_exit(NULL);
}

// Solo sale
void *hombre_sale(void *id)
{
    srand((unsigned int)time(NULL));
    int i = 0;

    while (i < HOMBRES)
    {
        sem_wait(&hombres);
        //printf("\nFUERA DE LA ZONA CRÍTICA 2.1\nCantidad de hombres en el baño: %d\n", hombres_en_banio);
        pthread_mutex_lock(&mutex); // Se cierra la región crítica

        //printf("DENTRO DE LA REGIÓN CRÍTICA 2.1\nValor actual de hombres_en_banio es %d\n", hombres_en_banio);

        int numero = (rand() % 1000) + 7000; // Rango: 1 - 7
        usleep(numero);

        if (hombres_en_banio >= 1)
        {
            printf("Sale un hombre.->\n");

            --hombres_en_banio;
            ++hombres_que_salen;
            --total;
            ++i;

            //printf("Valor actual de i es: %d", i);

            if (hombres_en_banio == 0)
            {
                sanitario = 0;
                printf("Sanitario vacio.\n");
            }
        }

        //printf("\nCIERRA ZONA CRÍTICA 2.1\nCantidad de hombres en el baño: %d\n", i);
        pthread_mutex_unlock(&mutex); // Se abre la región crítica
        sem_post(&hombres);
    }

    pthread_exit(NULL);
}
