#ifndef CONEXIONES_H_
#define CONEXIONES_H_

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

#define IP "127.0.0.1"
#define PUERTO "4444"

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

pthread_mutex_t mutexGeneradorIDMensaje,mutexGeneradorIDSuscriptor = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexColaNewPokemon, mutexColaLocalizedPokemon, mutexColaGetPokemon,mutexColaAppearedPokemon, mutexColaCatchPokemon, mutexColaCaughtPokemon = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexListaSuscriptores = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t no_vacio = PTHREAD_COND_INITIALIZER;

pthread_t thread;
pthread_t hiloConexionSusc;


void iniciar_servidor(void);
void esperar_cliente(int);
int recibir_operacion(int);
void process_request(int cod_op, int cliente_fd);
void serve_client(int *socket);
void* recibir_buffer(int*, int);
void* recibir_mensaje(int socket_cliente, int* size);
void* serializar_paquete(t_paquete* paquete, int bytes);
void devolver_mensaje(void* payload, int size, int socket_cliente);

Suscriptor* recibirSuscripcionNewPokemon(int socket_suscriptor);
void enviarColaNewPokemon(int socket_suscriptor, Suscriptor* unSuscriptor);
Suscriptor* recibirSuscripcionLocalizedPokemon(int socket_suscriptor);
void enviarColaLocalizedPokemon(int socket_suscriptor, Suscriptor* unSuscriptor);
Suscriptor* recibirSuscripcionGetPokemon(int socket_suscriptor);
void enviarColaGetPokemon(int socket_suscriptor,Suscriptor* unSuscriptor);
Suscriptor* recibirSuscripcionAppearedPokemon(int socket_suscriptor);
void enviarColaAppearedPokemon(int socket_suscriptor, Suscriptor* unSuscriptor);
Suscriptor* recibirSuscripcionCatchPokemon(int socket_suscriptor);
void enviarColaCatchPokemon(int socket_suscriptor, Suscriptor* unSuscriptor);
Suscriptor* recibirSuscripcionCaughtPokemon(int socket_suscriptor);
void enviarColaCaughtPokemon(int socket_suscriptor, Suscriptor* unSuscriptor);

NewPokemon* recibir_NEW_POKEMON(int cliente_fd,int* size);
void guardarMensajeNewPokemon(NewPokemon* unNewPokemon);
LocalizedPokemon* recibir_LOCALIZED_POKEMON(int cliente_fd,int* size);
void guardarMensajeLocalizedPokemon(LocalizedPokemon* unLocalizedPokemon);
GetPokemon* recibir_GET_POKEMON(int cliente_fd, int* size);
void guardarMensajeGetPokemon(GetPokemon* unGetPokemon);
AppearedPokemon* recibir_APPEARED_POKEMON(int cliente_fd,int* size);
void guardarMensajeAppearedPokemon(AppearedPokemon* unAppearedPokemon);
CatchPokemon* recibir_CATCH_POKEMON(int cliente_fd,int*size);
void guardarMensajeCatchPokemon(CatchPokemon* unCatchPokemon);
CaughtPokemon* recibir_CAUGHT_POKEMON(int cliente_fd,int* size);
void guardarMensajeCaughtPokemon(CaughtPokemon* unCaughtPokemon);

void* serializarNewPokemon(NewPokemon* newPokemon,int bytes);
void enviarNewPokemon(NewPokemon* unNewPokemon, int socket_subscriptor);
void* serializarLocalizedPokemon(LocalizedPokemon* localizedPokemon,int bytes);
void enviarLocalizedPokemon(LocalizedPokemon* localized_pokemon,int socket_suscriptor);
void* serializarGetPokemon(GetPokemon* getPokemon,int bytes);
void enviarGetPokemon(GetPokemon* get_pokemon,int socket_suscriptor);
void* serializarAppearedPokemon(AppearedPokemon* appearedPokemon,int bytes);
void enviarAppearedPokemon(AppearedPokemon* appeared_pokemon,int socket_suscriptor);
void* serializarCatchPokemon(CatchPokemon* catchPokemon,int bytes);
void enviarCatchPokemon(CatchPokemon* catch_pokemon,int socket_suscriptor);
void* serializarCaughtPokemon(CaughtPokemon* caughtPokemon,int bytes);
void enviarCaughtPokemon(CaughtPokemon* caught_pokemon,int socket_suscriptor);

void liberar_conexion(int socket_cliente);
//--------------------Clientes--------------

int crear_conexion(char* ip, char* puerto);


#endif /* CONEXIONES_H_ */
