/*
 *  Apuntadores a funciones
 * 
 *  Daniel Roa
 *  A01021960
 */
#include <stdio.h>
#include <stdlib.h>

#define N 3

typedef struct Libro
{
    char *titulo;
    int paginas;
} Libro;

typedef void (*d_print)(void *);
typedef void (*t_recorre)(void *, size_t, size_t, d_print);
void recorre(t_recorre, void *, size_t, size_t, d_print);

void *begin(void *valor)
{
    return valor;
}

void *end(void *valor, size_t cont, size_t tamanio)
{
    return (valor + (cont * tamanio) - tamanio);
}

void *next(void *valor, void *sup, size_t cont, size_t tamanio)
{
    void *next = sup + tamanio;
    void *destino = end(valor, cont, tamanio);

    if (next > destino)
    {
        return NULL;
    }
    return next;
}

void *prev(void *valor, void *sup, size_t cont, size_t tamanio)
{
    void *prev = sup - tamanio;
    void *start = begin(valor);

    if (prev < start)
    {
        return NULL;
    }
    return prev;
}

void forward(void *valor, size_t cont, size_t tamanio, d_print a)
{
    void *aux_skip = begin(valor);

    while (aux_skip != NULL){
        (*a)(aux_skip);
        aux_skip = next(valor, aux_skip, cont, tamanio);
    }
}

void backward(void *valor, size_t cont, size_t tamanio, d_print a)
{
    void *aux_ret = end(valor, cont, tamanio);
    while (aux_ret != NULL)
    {
        (*a)(aux_ret);
        aux_ret = prev(valor, aux_ret, cont, tamanio);
    }
}

void bidirectional(void *valor, size_t cont, size_t tamanio, d_print a){

    forward(valor, cont, tamanio, a);
    backward(valor, cont, tamanio, a);
}

    void imprimeInt(int *val)
{
    printf(" %d ", *val);
}

void imprimeLibro(Libro * infoLibro)
{
    printf(" %s \t %d\n", infoLibro->titulo, infoLibro->paginas);
}

void recorre(t_recorre dir, void * valores, size_t cont, size_t tamanio, d_print a){
    (*dir)(valores, cont, tamanio, a);
}

int main(int argc, char const *argv[])
{
    int *numeros = (int *)malloc(N * sizeof(int));

    int *sup = numeros;
    int *end = numeros + N;

    for (; sup < end; ++sup)
    {
        *sup = rand() % 100;
        printf("%d ", *sup);
    }

    printf("\n\nComenzando con los enteros\n\n");

    /* imprimeInt(numeros, N); */

    Libro *tomo = (Libro *)malloc(sizeof(Libro) * N);
    Libro *fin = tomo + N;

    printf("Primero forward\n");
    recorre(&forward, numeros, N, sizeof(*numeros), &imprimeInt);
    printf("\nDespués backward\n");
    recorre(&backward, numeros, N, sizeof(*numeros), &imprimeInt);
    printf("\nY bidireccional\n");
    recorre(&bidirectional, numeros, N, sizeof(*numeros), &imprimeInt);

    printf("\n\nAhora los libros!\n\n");

    for (Libro *aux = tomo; aux < fin; ++aux){
        aux->titulo = (char *)malloc(sizeof(char) * 20);
        printf("\nNombre del libro: ");
        scanf("%s", aux->titulo);

        printf("Número de páginas: ");
        scanf("%d", &aux->paginas);
    }

    printf("\n");

    printf("\nPrimero forward\n");
    recorre(&forward, tomo, N, sizeof(Libro), &imprimeLibro);
    printf("\nDespués backward\n");
    recorre(&backward, tomo, N, sizeof(Libro), &imprimeLibro);
    printf("\nY bidireccional\n");
    recorre(&bidirectional, tomo, N, sizeof(Libro), &imprimeLibro);

    free(numeros);

    for (Libro * adc = tomo; adc < fin; ++adc){
        free(adc->titulo);
    }

    return 0;
}
