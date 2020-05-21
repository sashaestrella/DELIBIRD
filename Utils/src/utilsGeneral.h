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

pthread_t th1, th2,hiloIDNewPokemon;

pthread_mutex_t mutexGeneradorIDMensaje = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexGeneradorIDSuscriptor = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexColaNewPokemon = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexListaSuscriptores = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t no_vacio = PTHREAD_COND_INITIALIZER;

pthread_t thread;
pthread_t hiloConexionSusc;


void* recibir_buffer(int*, int);

void iniciar_servidor(void);
void esperar_cliente(int);
int recibir_operacion(int);
void process_request(int cod_op, int cliente_fd);
void serve_client(int *socket);
void* recibir_mensaje(int socket_cliente, int* size);
void enviarSuscripcionNewPokemon(int socket);
Suscriptor* recibirSuscripcionNewPokemon(int socket_suscriptor);

NewPokemon* recibir_NEW_POKEMON(int cliente_fd,int* size);
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
void enviarIDNewPokemon(int IDmensaje,int socket_suscriptor);
void enviarIDsuscriptorAsuscriptor(int IDsuscriptor,int socket_suscriptor);
void enviarColaNewPokemon(int socket_suscriptor, Suscriptor* unSuscriptor);
void enviarColaLocalizedPokemon(int socket_suscriptor, int IDsuscriptor);
void enviarColaGetPokemon(int socket_suscriptor, int IDsuscriptor);
void enviarColaAppearedPokemon(int socket_suscriptor, int IDsuscriptor);
void enviarColaCatchPokemon(int socket_suscriptor, int IDsuscriptor);
void enviarColaCaughtPokemon(int socket_suscriptor, int IDsuscriptor);

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
int suscribirALocalizedPokemon(int socket_cliente);
int suscribirAGetPokemon(int socket_cliente);
int suscribirAAppearedPokemon(int socket_cliente);
int suscribirACatchPokemon(int socket_cliente);
int suscribirACaughtPokemon(int socket_cliente);
void liberar_conexion(int socket_cliente);
//--------------------Clientes--------------

int crear_conexion(char* ip, char* puerto);


#endif /* CONEXIONES_H_ */
