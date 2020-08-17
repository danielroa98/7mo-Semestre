#include <stdio.h>

enum DiaSemana {
    Domingo,
    Lunes, 
    Martes,
    Miercoles,      
    Jueves,         
    Viernes,
    Sabado
};

/* enum Errores{
    OK = 1,
    WARN = 2,
    INFO = 4,
    CRIT = 8
}; */

int main(int argc, char const *argv[])
{
    enum DiaSemana dia;

    for (dia = Domingo; dia <= Sabado; ++dia)
    {
        printf("%d\n", dia);
    }
    

    return 0;
}