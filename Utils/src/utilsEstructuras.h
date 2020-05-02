#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/collections/list.h>

typedef struct paresOrdenados{
	uint32_t cantidad;
	t_list* listaParesLocalized;
}ParesOrdenados;

typedef struct coordenadas{
	uint32_t posicionX;
	uint32_t posicionY;
}CoordenadasXY;

typedef struct newPokemon{
	char* nombre;
	uint32_t tamanioNombre;
	CoordenadasXY coordenadas;
	uint32_t cantidad;
}NewPokemon;


typedef struct localizedPokemon{
	char* nombre;
	uint32_t tamanioNombre;
	ParesOrdenados pares;
}LocalizedPokemon;

typedef struct getPokemon{
	char* nombre;
	uint32_t tamanioNombre;
}GetPokemon;


typedef struct appearedPokemon{
	char* nombre;
	uint32_t tamanioNombre;
	CoordenadasXY coordenadas;
}AppearedPokemon;

typedef struct catchPokemon{
	char* nombre;
	uint32_t tamanioNombre;
	CoordenadasXY coordenadas;
}CatchPokemon;


typedef struct caughtPokemon{
	uint32_t atrapar;
}CaughtPokemon;


#endif /*ESTRUCTURAS_H_*/



