#include "utilsGeneral.h"
#include "utilsEstructuras.h"


void iniciar_servidor(void)
{
	int socket_servidor;

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, PUERTO, &hints, &servinfo);

    for (p=servinfo; p != NULL; p = p->ai_next)
    {
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_servidor);
            continue;
        }
        break;
    }

	listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);

    while(1)
    	esperar_cliente(socket_servidor);
}

void esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
	pthread_detach(thread);

}

void serve_client(int* socket)
{
	int cod_op;
	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
		cod_op = -1;
	process_request(cod_op, *socket);
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

void process_request(int cod_op, int cliente_fd) {
	int size;
	void* algoARecibir;

		switch (cod_op) {
		case MENSAJE:
			algoARecibir = recibir_mensaje(cliente_fd, &size);
			devolver_mensaje(algoARecibir, size, cliente_fd);
			free(algoARecibir);
			break;
		case NEW_POKEMON:
			algoARecibir = recibir_NEW_POKEMON(cliente_fd, &size);
			free(algoARecibir);
			break;
		case APPEARED_POKEMON:
			algoARecibir = recibir_APPEARED_POKEMON(cliente_fd,&size);
			free(algoARecibir);
			break;
		case GET_POKEMON:
			algoARecibir = recibir_GET_POKEMON(cliente_fd,&size);
			free(algoARecibir);
			break;
		case SUSCRIPTOR_APPEARED_P:
			algoARecibir = suscriptorAppeared(cliente_fd);
			break;
		case 0:
			pthread_exit(NULL);
		case -1:
			pthread_exit(NULL);
	}
}


void* recibir_NEW_POKEMON(int cliente_fd,int* size){
	int tamanioNombrePokemon;

	recv(cliente_fd,&tamanioNombrePokemon, sizeof(int),0);
	NewPokemon* unNewPokemon = malloc(sizeof(NewPokemon));

	char* nombrePokemon = malloc(tamanioNombrePokemon);
	recv(cliente_fd,nombrePokemon,tamanioNombrePokemon,0);
	unNewPokemon->nombre = nombrePokemon;
	recv(cliente_fd,&(unNewPokemon->coordenadas.posicionX),sizeof(uint32_t),0);
	recv(cliente_fd,&(unNewPokemon->coordenadas.posicionY),sizeof(uint32_t),0);
	recv(cliente_fd,&(unNewPokemon->cantidad),sizeof(uint32_t),0);

	list_add(New_Pokemon,unNewPokemon); //Ver si tiene que ir el & o no


	return unNewPokemon;
}

void* recibir_APPEARED_POKEMON(int cliente_fd,int* size){
	int tamanioNombrePokemon;

	recv(cliente_fd,&tamanioNombrePokemon, sizeof(int),0);
	AppearedPokemon* unAppearedPokemon = malloc(sizeof(AppearedPokemon));
	char* nombrePokemon = malloc(tamanioNombrePokemon);
	recv(cliente_fd,nombrePokemon,tamanioNombrePokemon,0);
	unAppearedPokemon->nombre = nombrePokemon;
	recv(cliente_fd,&(unAppearedPokemon->coordenadas.posicionX),sizeof(uint32_t),0);
	recv(cliente_fd,&(unAppearedPokemon->coordenadas.posicionY),sizeof(uint32_t),0);

	list_add(Appeared_Pokemon,unAppearedPokemon);

	return unAppearedPokemon;
}

/*
void* recibir_LOCALIZED_POKEMON(int cliente_fd,int* size){
	int tamanioNombrePokemon;

	recv(cliente_fd,&tamanioNombrePokemon,sizeof(int),0);
	LocalizedPokemon* unLocalizedPokemon = malloc(sizeof(LocalizedPokemon));
	char* nombrePokemon = malloc(tamanioNombrePokemon);
	unLocalizedPokemon->nombre = nombrePokemon;
	recv(cliente_fd,&(unLocalizedPokemon->pares), sizeof())

}

*/

void* recibir_GET_POKEMON(int cliente_fd, int* size){
	int tamanioNombrePokemon;

	recv(cliente_fd,&tamanioNombrePokemon,sizeof(int),0);
	GetPokemon* unGetPokemon = malloc(sizeof(GetPokemon));
	char* nombrePokemon = malloc(tamanioNombrePokemon);
	recv(cliente_fd,nombrePokemon,tamanioNombrePokemon,0);
	unGetPokemon->nombre = nombrePokemon;

	list_add(Get_Pokemon,unGetPokemon);

	return unGetPokemon;
}

void conectarSuscriptorAppeared(int socket_suscriptor){

}


void* suscriptorAppeared(int socket_suscriptor){

	pthread_t hiloSuscriptorAppeared;

	pthread_create(&hiloSuscriptorAppeared,NULL, (void*)conectarSuscriptorAppeared, NULL);
	pthread_detach(hiloSuscriptorAppeared);


	return 0;
}

void* recibir_mensaje(int socket_cliente, int* size)
{
	void * buffer;
		recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
		buffer = malloc(*size);
		recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}


void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}


void devolver_mensaje(void* payload, int size, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, payload, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);



	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}


void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}

