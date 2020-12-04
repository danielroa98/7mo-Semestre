/***********************
 Apuntadores a funciones
 Luis Ortiz Revilla - A01022320
 **************************/
#include <stdio.h>
#include <stdlib.h>

#define I 10
#define N 5

typedef struct
{
  char * titulo;
  int paginas;
} Libro;

typedef void (*t_recorre)(void *, size_t, size_t);
void recorre(t_recorre, void *, size_t, size_t);
typedef void (*d_print)(void *);


void *begin(void *vector){
  return vector;
}

void *end(void *vector, size_t count, size_t size){
  return vector + ((count * size) - size);
}

void *next(void *vector, void *current, size_t count, size_t size){
  void * next = current + size;
  void * final = end(vector, count, size);
  if(next > final) {
    return NULL;
  }
  return next;
}

void *prev(void *vector, void *current, size_t count, size_t size){
  void * prev = current - size;
  void * start = begin(vector);
  if(prev < start) {
    return NULL;
  }
  return prev;
}

void forward(void*vector,size_t count, size_t size, d_print a){
  void * aux_forward=begin(vector);

  while(aux_forward!=NULL){
    (*a)(aux_forward);
    aux_forward=next(vector, aux_forward, count, size);
  }

}

void imprimeInt(int * printthis)
{
    printf(" %d ", *printthis);
}



int main (int arc, const char * argv[])
{
  int * vector = (int *) malloc(N * sizeof(int));

  int * aux = vector;
  int * last = vector + N;

  for (; aux < last; ++aux) {

      *aux = rand() % 100;
      printf(" %d\t ",*aux);
  }
  printf("\n");
  forward(vector,N,sizeof(int),&imprimeInt);
  printf("\n");
}
