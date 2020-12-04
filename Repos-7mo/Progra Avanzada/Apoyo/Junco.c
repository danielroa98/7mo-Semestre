#include <stdio.h>
#include <stdlib.h>

#define I 10
#define L 3

typedef struct
{
    char *titulo;
    int paginas;
} Libro;

typedef void (*t_recorre)(void *, size_t, size_t);
void recorre(t_recorre, void *, size_t, size_t);

void *begin(void *vector)
{
    return vector;
}

void *end(void *vector, size_t count, size_t size)
{
    return vector + ((count * size) - size);
}

void *next(void *vector, void *current, size_t count, size_t size)
{
    void *next = current + size;
    void *final = end(vector, count, size);
    if (next > final)
    {
        return NULL;
    }
    return next;
}

void *prev(void *vector, void *current, size_t count, size_t size)
{
    void *prev = current - size;
    void *start = begin(vector);
    if (prev < start)
    {
        return NULL;
    }
    return prev;
}
int main(int arc, const char *argv[])
{
    
}