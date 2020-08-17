#include <stdio.h>
#include <string.h>

typedef struct{
    char nombre[30];
    char apellidos[30];
    int edad;
} persona;

//Optimiza la memoria, cambia el tipo de datos que se está manejando
//Se reserva memoria para el mayor de sus atributos
typedef union{
    char nombre[30];
    char apellidos[30];
    int edad;
} persona_u;

int main(int argc, char const *argv[])
{
    persona e;

    persona_u u;

/*     
    En este caso no se maneja, si se quiere pasar el valor de una cadena a otra, se usa una función llamada strcopy
    e.nombre = "Juan";
 */
    
    strcpy(e.nombre, "Daniel");
    strcpy(e.apellidos, "Roa");
    e.edad = 89;

    printf("Valores en la estructura:\n\n%s %s (%d)\n\n", e.nombre, e.apellidos, e.edad);

    strcpy(u.nombre, "Juan");       //u.nombre = "Juan"
    strcpy(u.apellidos, "Perez");   //u.apellidos = "Perez"
    u.edad = 89;                    //u.edad nombre = apellidos = edad -> 89
                                    //Convierte el 89 en una cadena de caracteres, debido a que se van sobreescribiendo los valores anteriores.

    printf("Valores en la union:\n\n%s %s (%d)\n\n", u.nombre, u.apellidos, u.edad);

    /* "Y Y (89)" -> las Ys son equivalentes a 89 */

    return 0;
}

