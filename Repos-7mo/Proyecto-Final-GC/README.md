# Happy Feet 3D Electric Bungaloo

## Descripción
__Happy Feet 3D Electric Bungaloo__ es un juego inspirado en el clásico _Galaga_ con elementos de un tipo _bullet hell_.

El jugador toma el control de una nave con la forma de _Pingu_, la finalidad del juego es poder aguantar una cantidad alta de tiempo mientras se aumenta la puntuación máxima de la partida actual.

Durante el lapso del juego, los jugadores van a tener que enfrentarse contra palomas, gallinas e incluso un T-Rex.

## Controles
Para controlar a _Pingu_, se requiere del uso de las teclas WASD y la barra espaciadora (Space bar).

La combinación de estas teclas permite que Pingu se mueva en diferentes direcciones. 
Desde arriba y abajo, izquierda y derecha y en diagonal.

Al presionar la barra, Pingu comienza a disparar esferas rojas. Cuando colisionan con algún enemigo, desaparecen, tanto los enemigos como los disparos.

## Enemigos
Los enemigos están catalogados por diferentes tipos:

* Palomas (batallones)
    * Se mueven en la misma fila y van descendiendo una vez que tocan otro extremo de la escena.
    * Disparan bolillos que quitan el equivalente de 10 puntos de vida.
    * Se generan 10 palomas cada vez que tocan un extremo.
    * Cuando se destruye una paloma se puede llegar a generar un bolillo para recuperar 10 ptos. de vida.
    * Cuando Pingu toca una paloma, pierde 10 puntos de vida.

* Brayan (Gallina con cuchillo)
    * Se mueven en una sola fila, a diferencia de los _Batallones_, estos no descienden.
    * Son los guardaespaldas del dinosaurio.
    * Cada vez que tocan un extremo de la pantalla, llaman a Kevin para que ataque a Pingu.
    * Resiste dos disparos de Pingu, ya después desaparece.

* Kevin (Gallina con pistola)
    * Se mueven de arriba hacía abajo.
    * Son generadas cuando un Brayan (Gallina con cuchillo), toca un extremo de la línea en la que se encuentra.
    * Disparan cuchillos mientras van descendiendo.
    * No se pueden destruir para evitar que el jugador les dispare desde un lugar estático.

* Gerald (Dinosaurio)
    * Disparan un proyectil llamado Mar1o hacía Pingu.
    * Se le tiene que disparar 3 veces para derrotarlo.

## Gameplay
Para que se cuente con una experiencia única para cada jugador, los enemigos son generados de manera aleatoria al igual que sus disparos.

El jugador deberá cambiar su estrategia constantemente para poder obtener una puntuación máxima.

La cantidad de disparos generados por parte de los enemigos va a depender de la punutación del jugador.
Mientras mas alta, mas enemigos, mas disparos y mas destreza se deberá de tener. 