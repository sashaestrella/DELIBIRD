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

pthread_mutex_t mutexBusquedasFallidas = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexMemoriaInterna = PTHREAD_MUTEX_INITIALIZER;
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
t_list* listaPosicionesOcupadas;
void* memoriaInterna;
int tamanioMinimoParticion;
int ip;
int puerto;
char* algoritmoMemoria;
char* algoritmoParticionLibre;
char* algoritmoReemplazo;
int busquedasFallidasPreviasACompactacion;

t_log* logger;
t_config* config;

typedef struct{
	void* posicion;
	int tamanio;
}PosicionLibre;

typedef struct{
	int tamanio;
	void* posicion;
	int colaALaQuePertenece;
	int ID;
}PosicionOcupada;

//Mensajes 2
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
	void* contenidoDelMensaje; //parte de la memoria cache donde esta
	int tamanioEnMemoria;
	t_list* suscriptoresAtendidos; //suscriptores a los que fue enviado
	t_list* suscriptoresACK; //suscriptores que retornaron ACK del mismo
}MensajeLocalizedPokemon2;

typedef struct{
	int ID;
	void* contenidoDelMensaje; //parte de la memoria cache donde esta
	int tamanioEnMemoria;
	t_list* suscriptoresAtendidos; //suscriptores a los que fue enviado
	t_list* suscriptoresACK; //suscriptores que retornaron ACK del mismo
}MensajeGetPokemon2;

typedef struct{
	int ID;
	void* contenidoDelMensaje; //parte de la memoria cache donde esta
	int tamanioEnMemoria;
	t_list* suscriptoresAtendidos; //suscriptores a los que fue enviado
	t_list* suscriptoresACK; //suscriptores que retornaron ACK del mismo
}MensajeAppearedPokemon2;

typedef struct{
	int ID;
	void* contenidoDelMensaje; //parte de la memoria cache donde esta
	int tamanioEnMemoria;
	t_list* suscriptoresAtendidos; //suscriptores a los que fue enviado
	t_list* suscriptoresACK; //suscriptores que retornaron ACK del mismo
}MensajeCatchPokemon2;

typedef struct{
	int ID;
	int IDCorrelativo;
	void* contenidoDelMensaje; //parte de la memoria cache donde esta
	int tamanioEnMemoria;
	t_list* suscriptoresAtendidos; //suscriptores a los que fue enviado
	t_list* suscriptoresACK; //suscriptores que retornaron ACK del mismo
}MensajeCaughtPokemon2;
/*
//Mensajes 1
typedef struct{
	int ID;
	NewPokemon* contenidoDelMensaje; //
	t_list* suscriptoresAtendidos; //suscriptores a los que fue enviado
	t_list* suscriptoresACK; //suscriptores que retornaron ACK del mismo
}MensajeNewPokemon;

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
*/

void iniciar_servidor(void);
void esperar_cliente(int);
int recibir_operacion(int);
void process_request(int cod_op, int cliente_fd);
void serve_client(int *socket);


void recibirSuscripcionNewPokemon(int socket_suscriptor);
void enviarColaNewPokemon(int idGeneradoEnElMomento,int socket_suscriptor, Suscriptor* unSuscriptor);
void enviarNewPokemonASuscriptores(MensajeNewPokemon2* unMensajeNewPokemon);

void recibirSuscripcionLocalizedPokemon(int socket_suscriptor);
void enviarColaLocalizedPokemon(int idGeneradoEnElMomento,int socket_suscriptor, Suscriptor* unSuscriptor);
void enviarLocalizedPokemonASuscriptores(MensajeLocalizedPokemon2* unMensajeLocalizedPokemon);

void recibirSuscripcionGetPokemon(int socket_suscriptor);
void enviarColaGetPokemon(int idGeneradoEnElMomento,int socket_suscriptor,Suscriptor* unSuscriptor);
void enviarGetPokemonASuscriptores(MensajeGetPokemon2* unMensajeGetPokemon);

void recibirSuscripcionAppearedPokemon(int socket_suscriptor);
void enviarColaAppearedPokemon(int idGeneradoEnElMomento,int socket_suscriptor, Suscriptor* unSuscriptor);
void enviarAppearedPokemonASuscriptores(MensajeAppearedPokemon2* unMensajeAppearedPokemon);

void recibirSuscripcionCatchPokemon(int socket_suscriptor);
void enviarColaCatchPokemon(int idGeneradoEnElMomento,int socket_suscriptor, Suscriptor* unSuscriptor);
void enviarCatchPokemonASuscriptores(MensajeCatchPokemon2* unMensajeCatchPokemon);

void recibirSuscripcionCaughtPokemon(int socket_suscriptor);
void enviarColaCaughtPokemon(int idGeneradoEnElMomento,int socket_suscriptor, Suscriptor* unSuscriptor);
void enviarCaughtPokemonASuscriptores(MensajeCaughtPokemon2* unMensajeCaughtPokemon);

MensajeNewPokemon2* guardarMensajeNewPokemon(NewPokemon* unNewPokemon);
MensajeLocalizedPokemon2* guardarMensajeLocalizedPokemon(LocalizedPokemon* unLocalizedPokemon,int idCorrelativo);
MensajeGetPokemon2* guardarMensajeGetPokemon(GetPokemon* unGetPokemon);
MensajeAppearedPokemon2* guardarMensajeAppearedPokemon(AppearedPokemon* unAppearedPokemon);
MensajeCatchPokemon2* guardarMensajeCatchPokemon(CatchPokemon* unCatchPokemon);
MensajeCaughtPokemon2* guardarMensajeCaughtPokemon(CaughtPokemon* unCaughtPokemon,int idCorrelativo);

NewPokemon* sacarDeMemoriaElNewPokemon(MensajeNewPokemon2* mensajeNew2);
LocalizedPokemon* sacarDeMemoriaElLocalizedPokemon(MensajeLocalizedPokemon2* mensajeLocalized2);
GetPokemon* sacarDeMemoriaElGetPokemon(MensajeGetPokemon2* mensajeGet2);
AppearedPokemon* sacarDeMemoriaElAppearedPokemon(MensajeAppearedPokemon2* mensajeAppeared2);
CatchPokemon* sacarDeMemoriaElCatchPokemon(MensajeCatchPokemon2* mensajeCatch2);
CaughtPokemon* sacarDeMemoriaElCaughtPokemon(MensajeCaughtPokemon2* mensajeCaught2);

PosicionLibre* pedirPosicion(int tamanio);
PosicionLibre* pedirPosicionFF(int tamanio);
PosicionLibre* pedirPosicionBF(int tamanio);


#endif /* CONEXIONES_H_ */
