/*
Autores: Antonio Junco de Haas, Sergio Hernandez Castillo
Matrículas: A01339695, A01025210
Descripción: Actividad 7 - Conjuntos de señales
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

void subdirectorio(char *);
char * getFullName(char *, struct dirent *);
int remove_directory(const char *);
unsigned char statFileType(char *);

int main(int argc, char * const * argv){
    int dato;
    int tiempo = 0;
    int archivos = 0;
    char * valorDeTiempo;
    char * cantidadDeArchivos;

    while ((dato = getopt(argc, argv, "n:t:")) != -1){
        switch (dato){
            case 'n':
                cantidadDeArchivos = optarg;
                
                if (isdigit(*cantidadDeArchivos) > 0){
                    archivos = atoi(cantidadDeArchivos);

                    if (archivos == 0){
                        printf("Se debe proporcionar una cantidad mayor a 0 para la cantidad de archivos.\n");
                        exit(0);
                    }
                }

                else {
                    printf("Debe proporcionar un valor numérico (o un valor numérico positivo) para la cantidad de archivos.\n");
                    exit(0);
                }
            break;

            case 't':
                valorDeTiempo = optarg;
                
                if (isdigit(*valorDeTiempo) > 0){
                    tiempo = atoi(valorDeTiempo);

                    if (tiempo == 0){
                        printf("Se debe proporcionar una cantidad mayor a 0 para el tiempo del temporizador.\n");
                        exit(0);
                    }
                }

                else {
                    printf("Debe proporcionar un valor numérico (o un valor numérico positivo) para el tiempo del temporizador.\n");
                    exit(0);
                }
            break;

            case '?':
                if (optopt == 'n' || optopt == 't'){
                    fprintf(stderr, "Opción -%c requiere un argumento.\n", optopt);
                }
                    
                else if (isprint (optopt)){
                    fprintf (stderr, "Opción desconocida '-%c'.\n", optopt);
                }
                    
                else {
                    fprintf (stderr, "Opción desconocida '\\x%x'.\n", optopt);
                }
            
                return 1;
            break;

            default:
                abort();
            break;
        }
    }

    if (archivos == 0 && tiempo == 0){
        printf("Debe proporcionar la cantidad de archivos y el tiempo del temporizador.\n");
        return 0;
    }

    printf("Cantidad de archivos: %d\n", archivos);
    printf("Tiempo de temporizador: %d\n", tiempo);

    char * path ="../tc2025-actividad-7-equipo10";
    subdirectorio(path);

    return 0;
}

void subdirectorio(char * basePath){
  char path[1000];
  struct dirent * dp;
  DIR * dir = opendir(basePath);
  int flag = 0;
  
  if (!dir){}

  while ((dp=readdir(dir))!=NULL){
    if (strcmp(dp->d_name, ".")!=0&&strcmp(dp->d_name,"..")){
      //long int a =getFileSize(dp->d_name);

      struct stat st;
      stat(dp->d_name,&st);
      long long int size=st.st_size;
      //int f=st.st_mode;

      char *nombrecompleto=getFullName(basePath, dp);
      //printf("%s: \n", nombrecompleto);
      unsigned char a=statFileType(nombrecompleto);
      if(a==DT_DIR){
        printf("DIRECTORY \n");
        printf("Fullname: %s\n",nombrecompleto);
        printf("name: %s\n",dp->d_name);
        if (strcmp(dp->d_name,"datos")==0) {
          printf("Existe el directorio datos!: %s\n",dp->d_name);
          flag=1;
          //char *delete =strcat(nombrecompleto,"datos");
          printf("Existe el directorio datos, ELIMINANDO %s\n", nombrecompleto);
          rmdir(nombrecompleto);
          remove_directory(nombrecompleto);

        }
      }

      else{
        struct stat data;
        stat(nombrecompleto,&data);
        //printf("FILE ");
        //printf("%lld: %s",(long long)data.st_size, nombrecompleto);
        //printf("nextFile");

        /*Agregar a urna*/
        //vector=revisarUrnas(vector, (long long)data.st_size);

        //printf("\n");
      }
      //printf("%s: \n", dp->d_name);

      //strcpy(path, basePath);
      //strcat(path,"/");
      //strcat(path,dp->d_name);
      //vector=listFilesRecursively(path,vector);
      //closedir(dp);
    }
  }
  if (flag==0){
    printf("NO Existe el directorio datos!\n");
    if (mkdir("datos", 0777) == -1)
    //cerr << "Error :  " << strerror(errno) << endl;
    printf("Error while creating directory\n");

    else
    printf("Directory created\n");
  }
  else{
    printf("Volviendo a crear el directorio!\n");
    if (mkdir("datos", 0777) == -1)
    //cerr << "Error :  " << strerror(errno) << endl;
    printf("Error while creating directory\n");

    else
    printf("Directory created\n");
  }

  //return vector;
  closedir(dir);
}

char *getFullName(char *ruta, struct dirent *ent)
{
  char *nombrecompleto;
  int tmp;

  tmp=strlen(ruta);
  nombrecompleto=malloc(tmp+strlen(ent->d_name)+2); /* Sumamos 2, por el \0 y la barra de directorios (/) no sabemos si falta */
  if (ruta[tmp-1]=='/')
    sprintf(nombrecompleto,"%s%s", ruta, ent->d_name);
  else
    sprintf(nombrecompleto,"%s/%s", ruta, ent->d_name);

  return nombrecompleto;
}

int remove_directory(const char *path) {
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;

   if (d) {
      struct dirent *p;

      r = 0;
      while (!r && (p=readdir(d))) {
          int r2 = -1;
          char *buf;
          size_t len;

          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
             continue;

          len = path_len + strlen(p->d_name) + 2;
          buf = malloc(len);

          if (buf) {
             struct stat statbuf;

             snprintf(buf, len, "%s/%s", path, p->d_name);
             if (!stat(buf, &statbuf)) {
                if (S_ISDIR(statbuf.st_mode))
                   r2 = remove_directory(buf);
                else
                   r2 = unlink(buf);
             }
             free(buf);
          }
          r = r2;
      }
      closedir(d);
   }

   if (!r)
      r = rmdir(path);

   return r;
}

unsigned char statFileType(char *fname)
{
  struct stat sdata;
  /* Intentamos el stat() si no funciona, devolvemos tipo desconocido */unsigned char statFileType(char *fname);
  if (stat(fname, &sdata)==-1)
    {
      return DT_UNKNOWN;
    }

  switch (sdata.st_mode & S_IFMT)
    {
    case S_IFBLK:  return DT_BLK;
    case S_IFCHR:  return DT_CHR;
    case S_IFDIR:  return DT_DIR;
    case S_IFIFO:  return DT_FIFO;
    case S_IFLNK:  return DT_LNK;
    case S_IFREG:  return DT_REG;
    case S_IFSOCK: return DT_SOCK;
    default:       return DT_UNKNOWN;
    }
}