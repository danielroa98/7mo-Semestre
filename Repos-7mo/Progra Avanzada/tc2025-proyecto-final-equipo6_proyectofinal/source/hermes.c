/*
 *  Proyecto final:
 *      Project Hermes
 * 
 *  Programación Avanzada
 *  Profesor: Vicente Cubells
 * 
 *  Este código fue elaborado por:
 *      Daniel Roa González         - A01021960 @danielroa98
 *      Sergio Hernández Castillo  - A01025210 @sergio-hernandez-castillo
 *      Antonio Junco de Haas       - A01339695 @Tonojunco
 *      Miguel Monterrubio Bandera  - A01022153 @monterrubio-miguel
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#include <strings.h>
#include "mpi.h"

//Maximum number of characters to be read from a file
#define MAXCHAR 100000

/*
 *  Main
 *  Tasked on running Caesar's encoding
 */
int main(int argc, char *argv[])
{
    int myid, nodos;

    // Aquí se inicializa el MPI
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nodos);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    int maxData = MAXCHAR / nodos;

    char localdata[maxData];

    int c;
    char *filepath;

    FILE *fp;
    char content[MAXCHAR];
    char text[MAXCHAR];

    // El nodo master
    if (myid == 0)
    {
        filepath = "../docs/LongerMinCaps.txt";

        fp = fopen(filepath, "r");

        if (fp == NULL)
        {
            printf("Could not open file %s", filepath);
            return 1;
        }

        int counter = 0;

        fread(content, MAXCHAR, 1, fp);
    }

    // Se empieza la división de MPI
    MPI_Scatter(content, maxData, MPI_CHAR, localdata, maxData, MPI_CHAR, 0, MPI_COMM_WORLD);

    int x = maxData;
    int i;
    int key = 5;
    char newText;

//BEGINS PARALLELIZED SECTION IN EVERY MPI NODE
#pragma omp parallel for
    for (i = 0; i < x; i++)
    {
        newText = localdata[i];

        if (newText >= 'a' && newText <= 'z')
        {
            newText = newText + key;

            if (newText > 'z')
            {
                newText = newText - 'z' + 'a' - 1;
            }

            if (newText < 'a')
            {
                newText = newText - 'a' + 'z' + 1;
            }

            localdata[i] = newText;
        }
        else if (newText >= 'A' && newText <= 'Z')
        {
            newText = newText + key;

            if (newText > 'Z')
            {
                newText = newText - 'Z' + 'A' - 1;
            }

            if (newText < 'A')
            {
                newText = newText - 'A' + 'Z' + 1;
            }

            localdata[i] = newText;
        }
    }
    //MPI collects the encrypted text and unifies it
    MPI_Gather(localdata, maxData, MPI_CHAR, content, maxData, MPI_CHAR, 0, MPI_COMM_WORLD);

    //Printing of the encrypted file
    if (myid == 0)
    {

        printf("%s", content);

        fclose(fp);
    }

    //End MPI section
    MPI_Finalize();

    return 0;
}
