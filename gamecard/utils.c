/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"

//TODO
/*
 * Recibe un paquete a serializar, y un puntero a un int en el que dejar
 * el tamaño del stream de bytes serializados que devuelve
 */
void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	void* a_enviar = malloc(*bytes); //reservo memoria para el total de lo que se va a enviar
	int offset = 0;

	memcpy(a_enviar, &(paquete->codigo_operacion), sizeof(op_code)); //Lleno el codigo de operacion
	offset += sizeof(op_code);
	memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(int));//Lleno el tamaño
	offset += sizeof(int);
	memcpy(a_enviar + offset, (paquete->buffer->stream), paquete->buffer->size); //Lleno el contenido


	return a_enviar;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

//TODO
void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_buffer* buffer = malloc(sizeof(t_buffer)); //Crea el buffer a llenar con el mensaje

	buffer->size = sizeof(char)* strlen(mensaje) + 1; // El tamaño es la catidad de caracteres + el caracter centinela \0

	void* stream = malloc(buffer->size);		//El tamalo del stream a llenar es el tamaño que dice el buffer, o sea el del mensaje

	memcpy(stream, mensaje, strlen(mensaje) + 1); //Copio el mensaje en el stream

	buffer->stream = stream; //Apunto el stream del buffer al stream que se creo en la linea 59


	t_paquete* paquete = malloc(sizeof(t_paquete)); //Creo el paquete que es el que se va a enviar

	paquete->codigo_operacion = MENSAJE; // Apunto el codigo de operacion a mensaje

	paquete->buffer = buffer; // Y el buffer del paquete al buffer que cree recien

	int bytes = paquete->buffer->size + sizeof(int) + sizeof(op_code);//La cantidad de bytes del paquete es el tamaño de buffer, mas el tamaño de la variable  "size", mas el tamaño del codigo de operacion

	void* a_enviar = serializar_paquete(paquete, &bytes); //Uso la funcion serializar y les paso los bytes por referencia

	send(socket_cliente, a_enviar, paquete->buffer->size + sizeof(int) + sizeof(op_code), 0); //mando e paquete

	free(a_enviar);
	free(buffer->stream);
	free(paquete->buffer);
	free(paquete); //libero todo
}

//TODO

char* recibir_mensaje(int socket_cliente)
{
	char* mensaje;

	t_paquete* paquete = malloc(sizeof(t_paquete));
	t_buffer* buffer = malloc(sizeof(t_buffer));
	paquete->buffer=buffer;
	//creo todo e inicializo puntero a buffer para poder usar recv

	recv(socket_cliente, &(paquete->codigo_operacion), sizeof(op_code),0);
	recv(socket_cliente, &(paquete->buffer->size), sizeof(int), 0);

	void* stream = malloc(paquete->buffer->size);
	paquete->buffer->stream = stream; //Inicializo el puntero de stream para el ultimo recv

	recv(socket_cliente, (paquete->buffer->stream), paquete->buffer->size, 0); // No lleva & porque stream ya es un puntero, en cambio op_code y size no.

	if(paquete->codigo_operacion == MENSAJE){

		mensaje = malloc(paquete->buffer->size); //Reservo memoria para el mensaje

		memcpy(mensaje, (paquete->buffer->stream), paquete->buffer->size); //Copio el contenido a mensaje

		mensaje[paquete->buffer->size] = '\0'; //Agrego centinela en la ultima posicion
	}

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	return mensaje;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
