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
#include<commons/temporal.h>


typedef struct suscriptor{
	int socketSuscriptor;
	int IDsuscriptor;

}Suscriptor;

typedef struct coordenadas{
	uint32_t posicionX;
	uint32_t posicionY;
}CoordenadasXY;



typedef struct newPokemon{
	uint32_t tamanioNombrePokemon;
	char* nombre;
	CoordenadasXY coordenadas;
	uint32_t cantidad;
}NewPokemon;


typedef struct localizedPokemon{
	uint32_t tamanioNombrePokemon;
	char* nombre;
	uint32_t cantidadParesOrdenados;
	t_list* paresOrdenados;
}LocalizedPokemon;

typedef struct getPokemon{
	uint32_t tamanioNombrePokemon;
	char* nombre;
}GetPokemon;


typedef struct appearedPokemon{
	uint32_t tamanioNombrePokemon;
	char* nombre;
	CoordenadasXY coordenadas;
}AppearedPokemon;

typedef struct catchPokemon{
	uint32_t tamanioNombrePokemon;
	char* nombre;
	CoordenadasXY coordenadas;
}CatchPokemon;


typedef struct caughtPokemon{
	uint32_t atrapar;
}CaughtPokemon;



#endif /*ESTRUCTURAS_H_*/



