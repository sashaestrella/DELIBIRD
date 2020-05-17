#include "utilsGeneral.h"
#include "utilsEstructuras.h"
#include "utilsBroker.h"

void iniciar_servidor(void){
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
	int IDsuscriptor;
		switch (cod_op) {
				case MENSAJE:
					algoARecibir = recibir_mensaje(cliente_fd, &size);
					devolver_mensaje(algoARecibir, size, cliente_fd);
					free(algoARecibir);
					break;
				case 0:
					pthread_exit(NULL);
				case -1:
					pthread_exit(NULL);
				case NEW_POKEMON:
					recibir_NEW_POKEMON(cliente_fd, &size);
					free(algoARecibir);
					break;
				case LOCALIZED_POKEMON:
					recibir_LOCALIZED_POKEMON(cliente_fd,&size);
					free(algoARecibir);
					break;
				case GET_POKEMON:
					recibir_GET_POKEMON(cliente_fd,&size);
					free(algoARecibir);
					break;
				case APPEARED_POKEMON:
					recibir_APPEARED_POKEMON(cliente_fd,&size);
					free(algoARecibir);
					break;
				case CATCH_POKEMON:
					recibir_CATCH_POKEMON(cliente_fd,&size);
					free(algoARecibir);
					break;
				case CAUGHT_POKEMON:
					algoARecibir = recibir_CAUGHT_POKEMON(cliente_fd,&size);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_NEWPOKEMON:
					IDsuscriptor = suscribirANewPokemon(cliente_fd);
					enviarColaNewPokemon(cliente_fd,IDsuscriptor);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_LOCALIZEDPOKEMON:
					IDsuscriptor = suscribirALocalizedPokemon(cliente_fd);
					enviarColaLocalizedPokemon(cliente_fd,IDsuscriptor);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_GETPOKEMON:
					IDsuscriptor = suscribirAGetPokemon(cliente_fd);
					enviarColaGetPokemon(cliente_fd,IDsuscriptor);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_APPEAREDPOKEMON:
					IDsuscriptor = suscribirAAppearedPokemon(cliente_fd);
					enviarColaAppearedPokemon(cliente_fd,IDsuscriptor);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_CATCHPOKEMON:
					IDsuscriptor = suscribirACatchPokemon(cliente_fd);
					enviarColaCatchPokemon(cliente_fd,IDsuscriptor);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_CAUGHTPOKEMON:
					IDsuscriptor = suscribirACaughtPokemon(cliente_fd);
					enviarColaCaughtPokemon(cliente_fd,IDsuscriptor);
					free(algoARecibir);
					break;

	}
}



void* recibir_mensaje(int socket_cliente, int* size)
{
	void * buffer;
		recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
		buffer = malloc(*size);
		recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}


