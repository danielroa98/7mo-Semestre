/*
 *  Proyecto final:
 *      Project Hermes
 * 
 *  Programación Avanzada
 *  Profesor: Vicente Cubells
 * 
 *  Este código fue elaborado por:
 *      Daniel Roa González         - A01021960 @danielroa98
 *      Sergio Hernaández Castillo  - A01025210 @sergio-hernandez-castillo
 *      Antonio Junco de Haas       - A01339695 @Tonojunco
 *      Miguel Monterrubio Bandera  - A01022153 @monterrubio-miguel
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <strings.h>

//Maximum number of characters to be read from a file
#define MAXCHAR 100000

/*
 *  Main
 *  Tasked on running Caesar's encoding
 */
int main(int argc, char *argv[])
{
    char *filepath;

    FILE *fp;
    char content[MAXCHAR];


    filepath = "../docs/LongerMinCaps.txt";
    //filepath = "../docs/output.txt";

    fp = fopen(filepath, "r");

    if (fp == NULL)
    {
        printf("Could not open file %s", filepath);
        return 1;
    }

    int counter = 0;

    fread(content, MAXCHAR, 1, fp);

    int x = MAXCHAR;
    int i;
    int key = -5;
    char newText;

    for (i = 0; i < x; i++)
    {
        newText = content[i];

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

            content[i] = newText;
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

            content[i] = newText;
        }
    }
    //Printing of the encrypted file

    printf("%s", content);

    fclose(fp);

    return 0;
}
