/*
 *  Tarea 3
 *  Programación avanzada
 * 
 *  Equipo 9
 *  Daniel Roa
 *  Miguel Monterrubio 
 * 
 *  IMPORTANTE:
 *  ESTE CÓDIGO FUE ELABORADO CON EL APOYO DE SERGIO HERNÁNDEZ, DEBIDO A QUE IGUAL LE TOCÓ
 *  TRABAJAR EN ESTE MISMO EJERCICIO.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{
    int procMando;
    int childId;
    int execTime;
}theChild;

typedef struct helper
{
    theChild * babyYoda;
    struct helper * forward;
}theHelper;

theHelper * head = NULL;
int procAmount = 0;

void enqueue(int contYodas, int contGral, int crono)
{

    theHelper *current = head;

    if (procAmount == 0){
        
        theChild * vL = (theChild*)malloc(sizeof(theChild));

        vL->childId = contYodas;
        vL->procMando = contGral;
        vL->execTime = crono;

        head->babyYoda = vL;
    }
    else{ 
        while (current->forward != NULL){
            current = current->forward;
        }

        theChild *vL = (theChild *)malloc(sizeof(theChild));
        current->forward = malloc(sizeof(theHelper));
        
        vL->childId = contYodas;
        vL->procMando = contGral;
        vL->execTime = crono;

        current->forward->babyYoda = vL;
        current->forward->forward = NULL;
    }
    ++procAmount;
}

theChild * dequeue(){
    theChild * sayonara;

    if(procAmount == 0){
        sayonara = NULL;
    }
    else if (procAmount == 1){
        sayonara = head->babyYoda;
    }else{

        theHelper *nextProc = head->forward;
        sayonara = head->babyYoda;
        free(head);
        head = nextProc;
    }
    --procAmount;
    return sayonara;
}

int main(int argc, char const *argv[]){
    
    head = malloc(sizeof(theHelper));
    head->forward = NULL; 

    //Variables donde se encuentran las cantidades ingresadas
    int cpu = 0, \
    cantProc = 0, \
    cantSub = 0, \
    execTimeIn = 0,\
    yodasEnTotal = 0, \
    order = 0, \
    fries = 0, \
    total = 0,\
    crono = 0, \
    contYodas = -1,\
    cronoQuantum = 5;
    
    //Varbiables para realizar contadores
    int contProc = 0,\
    contSub = 0,\
    cont = 0;

    printf("El quantum de ejecución por default es de %d", cronoQuantum);

    printf("\nIngrese la cantidad de CPUs que va a utilizar: ");
    scanf("%d", &cpu);

    printf("\nIngrese la cantidad de procesos que se van a crear: ");
    scanf("%d", &cantProc);
    printf("\n");

    //Cantidad de procesos
    int * proQ = malloc(sizeof(int) * cantProc);
    int * fin = proQ + cantProc;

    int * i = proQ;
    int contGral = 0;

    while ((i<fin) && (contGral < cantProc))
    {
        printf("Escriba la cantidad de subprocesos que va a tener el proceso %d: ", contGral);
        scanf("%d", &cantSub);

        yodasEnTotal += cantSub;
        *i = yodasEnTotal;

        ++i;
        ++contGral;
    }

    i = proQ;
    contGral = 0;
    
    int fFries = 0;

    for (; order < yodasEnTotal; ++order){
        if (fFries == *i){

            contYodas = 0;
            contProc+=1;
            ++fFries;
            ++i;

        }else{

            ++contYodas;
            ++fFries;

        }
        
        printf("Inserte el tiempo de ejecución del proceso %d y subproceso %d: ", contProc, contYodas);
        scanf("%d", &crono);

        printf("Se está insertando al Round Robin\n");
        printf("\nSub: %d\tProc:%d\tExec:%d\n", contYodas, contProc, crono);
        enqueue(contYodas, contProc, crono);

    }    

    while (procAmount > 0)
    {

        for (int argH = 0; argH < cpu; ++argH){
            if (procAmount > 0){
                theChild * bY = dequeue();

                bY->execTime -= cronoQuantum;

                if (bY->execTime <= 0){
                    printf("\nEl subproceso %d del proceso %d terminó su tiempo de ejecución\n", bY->childId, bY->procMando);
                    free(bY);
                }else if (bY->execTime > 0){

                    printf("\nEl subproceso %d del proceso %d acaba de salir de la cola y entró al CPU %d\n", bY->childId, bY->procMando, argH);
                    enqueue(bY->childId, bY->procMando, bY->execTime);

                }  
                
            }
            
        }
        sleep(cronoQuantum);
        printf("\nFin de quantum\n");
        
    }

    printf("\nFIN DE OPERACIÓN\n");
    free(proQ);
    
    return 0;
}
