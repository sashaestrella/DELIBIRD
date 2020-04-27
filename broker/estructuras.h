#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<list.h>


typedef struct newPokemon{
	NombrePokemon nombreP;
	CoordenadasXY coordenadas;
	uint32_t cantidad;
}NewPokemon;

typedef struct nombrePokemon{
	char* nombre;
	uint32_t tamanioNombre;
}NombrePokemon;

typedef struct coordenadas{
	uint32_t posicionX;
	uint32_t posicionY;
}CoordenadasXY;


typedef struct localizedPokemon{
	NombrePokemon nombreP;
	ParesOrdenados pares;
}LocalizedPokemon;

typedef struct paresOrdenados{
	uint32_t cantidad;
	t_list* listaParesLocalized;
}ParesOrdenados;

typedef struct getPokemon{
	NombrePokemon nombreP;
}GetPokemon;


typedef struct appearedPokemon{
	NombrePokemon nombreP;
	CoordenadasXY coordenadas;
}AppearedPokemon;

typedef struct catchPokemon{
	NombrePokemon nombreP;
	CoordenadasXY coordenadas;
}CatchPokemon;


typedef struct caughtPokemon{
	uint32_t atrapar;
}CaughtPokemon;


#endif /*ESTRUCTURAS_H_*/



