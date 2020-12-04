#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void crawler(char *basePath, long int *max, long int *counter, long int *sizes, long int *mult);
void urner(long int *sizes, long int counter, long int max, long int urn);


int main()
{
    
    long int urn;
    long int mult = 1000;
    long int counter = 0;
    long int max = 0;

    //dynamic array for directory path
    char* path;
    path = (char*)malloc(100 * sizeof(char)); 

    //dynamic array to store file sizes
    long int* sizes;
    sizes = (long int*)malloc(sizeof(long int) * mult);

    // Input path from user
    printf("Enter path to list files: \n");
    scanf("%s", path);

    crawler(path, &max, &counter, sizes, &mult);

    free(path);

    //input urn size from user
    printf("Enter urn size: \n");
    scanf("%ld", &urn);

    urner(sizes, counter, max, urn);

    // for (int i = 0; i < counter; i++)
    // {
    //     printf("%ld\n", sizes[i]);
    // }

    free(sizes);

    return 0;
}

//recursive function to get all file sizes
void crawler(char *basePath, long int *max, long int *counter, long int *sizes, long int *mult)
{
    struct stat buffer;
    int status;
    
    char* nextPath;
    nextPath = (char*)malloc(100 * sizeof(char)); 

    struct dirent *dirent;
    DIR *dir = opendir(basePath);

    // if unable to open, exit
    if (!dir)
        return;

    while ((dirent = readdir(dir)) != NULL)
    {
        if (strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0)
        {
            
            //adds dir name to path for next instance
            strcpy(nextPath, basePath);
            strcat(nextPath, "/");
            strcat(nextPath, dirent->d_name);

            status = stat(nextPath, &buffer);

            //checks if entry is directory and sends crawler through it
            if (S_ISDIR(buffer.st_mode))
            {
                crawler(nextPath, max, counter, sizes, mult);
            }

            //checks if entry is file and saves size to array
            if (S_ISREG(buffer.st_mode))
            {
                printf("Name: %s\nWeight: %ld\n\n", dirent->d_name, buffer.st_size);
                if (buffer.st_size > *max)
                {
                    *max = buffer.st_size;
                }
                sizes[*counter] = buffer.st_size;
                *counter += 1;
                printf("Counter = %ld\nMax: %ld\n\n\n", *counter, *max);

                // if (*counter >= *mult)
                // {   
                //     *mult = *mult * 2;
                //     printf("mult is %ld\n", *mult);
                //     sizes = (long int*)realloc(sizes, *mult);
                //     if (sizes == NULL)
                //     {
                //         printf("ERROR\n");
                //         return;
                //     }
                //     else
                //     {
                //         printf("REALLOC SUCCEEDED\n");
                //     }
                // }
            }

            
        }
    }

    closedir(dir);
    free(nextPath);
}

//sorts the file sizes into urns
void urner(long int *sizes, long int counter, long int max, long int urnSize)
{
    long int urnNum = max/urnSize + 1;
    printf("Number of urns: %ld\n", urnNum);
    int perc;

    int* urns;
    urns = (int*)malloc(sizeof(int) * urnNum);

    //sets urn size to 0
    for (int i = 0; i < urnNum; i++)
    {
        urns[i] = 0;
    }

    //adds to counter for each urn depending on file size
    for (int i = 0; i < counter; ++i)
    {
        urns[sizes[i]/urnSize] += 1;
    }

    //prints histogram
    for (int i = 0; i < urnNum; i++)
    {
        if (urns[i] > 0)
        {
            printf("%7ld   -%7ld    %5d   ", urnSize*(i), urnSize*(i+1)-1, urns[i]);
            perc = urns[i]*100/counter;
            for (int i = 0; i <= perc; ++i)
            {
                printf("*");
            }

            printf("\n");
        }
        else
        {
            printf("%7ld   -%7ld    %5d   \n", urnSize*(i), urnSize*(i+1)-1, urns[i]);
        }
        
    }
    // printf("EMPTY URNS WERE OMITTED\n");
}