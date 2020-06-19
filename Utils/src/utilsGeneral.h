#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/config.h>
#include<string.h>
#include<pthread.h>
#include"utilsEstructuras.h"
#include <stdbool.h>

#define IP "127.0.0.1"
#define PUERTO "4444"

typedef enum
{
	MENSAJE=1,
	NEW_POKEMON=2,
	LOCALIZED_POKEMON = 3,
	GET_POKEMON = 4,
	APPEARED_POKEMON = 5,
	CATCH_POKEMON = 6,
	CAUGHT_POKEMON = 7,
	SUSCRIPTOR_NEWPOKEMON = 8,
	SUSCRIPTOR_LOCALIZEDPOKEMON = 9,
	SUSCRIPTOR_GETPOKEMON = 10,
	SUSCRIPTOR_APPEAREDPOKEMON = 11,
	SUSCRIPTOR_CATCHPOKEMON = 12,
	SUSCRIPTOR_CAUGHTPOKEMON = 13
}op_code;

typedef struct parametrosSuscripcion{
	op_code colaASuscribirse;
	uint32_t nuevoExistente;
}ParametrosSuscripcion;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

pthread_t thread;

void* recibir_buffer(int*, int);
void* recibir_mensaje(int socket_cliente, int* size);
void* serializar_paquete(t_paquete* paquete, int bytes);
void devolver_mensaje(void* payload, int size, int socket_cliente);

NewPokemonConIDs* recibir_NEW_POKEMON(int cliente_fd,int* size,int reciboID);


LocalizedPokemonConIDs* recibir_LOCALIZED_POKEMON(int cliente_fd,int* size,int reciboID);


GetPokemonConIDs* recibir_GET_POKEMON(int cliente_fd, int* size,int reciboID);


AppearedPokemonConIDs* recibir_APPEARED_POKEMON(int cliente_fd,int* size,int reciboID,int idCorrelativo);


CatchPokemonConIDs* recibir_CATCH_POKEMON(int cliente_fd,int*size,int reciboID);


CaughtPokemonConIDs* recibir_CAUGHT_POKEMON(int cliente_fd,int* size,int reciboID);

void* serializarNewPokemon(NewPokemon* newPokemon,int bytes,int id);
void enviarNewPokemon(NewPokemon* unNewPokemon, int socket_suscriptor,int id);

void* serializarLocalizedPokemon(LocalizedPokemon* localizedPokemon,int bytes,int id,int idCorrelativo);
void enviarLocalizedPokemon(LocalizedPokemon* localized_pokemon,int socket_suscriptor,int id,int idCorrelativo);

void* serializarGetPokemon(GetPokemon* getPokemon,int bytes,int id);
void enviarGetPokemon(GetPokemon* get_pokemon,int socket_suscriptor,int id);

void* serializarAppearedPokemon(AppearedPokemon* appearedPokemon,int bytes,int id,int idCorrelativo);
void enviarAppearedPokemon(AppearedPokemon* appeared_pokemon,int socket_suscriptor,int id,int idCorrelativo);

void* serializarCatchPokemon(CatchPokemon* catchPokemon,int bytes,int id);
void enviarCatchPokemon(CatchPokemon* catch_pokemon,int socket_suscriptor,int id);

void* serializarCaughtPokemon(CaughtPokemon* caughtPokemon,int bytes,int id,int idCorrelativo);
void enviarCaughtPokemon(CaughtPokemon* caught_pokemon,int socket_suscriptor,int id,int idCorrelativo);

void enviarSuscripcion(int idSuscriptor,int socket_suscriptor,int cod_op);
void* serializarSuscripcion(int idSuscriptor,int bytes);

AppearedPokemon* parsearAppearedPokemon(char* pokemon, char* posX, char* posY);
NewPokemon* parsearNewPokemon(char* pokemon, char* posicion, char* posicionY, char* cantidad);

void liberar_conexion(int socket_cliente);
//--------------------Clientes--------------

int crear_conexion(char* ip, char* puerto);


#endif /* CONEXIONES_H_ */
