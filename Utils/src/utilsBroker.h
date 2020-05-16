#ifndef CONEXI_H_
#define CONEXI_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/config.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>
#include"utilsEstructuras.h"

int generadorDeIDsMensaje = 0;
int generadorDeIDsSuscriptor = 0;

typedef struct{
	int ID;
	NewPokemon* contenidoDelMensaje;
	t_list* suscriptoresAtendidos; //suscriptores a los que fue enviado
	t_list* suscriptoresACK; //suscriptores que retornaron ACK del mismo
}MensajeNewPokemon;

typedef struct{
	int ID;
	LocalizedPokemon* contenidoDelMensaje;
	t_list* suscriptoresAtendidos; //suscriptores a los que fue enviado
	t_list* suscriptoresACK; //suscriptores que retornaron ACK del mismo
}MensajeLocalizedPokemon;

typedef struct{
	int ID;
	GetPokemon* contenidoDelMensaje;
	t_list* suscriptoresAtendidos; //suscriptores a los que fue enviado
	t_list* suscriptoresACK; //suscriptores que retornaron ACK del mismo
}MensajeGetPokemon;

typedef struct{
	int ID;
	AppearedPokemon* contenidoDelMensaje;
	t_list* suscriptoresAtendidos; //suscriptores a los que fue enviado
	t_list* suscriptoresACK; //suscriptores que retornaron ACK del mismo
}MensajeAppearedPokemon;

typedef struct{
	int ID;
	CatchPokemon* contenidoDelMensaje;
	t_list* suscriptoresAtendidos; //suscriptores a los que fue enviado
	t_list* suscriptoresACK; //suscriptores que retornaron ACK del mismo
}MensajeCatchPokemon;

typedef struct{
	int ID;
	CaughtPokemon* contenidoDelMensaje;
	t_list* suscriptoresAtendidos; //suscriptores a los que fue enviado
	t_list* suscriptoresACK; //suscriptores que retornaron ACK del mismo
}MensajeCaughtPokemon;


#endif /* CONEXIONES_H_ */
