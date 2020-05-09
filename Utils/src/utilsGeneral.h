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
#include"utilsEstructuras.h"

#define IP "127.0.0.1"
#define PUERTO "4444"

t_list* New_Pokemon;
t_list* Localized_Pokemon;
t_list* Get_Pokemon;
t_list* Appeared_Pokemon;
t_list* Catch_Pokemon;
t_list* Caught_Pokemon;
t_list* suscriptores;

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


pthread_t thread;
pthread_t hiloConexionSusc;

//pthread_mutex_t mutex;

void* recibir_buffer(int*, int);

void iniciar_servidor(void);
void esperar_cliente(int);
void* recibir_mensaje(int socket_cliente, int* size);
void* recibir_NEW_POKEMON(int cliente_fd,int* size);
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
int recibir_operacion(int);
void process_request(int cod_op, int cliente_fd);
void serve_client(int *socket);
void* serializar_paquete(t_paquete* paquete, int bytes);
void* serializarNewPokemon(NewPokemon* newPokemon,int bytes);
void* serializarLocalizedPokemon(LocalizedPokemon* localizedPokemon,int bytes);
void* serializarGetPokemon(GetPokemon* getPokemon,int bytes);
void* serializarAppearedPokemon(AppearedPokemon* appearedPokemon,int bytes);
void* serializarCatchPokemon(CatchPokemon* catchPokemon,int bytes);
void* serializarCaughtPokemon(CaughtPokemon* caughtPokemon,int bytes);
void enviarNewPokemon(NewPokemon* new_pokemon,int size,int socket_suscriptor);
void enviarLocalizedPokemon(LocalizedPokemon* localized_pokemon,int size,int socket_suscriptor);
void enviarGetPokemon(GetPokemon* get_pokemon,int size,int socket_suscriptor);
void enviarAppearedPokemon(AppearedPokemon* appeared_pokemon,int size,int socket_suscriptor);
void enviarCatchPokemon(CatchPokemon* catch_pokemon,int size,int socket_suscriptor);
void enviarCaughtPokemon(CaughtPokemon* caught_pokemon,int size,int socket_suscriptor);
void devolver_mensaje(void* payload, int size, int socket_cliente);

//--------------------Clientes--------------

int crear_conexion(char* ip, char* puerto);


#endif /* CONEXIONES_H_ */
