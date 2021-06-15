# Computación Gráfica - UPC

## Práctica Calificada 2 (PC2)

2021-1

## Instrucciones

## The Maze

Debe construir un laberinto el cual será generado a partir de una matriz, al 
mejor estilo de los proyectos de programación 1. Exíste una única luz de color
roja que cambia de posición (para simular que es una luz que se prende cuando
el jugador se acerca) cada cierta cantidad de bloques para iluminar el camino
del jugador.

Se le proporciona el archivo maze.h en include/ del repo del curso y un
archivo de prueba llamado mazeTest.cpp en src/pc2 para que pueda probarla.

Puntos | Descripción
-- | --
1 | Generar el laberinto de manera aleatoria usando mkMaze de 20x20 en su prog.
5 | Generar el laberinto en 3D usando OpenGL como todo un profesional.
3 | Usar texturas para simular paredes terroríficas.
4 | Activar la luz de color rojo en distintas posiciones al acercarse.
3 | Activar una luz que cambia de color al llegar al final del laberinto.
2 | Objetos de diferentes materiales al final para apreciar la luz psicodélica.
2 | Elaborar un video de maximo 3 minutos mostrando toda la funcionalidad.

Éste es un ejemplo del un laberinto aleatorio generado por mkMaze

██████████████████████████████████████████
==  ██  ██      ██                      ██
██  ██  ██████  ██████████  ██████████  ██
██  ██  ██  ██  ██  ██  ██          ██  ██
██  ██  ██  ██  ██  ██  ██████  ██████████
██          ██      ██  ██      ██      ██
██████████  ██  ██████  ██  ██  ██████  ██
██  ██              ██      ██  ██  ██  ██
██  ██████████  ██  ██████  ██████  ██  ██
██              ██  ██                  ██
██████████  ██████  ██  ██████  ██████████
██      ██      ██      ██              ██
██  ██  ██  ██████  ██████  ██████  ██████
██  ██          ██  ██  ██      ██      ██
██  ██████  ██  ██████  ██████████████████
██  ██      ██          ██  ██  ██  ██  ██
██████  ██  ██████  ██████  ██  ██  ██  ██
██      ██      ██              ██      ██
██  ██  ██████  ██████  ██  ██  ██  ██████
██  ██  ██      ██      ██  ██          ==    (o) Posicion de luz psicodélica
██████████████████████████████████████████

Sugerencias:
* No es necesario implementar chunking, a menos que le facilite las cosas.
* Se recomienda definir las posiciones en el laberinto donde se prenderán las
	luces, definiendo dichas posiciones en la matriz, por ejemplo con el número 4
	cuando el jugador está a cierta cantidad de bloques de dicha posición, la luz
	se prende mueve a la posición. Tenga cuidado que no existan 2 posiciones que
	podrían activarse al mismo tiempo.
* No se complique y "hardcodee" lo que sea necesario, lo importante es lograr
	el objetivo
* No se rinda, está facilitooo!
