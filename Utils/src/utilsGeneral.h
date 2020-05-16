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

pthread_t th1, th2,hiloIDNewPokemon;

pthread_mutex_t mutexGeneradorIDMensaje = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexGeneradorIDSuscriptor = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t ack_ok;

pthread_t thread;
pthread_t hiloConexionSusc;


void* recibir_buffer(int*, int);

void iniciar_servidor(void);
void esperar_cliente(int);
int recibir_operacion(int);
void process_request(int cod_op, int cliente_fd);
void serve_client(int *socket);
void* recibir_mensaje(int socket_cliente, int* size);
void recibir_NEW_POKEMON(int cliente_fd,int* size);
void* recibir_LOCALIZED_POKEMON(int cliente_fd,int* size);
void* recibir_GET_POKEMON(int cliente_fd, int* size);
void* recibir_APPEARED_POKEMON(int cliente_fd,int* size);
void* recibir_CATCH_POKEMON(int cliente_fd,int*size);
void* recibir_CAUGHT_POKEMON(int cliente_fd,int* size);
void* solicMensajeNewPokemon(int socket_suscriptor);
void* solicMensajeLocalizedPokemon(int cliente_fd);
void* solicMensajeGetPokemon(int cliente_fd);
void* solicMensajeAppearedPokemon(int cliente_fd);
void* solicMensajeCatchPokemon(int cliente_fd);
void* solicMensajeCaughtPokemon(int cliente_fd);
void enviarColaNewPokemon(int socket_suscriptor, int IDsuscriptor);
void enviarColaLocalizedPokemon(LocalizedPokemon* localizedPokemon,int socket_suscriptor);
void enviarColaGetPokemon(GetPokemon* getPokemon,int socket_suscriptor);
void enviarColaAppearedPokemon(AppearedPokemon* appearedPokemon,int socket_suscriptor);
void enviarColaCatchPokemon(CatchPokemon* catchPokemon,int socket_suscriptor);
void enviarColaCaughtPokemon(CaughtPokemon* caughtPokemon,int socket_suscriptor);

void* serializar_paquete(t_paquete* paquete, int bytes);
void* serializarNewPokemon(NewPokemon* newPokemon,int bytes);
void* serializarLocalizedPokemon(LocalizedPokemon* localizedPokemon,int bytes);
void* serializarGetPokemon(GetPokemon* getPokemon,int bytes);
void* serializarAppearedPokemon(AppearedPokemon* appearedPokemon,int bytes);
void* serializarCatchPokemon(CatchPokemon* catchPokemon,int bytes);
void* serializarCaughtPokemon(CaughtPokemon* caughtPokemon,int bytes);
void enviarNewPokemon(NewPokemon* unNewPokemon, int socket_subscriptor);
void enviarLocalizedPokemon(LocalizedPokemon* localized_pokemon,int socket_suscriptor);
void enviarGetPokemon(GetPokemon* get_pokemon,int socket_suscriptor);
void enviarAppearedPokemon(AppearedPokemon* appeared_pokemon,int socket_suscriptor);
void enviarCatchPokemon(CatchPokemon* catch_pokemon,int socket_suscriptor);
void enviarCaughtPokemon(CaughtPokemon* caught_pokemon,int socket_suscriptor);
void devolver_mensaje(void* payload, int size, int socket_cliente);
int suscribirANewPokemon(int socket_cliente);
//--------------------Clientes--------------

int crear_conexion(char* ip, char* puerto);


#endif /* CONEXIONES_H_ */
