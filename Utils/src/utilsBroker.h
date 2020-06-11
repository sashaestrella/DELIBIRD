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
#include"utilsGeneral.h"

int generadorDeIDsMensaje = 0;
int generadorDeIDsSuscriptor = 0;

pthread_mutex_t mutexGeneradorIDMensaje,mutexGeneradorIDSuscriptor, mutexListaSuscriptores = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexColaNewPokemon, mutexColaLocalizedPokemon, mutexColaGetPokemon,mutexColaAppearedPokemon, mutexColaCatchPokemon, mutexColaCaughtPokemon = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t no_vacioNP,no_vacioLP,no_vacioGP,no_vacioAP,no_vacioCP,no_vacioCAP = PTHREAD_COND_INITIALIZER;
t_list* New_Pokemon;
t_list* Localized_Pokemon;
t_list* Get_Pokemon;
t_list* Appeared_Pokemon;
t_list* Catch_Pokemon;
t_list* Caught_Pokemon;
t_list* suscriptores_new_pokemon;
t_list* suscriptores_localized_pokemon;
t_list* suscriptores_get_pokemon;
t_list* suscriptores_appeared_pokemon;
t_list* suscriptores_catch_pokemon;
t_list* suscriptores_caught_pokemon;
t_list* listaPosicionesLibres;
void* memoriaInterna; //esto en el futuro va a ser una lista



typedef struct{
	int ID;
	NewPokemon* contenidoDelMensaje; //
	t_list* suscriptoresAtendidos; //suscriptores a los que fue enviado
	t_list* suscriptoresACK; //suscriptores que retornaron ACK del mismo
}MensajeNewPokemon;


typedef struct{
	int ID;
	void* contenidoDelMensaje; //parte de la memoria cache donde esta
	int tamanioEnMemoria;
	t_list* suscriptoresAtendidos; //suscriptores a los que fue enviado
	t_list* suscriptoresACK; //suscriptores que retornaron ACK del mismo
}MensajeNewPokemon2;

typedef struct{

	int ID;
	int IDCorrelativo;
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
	int IDCorrelativo;
	CaughtPokemon* contenidoDelMensaje;
	t_list* suscriptoresAtendidos; //suscriptores a los que fue enviado
	t_list* suscriptoresACK; //suscriptores que retornaron ACK del mismo
}MensajeCaughtPokemon;

typedef struct{
	void* posicion;
	int tamanio;
}PosicionLibre;


void iniciar_servidor(void);
void esperar_cliente(int);
int recibir_operacion(int);
void process_request(int cod_op, int cliente_fd);
void serve_client(int *socket);


void recibirSuscripcionNewPokemon(int socket_suscriptor);
void enviarColaNewPokemon(int idGeneradoEnElMomento,int socket_suscriptor, Suscriptor* unSuscriptor);
void enviarNewPokemonASuscriptores(MensajeNewPokemon2* unMensajeNewPokemon);

Suscriptor* recibirSuscripcionLocalizedPokemon(int socket_suscriptor);
void enviarColaLocalizedPokemon(int idGeneradoEnElMomento,int socket_suscriptor, Suscriptor* unSuscriptor);
void enviarLocalizedPokemonASuscriptores(MensajeLocalizedPokemon* unMensajeLocalizedPokemon);

Suscriptor* recibirSuscripcionGetPokemon(int socket_suscriptor);
void enviarColaGetPokemon(int idGeneradoEnElMomento,int socket_suscriptor,Suscriptor* unSuscriptor);
void enviarGetPokemonASuscriptores(MensajeGetPokemon* unMensajeGetPokemon);

Suscriptor* recibirSuscripcionAppearedPokemon(int socket_suscriptor);
void enviarColaAppearedPokemon(int idGeneradoEnElMomento,int socket_suscriptor, Suscriptor* unSuscriptor);
void enviarAppearedPokemonASuscriptores(MensajeAppearedPokemon* unMensajeAppearedPokemon);

Suscriptor* recibirSuscripcionCatchPokemon(int socket_suscriptor);
void enviarColaCatchPokemon(int idGeneradoEnElMomento,int socket_suscriptor, Suscriptor* unSuscriptor);
void enviarCatchPokemonASuscriptores(MensajeCatchPokemon* unMensajeCatchPokemon);

Suscriptor* recibirSuscripcionCaughtPokemon(int socket_suscriptor);
void enviarColaCaughtPokemon(int idGeneradoEnElMomento,int socket_suscriptor, Suscriptor* unSuscriptor);
void enviarCaughtPokemonASuscriptores(MensajeCaughtPokemon* unMensajeCaughtPokemon);

MensajeNewPokemon2* guardarMensajeNewPokemon(NewPokemon* unNewPokemon);
MensajeLocalizedPokemon* guardarMensajeLocalizedPokemon(LocalizedPokemon* unLocalizedPokemon,int idCorrelativo);
MensajeGetPokemon* guardarMensajeGetPokemon(GetPokemon* unGetPokemon);
MensajeAppearedPokemon* guardarMensajeAppearedPokemon(AppearedPokemon* unAppearedPokemon);
MensajeCatchPokemon* guardarMensajeCatchPokemon(CatchPokemon* unCatchPokemon);
MensajeCaughtPokemon* guardarMensajeCaughtPokemon(CaughtPokemon* unCaughtPokemon,int idCorrelativo);

NewPokemon* sacarDeMemoriaElNewPokemon();

#endif /* CONEXIONES_H_ */