void recibir_NEW_POKEMON(int cliente_fd,int* size){

		NewPokemon* unNewPokemon = malloc(sizeof(NewPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&(unNewPokemon->tamanioNombrePokemon),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		unNewPokemon->nombre = malloc(unNewPokemon->tamanioNombrePokemon);
		memcpy(unNewPokemon->nombre,stream,unNewPokemon->tamanioNombrePokemon);
		stream+=unNewPokemon->tamanioNombrePokemon;
		puts(unNewPokemon->nombre);
		memcpy(&(unNewPokemon->coordenadas.posicionX),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		memcpy(&(unNewPokemon->coordenadas.posicionY),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		memcpy(&(unNewPokemon->cantidad),stream,sizeof(uint32_t));
		if(unNewPokemon->cantidad==3){
			puts("y la cantidaaad");
		}

		MensajeNewPokemon* mensaje = malloc(sizeof(MensajeNewPokemon));
		mensaje->contenidoDelMensaje = unNewPokemon;
		pthread_mutex_lock(&mutexGeneradorIDMensaje);
		generadorDeIDsMensaje++;
		mensaje->ID = generadorDeIDsMensaje;
		list_add(New_Pokemon,mensaje);
		pthread_mutex_unlock(&mutexGeneradorIDMensaje);

		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();


		free(buffer->stream);
		free(buffer);
}



void* recibir_LOCALIZED_POKEMON(int cliente_fd,int* size){

		LocalizedPokemon* unLocalizedPokemon = malloc(sizeof(LocalizedPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&(unLocalizedPokemon->tamanioNombrePokemon),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		unLocalizedPokemon->nombre = malloc(unLocalizedPokemon->tamanioNombrePokemon);
		memcpy(unLocalizedPokemon->nombre,stream,unLocalizedPokemon->tamanioNombrePokemon);
		stream+=unLocalizedPokemon->tamanioNombrePokemon;
		puts(unLocalizedPokemon->nombre);
		memcpy(&(unLocalizedPokemon->cantidadParesOrdenados),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		for(int i=0;i<=unLocalizedPokemon->cantidadParesOrdenados*2;i++){
			CoordenadasXY* coordenadas = malloc(sizeof(CoordenadasXY));
			memcpy(&(coordenadas->posicionX),stream,sizeof(uint32_t));
			stream+=sizeof(uint32_t);
			memcpy(&(coordenadas->posicionY),stream,sizeof(uint32_t));
			stream+=sizeof(uint32_t);
			list_add(unLocalizedPokemon->paresOrdenados,coordenadas);
			free(coordenadas);
		}

		MensajeLocalizedPokemon* mensaje = malloc(sizeof(MensajeLocalizedPokemon));
		mensaje->contenidoDelMensaje = unLocalizedPokemon;
		pthread_mutex_lock(&mutexGeneradorIDMensaje);
		generadorDeIDsMensaje++;
		mensaje->ID = generadorDeIDsMensaje;
		list_add(Localized_Pokemon,mensaje);
		pthread_mutex_unlock(&mutexGeneradorIDMensaje);

		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();

		free(buffer->stream);
		free(buffer);

		return unLocalizedPokemon;
}


void* recibir_GET_POKEMON(int cliente_fd, int* size){

		GetPokemon* unGetPokemon = malloc(sizeof(GetPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&(unGetPokemon->tamanioNombrePokemon),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		unGetPokemon->nombre = malloc(unGetPokemon->tamanioNombrePokemon);
		memcpy(unGetPokemon->nombre,stream,unGetPokemon->tamanioNombrePokemon);
		stream+=unGetPokemon->tamanioNombrePokemon;
		puts(unGetPokemon->nombre);

		MensajeGetPokemon* mensaje = malloc(sizeof(MensajeGetPokemon));
		mensaje->contenidoDelMensaje = unGetPokemon;
		pthread_mutex_lock(&mutexGeneradorIDMensaje);
		generadorDeIDsMensaje++;
		mensaje->ID = generadorDeIDsMensaje;
		list_add(Get_Pokemon,mensaje);
		pthread_mutex_unlock(&mutexGeneradorIDMensaje);

		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();

		free(buffer->stream);
		free(buffer);

		return unGetPokemon;
}


void* recibir_APPEARED_POKEMON(int cliente_fd,int* size){

		AppearedPokemon* unAppearedPokemon = malloc(sizeof(AppearedPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&(unAppearedPokemon->tamanioNombrePokemon),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		unAppearedPokemon->nombre = malloc(unAppearedPokemon->tamanioNombrePokemon);
		memcpy(unAppearedPokemon->nombre,stream,unAppearedPokemon->tamanioNombrePokemon);
		stream+=unAppearedPokemon->tamanioNombrePokemon;
		puts(unAppearedPokemon->nombre);
		memcpy(&(unAppearedPokemon->coordenadas.posicionX),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		memcpy(&(unAppearedPokemon->coordenadas.posicionY),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);

		MensajeAppearedPokemon* mensaje = malloc(sizeof(MensajeAppearedPokemon));
		mensaje->contenidoDelMensaje = unAppearedPokemon;
		pthread_mutex_lock(&mutexGeneradorIDMensaje);
		generadorDeIDsMensaje++;
		mensaje->ID = generadorDeIDsMensaje;
		list_add(Appeared_Pokemon,mensaje);
		pthread_mutex_unlock(&mutexGeneradorIDMensaje);

		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();

		free(buffer->stream);
		free(buffer);

		return unAppearedPokemon;
}


void* recibir_CATCH_POKEMON(int cliente_fd,int*size){

		CatchPokemon* unCatchPokemon = malloc(sizeof(CatchPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&(unCatchPokemon->tamanioNombrePokemon),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		unCatchPokemon->nombre = malloc(unCatchPokemon->tamanioNombrePokemon);
		memcpy(unCatchPokemon->nombre,stream,unCatchPokemon->tamanioNombrePokemon);
		stream+=unCatchPokemon->tamanioNombrePokemon;
		puts(unCatchPokemon->nombre);
		memcpy(&(unCatchPokemon->coordenadas.posicionX),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		memcpy(&(unCatchPokemon->coordenadas.posicionY),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);

		MensajeCatchPokemon* mensaje = malloc(sizeof(MensajeCatchPokemon));
		mensaje->contenidoDelMensaje = unCatchPokemon;
		pthread_mutex_lock(&mutexGeneradorIDMensaje);
		generadorDeIDsMensaje++;
		mensaje->ID = generadorDeIDsMensaje;
		list_add(Catch_Pokemon,mensaje);
		pthread_mutex_unlock(&mutexGeneradorIDMensaje);

		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();

		free(buffer->stream);
		free(buffer);

		return unCatchPokemon;
}

void* recibir_CAUGHT_POKEMON(int cliente_fd,int* size){

		CaughtPokemon* unCaughtPokemon = malloc(sizeof(CaughtPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&(unCaughtPokemon->atrapar),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);

		MensajeCaughtPokemon* mensaje = malloc(sizeof(MensajeCaughtPokemon));
		mensaje->contenidoDelMensaje = unCaughtPokemon;
		pthread_mutex_lock(&mutexGeneradorIDMensaje);
		generadorDeIDsMensaje++;
		mensaje->ID = generadorDeIDsMensaje;
		list_add(Caught_Pokemon,mensaje);
		pthread_mutex_unlock(&mutexGeneradorIDMensaje);

		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();

		return unCaughtPokemon;
}




void enviarColaNewPokemon(int socket_suscriptor, int IDsuscriptor){
	puts("llegue a enviarCola np");
		int cantidadDeNewPokemon = list_size(New_Pokemon);
		send(socket_suscriptor,&IDsuscriptor,sizeof(int),0);
		send(socket_suscriptor,&cantidadDeNewPokemon,sizeof(int),0);
		MensajeNewPokemon* mensaje;
		NewPokemon* unNewPokemon;
		mensaje = list_get(New_Pokemon,0);
		for(int i=0;i<=cantidadDeNewPokemon;i++){
			send(socket_suscriptor,&(mensaje->ID),sizeof(int),0);
			pthread_mutex_lock(&mutexGuardarEnviado);

			mensaje = list_get(New_Pokemon,i);
			unNewPokemon = mensaje->contenidoDelMensaje;
			enviarNewPokemon(unNewPokemon,socket_suscriptor);
			list_add(mensaje->suscriptoresAtendidos,&IDsuscriptor);

			pthread_mutex_unlock(&mutexGuardarEnviado);
		}

}



void enviarColaLocalizedPokemon(int socket_suscriptor,int IDsuscriptor){

		int cantidadDeLocalizedPokemon = list_size(Localized_Pokemon);
		send(socket_suscriptor,&IDsuscriptor,sizeof(int),0);
		send(socket_suscriptor,&cantidadDeLocalizedPokemon,sizeof(int),0);
		MensajeLocalizedPokemon* mensaje;
		LocalizedPokemon* unLocalizedPokemon;
		mensaje = list_get(Localized_Pokemon,0);
		for(int i=0;i<=cantidadDeLocalizedPokemon;i++){
			send(socket_suscriptor,&(mensaje->ID),sizeof(int),0);
			pthread_mutex_lock(&mutexGuardarEnviado);

			mensaje = list_get(Localized_Pokemon,i);
			unLocalizedPokemon = mensaje->contenidoDelMensaje;
			enviarLocalizedPokemon(unLocalizedPokemon,socket_suscriptor);
			list_add(mensaje->suscriptoresAtendidos,&IDsuscriptor);

			pthread_mutex_unlock(&mutexGuardarEnviado);
		}
}

void enviarColaGetPokemon(int socket_suscriptor,int IDsuscriptor){

		int cantidadDeGetPokemon = list_size(Get_Pokemon);
		send(socket_suscriptor,&IDsuscriptor,sizeof(int),0);
		send(socket_suscriptor,&cantidadDeGetPokemon,sizeof(int),0);
		MensajeGetPokemon* mensaje;
		GetPokemon* unGetPokemon;
		mensaje = list_get(Get_Pokemon,0);
		for(int i=0;i<=cantidadDeGetPokemon;i++){
			send(socket_suscriptor,&(mensaje->ID),sizeof(int),0);
			pthread_mutex_lock(&mutexGuardarEnviado);

			mensaje = list_get(Get_Pokemon,i);
			unGetPokemon = mensaje->contenidoDelMensaje;
			enviarGetPokemon(unGetPokemon,socket_suscriptor);
			list_add(mensaje->suscriptoresAtendidos,&IDsuscriptor);

			pthread_mutex_unlock(&mutexGuardarEnviado);
		}

}


void enviarColaAppearedPokemon(int socket_suscriptor,int IDsuscriptor){

		int cantidadDeAppearedPokemon = list_size(Appeared_Pokemon);
		send(socket_suscriptor,&IDsuscriptor,sizeof(int),0);
		send(socket_suscriptor,&cantidadDeAppearedPokemon,sizeof(int),0);
		MensajeGetPokemon* mensaje;
		AppearedPokemon* unAppearedPokemon;
		mensaje = list_get(Appeared_Pokemon,0);
		for(int i=0;i<=cantidadDeAppearedPokemon;i++){
			send(socket_suscriptor,&(mensaje->ID),sizeof(int),0);
			pthread_mutex_lock(&mutexGuardarEnviado);

			mensaje = list_get(Appeared_Pokemon,i);
			unAppearedPokemon = mensaje->contenidoDelMensaje;
			enviarAppearedPokemon(unAppearedPokemon,socket_suscriptor);
			list_add(mensaje->suscriptoresAtendidos,&IDsuscriptor);

			pthread_mutex_unlock(&mutexGuardarEnviado);
		}
}

void enviarColaCatchPokemon(int socket_suscriptor,int IDsuscriptor){

		int cantidadDeCatchPokemon = list_size(Catch_Pokemon);
		send(socket_suscriptor,&IDsuscriptor,sizeof(int),0);
		send(socket_suscriptor,&cantidadDeCatchPokemon,sizeof(int),0);
		MensajeGetPokemon* mensaje;
		CatchPokemon* unCatchPokemon;
		mensaje = list_get(Catch_Pokemon,0);
		for(int i=0;i<=cantidadDeCatchPokemon;i++){
			send(socket_suscriptor,&(mensaje->ID),sizeof(int),0);
			pthread_mutex_lock(&mutexGuardarEnviado);

			mensaje = list_get(Catch_Pokemon,i);
			unCatchPokemon = mensaje->contenidoDelMensaje;
			enviarCatchPokemon(unCatchPokemon,socket_suscriptor);
			list_add(mensaje->suscriptoresAtendidos,&IDsuscriptor);

			pthread_mutex_unlock(&mutexGuardarEnviado);
		}
}


void enviarColaCaughtPokemon(int socket_suscriptor,int IDsuscriptor){

		int cantidadDeCaughtPokemon = list_size(Caught_Pokemon);
		send(socket_suscriptor,&IDsuscriptor,sizeof(int),0);
		send(socket_suscriptor,&cantidadDeCaughtPokemon,sizeof(int),0);
		MensajeGetPokemon* mensaje;
		CaughtPokemon* unCaughtPokemon;
		mensaje = list_get(Caught_Pokemon,0);
		for(int i=0;i<=cantidadDeCaughtPokemon;i++){
			send(socket_suscriptor,&(mensaje->ID),sizeof(int),0);
			pthread_mutex_lock(&mutexGuardarEnviado);

			mensaje = list_get(Caught_Pokemon,i);
			unCaughtPokemon = mensaje->contenidoDelMensaje;
			enviarCaughtPokemon(unCaughtPokemon,socket_suscriptor);
			list_add(mensaje->suscriptoresAtendidos,&IDsuscriptor);

			pthread_mutex_unlock(&mutexGuardarEnviado);
		}
}

void* serializar_paquete(t_paquete* paquete, int bytes)
{

	void * a_enviar = malloc(bytes);
	int desplazamiento = 0;

	memcpy(a_enviar + desplazamiento, &(paquete->codigo_operacion), sizeof(op_code));
	desplazamiento+= sizeof(op_code);
	memcpy(a_enviar + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(a_enviar + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;
	return a_enviar;
}


void* serializarNewPokemon(NewPokemon* newPokemon,int bytes){

	void* stream = malloc(bytes);
	uint32_t tamanioNombre = strlen(newPokemon->nombre) + 1;
	int desplazamiento = 0;

	memcpy(stream + desplazamiento,&tamanioNombre,sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(stream + desplazamiento,newPokemon->nombre,tamanioNombre);
	desplazamiento+= tamanioNombre;
	memcpy(stream + desplazamiento,&(newPokemon->coordenadas.posicionX),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(stream + desplazamiento,&(newPokemon->coordenadas.posicionY),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(stream + desplazamiento,&(newPokemon->cantidad),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);


	return stream;
}


void* serializarLocalizedPokemon(LocalizedPokemon* localizedPokemon,int bytes){

	void* buffer = malloc(bytes);
	uint32_t tamanioNombre = strlen(localizedPokemon->nombre) + 1;
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento,&tamanioNombre,sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,localizedPokemon->nombre,tamanioNombre);
	desplazamiento+= tamanioNombre;
	memcpy(buffer + desplazamiento,&(localizedPokemon->cantidadParesOrdenados),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	for(int i=0;i<localizedPokemon->cantidadParesOrdenados;i++){
		CoordenadasXY* coordenadas = list_get(localizedPokemon->paresOrdenados,i);
		memcpy(buffer + desplazamiento,&(coordenadas->posicionX),sizeof(uint32_t));
		desplazamiento+= sizeof(uint32_t);
		memcpy(buffer + desplazamiento,&(coordenadas->posicionY),sizeof(uint32_t));
		desplazamiento+= sizeof(uint32_t);
	}

	return buffer;
}

void* serializarGetPokemon(GetPokemon* getPokemon,int bytes){

	void* buffer = malloc(bytes);
	uint32_t tamanioNombre = strlen(getPokemon->nombre) + 1;
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento,&tamanioNombre,sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,getPokemon->nombre,tamanioNombre);
	desplazamiento+= tamanioNombre;

	return buffer;
}

void* serializarAppearedPokemon(AppearedPokemon* appearedPokemon,int bytes){

	void* buffer = malloc(bytes);
	uint32_t tamanioNombre = strlen(appearedPokemon->nombre) + 1;
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento,&tamanioNombre,sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,appearedPokemon->nombre,tamanioNombre);
	desplazamiento+= tamanioNombre;
	memcpy(buffer + desplazamiento,&(appearedPokemon->coordenadas.posicionX),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,&(appearedPokemon->coordenadas.posicionY),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	return buffer;
}

void* serializarCatchPokemon(CatchPokemon* catchPokemon,int bytes){

	void* buffer = malloc(bytes);
	uint32_t tamanioNombre = strlen(catchPokemon->nombre) + 1;
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento,&tamanioNombre,sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,catchPokemon->nombre,tamanioNombre);
	desplazamiento+= tamanioNombre;
	memcpy(buffer + desplazamiento,&(catchPokemon->coordenadas.posicionX),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,&(catchPokemon->coordenadas.posicionY),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	return buffer;
}

void* serializarCaughtPokemon(CaughtPokemon* caughtPokemon,int bytes){

	void* buffer = malloc(bytes);
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento,&(caughtPokemon->atrapar),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	return buffer;
}

void enviarNewPokemon(NewPokemon* unNewPokemon, int socket_suscriptor){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	uint32_t tamanioNombrePokemon = strlen(unNewPokemon->nombre) +1;
	buffer->size = sizeof(uint32_t) * 4 + tamanioNombrePokemon;
	void* stream = serializarNewPokemon(unNewPokemon,buffer->size);
	buffer->stream = stream;
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = NEW_POKEMON;
	paquete->buffer = buffer;
	int bytes = buffer->size + sizeof(int) + sizeof(op_code);

	void* a_enviar = serializar_paquete(paquete,bytes);

	send(socket_suscriptor, a_enviar, bytes, 0);

	free(a_enviar);
	free(buffer->stream);
	free(paquete->buffer);
	free(paquete);


}



void enviarLocalizedPokemon(LocalizedPokemon* localized_pokemon,int socket_suscriptor){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	int tamanioNombrePokemon = strlen(localized_pokemon->nombre) +1;
	int coordenadas = localized_pokemon->cantidadParesOrdenados * 2;
	buffer->size = sizeof(uint32_t) * 2 * coordenadas + tamanioNombrePokemon;
	void* stream = serializarLocalizedPokemon(localized_pokemon,buffer->size);
	buffer->stream = stream;
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = LOCALIZED_POKEMON;
	paquete->buffer = buffer;
	int bytes = buffer->size + sizeof(int) + sizeof(op_code);

	void* a_enviar = serializar_paquete(paquete,bytes);

	send(socket_suscriptor, a_enviar, bytes, 0);

	free(a_enviar);
	free(buffer->stream);
	free(paquete->buffer);
	free(paquete);

}

void enviarGetPokemon(GetPokemon* get_pokemon,int socket_suscriptor){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	uint32_t tamanioNombrePokemon = strlen(get_pokemon->nombre) +1;
	buffer->size = sizeof(uint32_t) + tamanioNombrePokemon;
	void* stream = serializarGetPokemon(get_pokemon,buffer->size);
	buffer->stream = stream;
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = GET_POKEMON;
	paquete->buffer = buffer;
	int bytes = buffer->size + sizeof(int) + sizeof(op_code);

	void* a_enviar = serializar_paquete(paquete,bytes);

	send(socket_suscriptor, a_enviar, bytes, 0);

	free(a_enviar);
	free(buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviarAppearedPokemon(AppearedPokemon* appeared_pokemon,int socket_suscriptor){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	uint32_t tamanioNombrePokemon = strlen(appeared_pokemon->nombre) +1;
	buffer->size = sizeof(uint32_t) * 3 + tamanioNombrePokemon;
	void* stream = serializarAppearedPokemon(appeared_pokemon,buffer->size);
	buffer->stream = stream;
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = APPEARED_POKEMON;
	paquete->buffer = buffer;
	int bytes = buffer->size + sizeof(int) + sizeof(op_code);

	void* a_enviar = serializar_paquete(paquete,bytes);

	send(socket_suscriptor, a_enviar, bytes, 0);

	free(a_enviar);
	free(buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviarCatchPokemon(CatchPokemon* catch_pokemon,int socket_suscriptor){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	uint32_t tamanioNombrePokemon = strlen(catch_pokemon->nombre) +1;
	buffer->size = sizeof(uint32_t) * 3 + tamanioNombrePokemon;
	void* stream = serializarCatchPokemon(catch_pokemon,buffer->size);
	buffer->stream = stream;
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = CATCH_POKEMON;
	paquete->buffer = buffer;
	int bytes = buffer->size + sizeof(int) + sizeof(op_code);

	void* a_enviar = serializar_paquete(paquete,bytes);

	send(socket_suscriptor, a_enviar, bytes, 0);

	free(a_enviar);
	free(buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviarCaughtPokemon(CaughtPokemon* caught_pokemon,int socket_suscriptor){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t);
	void* stream = serializarCaughtPokemon(caught_pokemon,buffer->size);
	buffer->stream = stream;
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = CAUGHT_POKEMON;
	paquete->buffer = buffer;
	int bytes = buffer->size + sizeof(int) + sizeof(op_code);

	void* a_enviar = serializar_paquete(paquete,bytes);

	send(socket_suscriptor, a_enviar, bytes, 0);

	free(a_enviar);
	free(buffer->stream);
	free(paquete->buffer);
	free(paquete);

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

	void* a_enviar = serializar_paquete(paquete, bytes); //bytes string mensaje

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


int suscribirANewPokemon(int socket_cliente){
	puts("llegue a la parte de suscribir a un tipo");
	Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));
	unSuscriptor->socketSuscriptor = socket_cliente;

	pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
	generadorDeIDsSuscriptor++;
	unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
	list_add(suscriptores_new_pokemon,unSuscriptor);
	pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);

	return unSuscriptor->IDsuscriptor;
}

int suscribirALocalizedPokemon(int socket_cliente){
	Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));

	pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
	generadorDeIDsSuscriptor++;
	unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
	unSuscriptor->socketSuscriptor = socket_cliente;
	list_add(suscriptores_localized_pokemon,unSuscriptor);
	pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);

	return unSuscriptor->IDsuscriptor;
}

int suscribirAGetPokemon(int socket_cliente){
	Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));

	pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
	generadorDeIDsSuscriptor++;
	unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
	unSuscriptor->socketSuscriptor = socket_cliente;
	list_add(suscriptores_get_pokemon,unSuscriptor);
	pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);

	return unSuscriptor->IDsuscriptor;
}

int suscribirAAppearedPokemon(int socket_cliente){
	Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));
	unSuscriptor->socketSuscriptor = socket_cliente;

	pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
	generadorDeIDsSuscriptor++;
	unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
	list_add(suscriptores_appeared_pokemon,unSuscriptor);
	pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);
	send(socket_cliente,&(unSuscriptor->IDsuscriptor), sizeof(int),0);
	puts("te acabo de mandar tu ID maxi");
	return unSuscriptor->IDsuscriptor;
}

int suscribirACatchPokemon(int socket_cliente){
	Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));
	unSuscriptor->socketSuscriptor = socket_cliente;

	pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
	generadorDeIDsSuscriptor++;
	unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
	list_add(suscriptores_catch_pokemon,unSuscriptor);
	pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);

	return unSuscriptor->IDsuscriptor;
}

int suscribirACaughtPokemon(int socket_cliente){
	Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));
	unSuscriptor->socketSuscriptor = socket_cliente;

	pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
	generadorDeIDsSuscriptor++;
	unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
	list_add(suscriptores_caught_pokemon,unSuscriptor);
	pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);

	return unSuscriptor->IDsuscriptor;
}

