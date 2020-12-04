# Proyecto Final - _Project Hermes_
---
#### Materia: *TC2025-Programación Avanzada*

##### Integrantes:
1. *Daniel Roa González* - *A01021960* 
2. *Sergio Hernández Castillo* - *A01025210* 
3. *Antonio Junco de Haas* - *A01339695* 
4. *Miguel Monterrubio Bandera* - *A01022153* 

---
## 1. Aspectos generales

Las orientaciones de la tarea se encuentran disponibles en la plataforma **Canvas**.

Este documento es una guía sobre qué información debe entregar como parte del proyecto, qué requerimientos técnicos debe cumplir y la estructura que debe seguir para organizar su entrega.

### 1.1 Requerimientos técnicos

A continuación se mencionan los requerimientos técnicos mínimos del proyecto, favor de tenerlos presente para que cumpla con todos.

* Toda la programación debe realizarse en C.
* La solución al problema debe utilizar el máximo nivel de paralelismo posible, utilizando PThreads u OpenMP y MPI.
* La solución debe ejecutarse en un clúster formado como mínimo por el mismo número de nodos que integrantes tenga el equipo.
* Se debe realizar un análisis de los problemas de concurrencia que se pueden presentar y resolverse de la manera correcta, evitando en todo momento las esperas activas.
* Para resolver los problemas de concurrencia se deben utilizar algunas de las técnicas estudiadas en el semestre.
* Todo el código y la documentación del proyecto debe alojarse en este repositorio de GitHub. Favor de mantener la estructura de carpetas propuesta.
* El código debe estar documentado siguiendo los estándares definidos para el lenguaje de programación C.

### 1.2 Estructura del repositorio

El proyecto debe seguir la siguiente estructura de carpetas:
```
- / 			        # Raíz de todo el proyecto
    - README.md			# Archivo con los datos del proyecto (este archivo)
    - source			# Carpeta con el código fuente de la solución
    - docs			    # Carpeta con la documentación del proyecto: PDF, imágenes, etc.
```

### 1.3 Documentación  del proyecto

Como parte de la entrega final del proyecto, se debe incluir la siguiente información:

* Descripción del problema a resolver.
* Análisis exhaustivo de los problemas de concurrencia que se presentan y una explicación clara de cómo se resuelve cada uno de ellos en la solución propuesta.
* Análisis exhauistivo de los inhibidores del paralelismo presentes en el problema planteado y una explicación clara de cómo se resuelve cada uno de ellos en la solución implementada.
* Diagrama con la arquitectura de la solución que incluya: nodos del clúster, componentes de código que se ejecuta en cada nodo, si se hace un *scatter*, *gather*, indicar quien es el nodo maestro y cuáles son los nodos obreros, etc.
* Descripción de cada uno de los componentes de la solución.
* Guía de configuración, compilación y ejecución de la solución implementada y todos los componentes, liberías, etc. que se requieran.

## 2. Descripción del problema

Debido a la pandemia, muchos centros de datos se han vuelto inseguros debido a la cantidad de ciberataques que son llevados a cabo diariamente. 
Cuando _Satoshi Nakamoto_, publicó el ensayo en la que propone y sienta las bases de lo que se convertiría en la base de _Blockchain_. Es por ello que decidimos implementar un clúster de cuatro nodos, para poder simular una encriptación simultánea utilizando un sistema compuesto de diferentes computadoras.

## 3. Solución

Este proyecto plantea la encriptación de documentos .txt utilizando un (_Caesar Encode_).
El nombre del proyecto, _Hérmes_, es derivado por el mensajero de los dioses. Escogimos este nombre debido a que los documentos que se encripten y se corran, serán disponibles en todo momento para la persona que lo encriptó, sin comprometer la confidencialidad de este mismo.

Para poder llevarlo a cabo, decidimos implementar una solución construida con las librerías _OpenMP_ y _MPI_.

__MPI__ nos permite separar la carga de trabajo en diferentes computadoras, de tal manera, esto nos permite dividir la carga y no sobrecargar las computadoras con el procesamiento de cada uno de estos.

### 3.1 Análisis de concurrencia

_Anexado dentro del [PDF](docs/Hermes.pdf)._

### 3.2 Análisis de los inhibidores del paralelismo

_Anexado dentro del [PDF](docs/Hermes.pdf)._

### 3.3 Arquitectura de la solución

_Anexado dentro del [PDF](docs/Hermes.pdf)._

### 3.4 Descripción de los componentes

_Anexado dentro del [PDF](docs/Hermes.pdf)._

## 4. Pasos a seguir para utilizar el proyecto

_Anexado dentro del [PDF](docs/Hermes.pdf)._

### 4.1 Dependencias y requisitos

Las dos librerías significantes que fueron requeridas para la elaboración de la solución a nuestro problema fueron [MPI](https://www.mcs.anl.gov/research/projects/mpi/) y [OpenMP](https://www.openmp.org/). 

* MPI - Utilizado para dividir nuestro arreglo de caracteres entre la cantidad de nodos declarados al correr el archivo de C con la solución, distribuír cada parte a un nodo y unir los caracteres encriptados en el arreglo original.

* OpenMP - Utilizado para paralelizar el ciclo que encripta los caracteres de cada nodo (La cantidad de cores que se utilizan para paralelizar depende de la computadora master donde se corre la solución).

Finalmente, para crear las maquinas virtuales que actuan como nodos y establecer comunicación entre ellos mediante SSH, se requerió [VirtualBox](https://www.virtualbox.org/) y [Vagrant](https://www.vagrantup.com/).

#### 4.1.1 Frameworks

No se manejó ningún framework en este proyecto.

#### 4.1.2 Librerías de funciones o dependencias

* [MPI](https://www.mcs.anl.gov/research/projects/mpi/)
* [OpenMP](https://www.openmp.org/)

#### 4.1.3 Guía de uso

_Anexado dentro del [PDF](docs/Hermes.pdf)._

## 5. Referencias

* [MPI](https://www.mcs.anl.gov/research/projects/mpi/)
* [OpenMP](https://www.openmp.org/)
* [VirtualBox](https://www.virtualbox.org/)
* [Vagrant](https://www.vagrantup.com/)
* [Stackoverflow](https://stackoverflow.com/questions/40080362/how-to-use-mpi-scatter-and-gather-with-array)
* [MPI Tutorial](https://mpitutorial.com/tutorials/mpi-scatter-gather-and-allgather/)
