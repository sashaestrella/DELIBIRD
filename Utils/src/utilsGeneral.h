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

#define IP "127.0.0.1"
#define PUERTO "4444"

t_list* New_Pokemon;
t_list* Localized_Pokemon;
t_list* Get_Pokemon;
t_list* Appeared_Pokemon;
t_list* Catch_Pokemon;
t_list* Caught_Pokemon;

typedef enum
{
	MENSAJE=1,
	NEW_POKEMON=2,
	LOCALIZED_POKEMON = 3,
	GET_POKEMON = 4,
	APPEARED_POKEMON = 5,
	CATCH_POKEMON = 6,
	CAUGHT_POKEMON = 7,
	SUSCRIPTOR_NEW_P = 8,
	SUSCRIPTOR_LOCALIZED_P = 9,
	SUSCRIPTOR_GET_P = 10,
	SUSCRIPTOR_APPEARED_P = 11,
	SUSCRIPTOR_CATCH_P = 12,
	SUSCRIPTOR_CAUGHT_P = 13
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

void* recibir_buffer(int*, int);

void iniciar_servidor(void);
void esperar_cliente(int);
void* recibir_mensaje(int socket_cliente, int* size);
void* recibir_NEW_POKEMON(int cliente_fd,int* size);
void* recibir_APPEARED_POKEMON(int cliente_fd,int* size);
void* recibir_GET_POKEMON(int cliente_fd, int* size);
void* suscriptorAppeared(int socket_suscriptor);
int recibir_operacion(int);
void process_request(int cod_op, int cliente_fd);
void serve_client(int *socket);
void* serializar_paquete(t_paquete* paquete, int bytes);
void devolver_mensaje(void* payload, int size, int socket_cliente);
NewPokemon* parsearNewPokemon(char* unMensaje);
void enviarNewPokemon(NewPokemon* newPokemon, int conexion);
void validarMensaje();

//--------------------Clientes--------------

int crear_conexion(char* ip, char* puerto);


#endif /* CONEXIONES_H_ */
