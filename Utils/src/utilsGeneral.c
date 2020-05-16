#include "utilsGeneral.h"
#include "utilsEstructuras.h"
#include "utilsBroker.h"

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

	suscriptores = list_create();
	void* algoARecibir;

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
					list_add(suscriptores,&cliente_fd);
					algoARecibir = solicitudMensaje(newPokemon,cliente_fd);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_LOCALIZEDPOKEMON:
					list_add(suscriptores,&cliente_fd);
					algoARecibir = solicitudMensaje(localizedPokemon,cliente_fd);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_GETPOKEMON:
					list_add(suscriptores,&cliente_fd);
					algoARecibir = solicitudMensaje(getPokemon,cliente_fd);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_APPEAREDPOKEMON:
					list_add(suscriptores,&cliente_fd);
					algoARecibir = solicitudMensaje(appearedPokemon,cliente_fd);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_CATCHPOKEMON:
					list_add(suscriptores,&cliente_fd);
					algoARecibir = solicitudMensaje(catchPokemon,cliente_fd);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_CAUGHTPOKEMON:
					list_add(suscriptores,&cliente_fd);
					algoARecibir = solicitudMensaje(caughtPokemon,cliente_fd);
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
		uint32_t tamanioNombrePokemon;

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&tamanioNombrePokemon,stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		unNewPokemon->nombre = malloc(tamanioNombrePokemon); //aca no iria: strlen(unNewPokemon->nombre) + 1
		memcpy(unNewPokemon->nombre,stream,tamanioNombrePokemon);
		stream+=tamanioNombrePokemon;
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
		pthread_mutex_lock(&mutexID);
		generadorDeIDsMensaje++;
		mensaje->ID = generadorDeIDsMensaje;
		pthread_mutex_unlock(&mutexID);

		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();


		list_add(New_Pokemon,mensaje);


		free(buffer->stream);
		free(buffer);
}



void* recibir_LOCALIZED_POKEMON(int cliente_fd,int* size){

		LocalizedPokemon* localizedPokemon = malloc(sizeof(LocalizedPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));
		uint32_t tamanioNombrePokemon;
		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&tamanioNombrePokemon,stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		localizedPokemon->nombre = malloc(tamanioNombrePokemon); //aca no iria: strlen(unNewPokemon->nombre) + 1
		memcpy(localizedPokemon->nombre,stream,tamanioNombrePokemon);
		stream+=tamanioNombrePokemon;
		puts(localizedPokemon->nombre);
		memcpy(&(localizedPokemon->cantidadParesOrdenados),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		for(int i=0;i<=localizedPokemon->cantidadParesOrdenados*2;i++){
			CoordenadasXY* coordenadas = malloc(sizeof(CoordenadasXY));
			memcpy(&(coordenadas->posicionX),stream,sizeof(uint32_t));
			stream+=sizeof(uint32_t);
			memcpy(&(coordenadas->posicionY),stream,sizeof(uint32_t));
			stream+=sizeof(uint32_t);

			free(coordenadas);
		}





		free(buffer->stream);
		free(buffer);

		return localizedPokemon;
}


void* recibir_GET_POKEMON(int cliente_fd, int* size){

		GetPokemon* getPokemon = malloc(sizeof(GetPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));
		uint32_t tamanioNombrePokemon;
		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&tamanioNombrePokemon,stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		getPokemon->nombre = malloc(tamanioNombrePokemon); //aca no iria: strlen(unNewPokemon->nombre) + 1
		memcpy(getPokemon->nombre,stream,tamanioNombrePokemon);
		stream+=tamanioNombrePokemon;
		puts(getPokemon->nombre);

		list_add(Get_Pokemon,getPokemon);

		free(buffer->stream);
		free(buffer);

		return getPokemon;
}


void* recibir_APPEARED_POKEMON(int cliente_fd,int* size){

		AppearedPokemon* appearedPokemon = malloc(sizeof(AppearedPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));
		uint32_t tamanioNombrePokemon;

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&tamanioNombrePokemon,stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		appearedPokemon->nombre = malloc(tamanioNombrePokemon); //aca no iria: strlen(unNewPokemon->nombre) + 1
		memcpy(appearedPokemon->nombre,stream,tamanioNombrePokemon);
		stream+=tamanioNombrePokemon;
		puts(appearedPokemon->nombre);
		memcpy(&(appearedPokemon->coordenadas.posicionX),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		memcpy(&(appearedPokemon->coordenadas.posicionY),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);

		list_add(Appeared_Pokemon,appearedPokemon);

		free(buffer->stream);
		free(buffer);

		return appearedPokemon;
}


void* recibir_CATCH_POKEMON(int cliente_fd,int*size){

		CatchPokemon* catchPokemon = malloc(sizeof(CatchPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));
		uint32_t tamanioNombrePokemon;

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&tamanioNombrePokemon,stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		catchPokemon->nombre = malloc(tamanioNombrePokemon); //aca no iria: strlen(unNewPokemon->nombre) + 1
		memcpy(catchPokemon->nombre,stream,tamanioNombrePokemon);
		stream+=tamanioNombrePokemon;
		puts(catchPokemon->nombre);
		memcpy(&(catchPokemon->coordenadas.posicionX),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		memcpy(&(catchPokemon->coordenadas.posicionY),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);

		list_add(Appeared_Pokemon,catchPokemon);

		free(buffer->stream);
		free(buffer);

		return catchPokemon;
}

void* recibir_CAUGHT_POKEMON(int cliente_fd,int* size){

		CaughtPokemon* caughtPokemon = malloc(sizeof(CaughtPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&(caughtPokemon->atrapar),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);

		list_add(Caught_Pokemon,caughtPokemon);

		return caughtPokemon;
}


void registrarMensajeEnviado(int cod_op,int socket_suscriptor){




}


void enviarColaNewPokemon(int socket_suscriptor){

		int cantidadDeNewPokemon = list_size(New_Pokemon);
		MensajeNewPokemon* mensaje;
		NewPokemon* unNewPokemon;
		for(int i=0;i<=cantidadDeNewPokemon;i++){
			mensaje = list_get(New_Pokemon,i);
			send(socket_suscriptor,&(mensaje->ID),sizeof(int),0);
			mensaje->contenidoDelMensaje = unNewPokemon;
			enviarNewPokemon(unNewPokemon,socket_suscriptor);
		}




}

void enviarColaLocalizedPokemon(LocalizedPokemon* localizedPokemon,int socket_suscriptor){

		int cantidadDeLocalizedPokemon = list_size(Localized_Pokemon);
		for(int i=0;i<=cantidadDeLocalizedPokemon;i++){
			enviarLocalizedPokemon(localizedPokemon,socket_suscriptor);
			registrarMensajeEnviado(LOCALIZED_POKEMON,socket_suscriptor);
		}

}

void enviarColaGetPokemon(GetPokemon* getPokemon,int socket_suscriptor){

		int cantidadDeGetPokemon = list_size(Get_Pokemon);
		for(int i=0;i<=cantidadDeGetPokemon;i++){
			enviarGetPokemon(getPokemon,socket_suscriptor);
			registrarMensajeEnviado(GET_POKEMON,socket_suscriptor);
		}
}


void enviarColaAppearedPokemon(AppearedPokemon* appearedPokemon,int socket_suscriptor){

		int cantidadDeAppearedPokemon = list_size(Appeared_Pokemon);
		for(int i=0;i<=cantidadDeAppearedPokemon;i++){
			enviarAppearedPokemon(appearedPokemon,socket_suscriptor);
			registrarMensajeEnviado(APPEARED_POKEMON,socket_suscriptor);
		}

}

void enviarColaCatchPokemon(CatchPokemon* catchPokemon,int socket_suscriptor){

		int cantidadDeCatchPokemon = list_size(Catch_Pokemon);
		for(int i=0;i<=cantidadDeCatchPokemon;i++){
			enviarCatchPokemon(catchPokemon,socket_suscriptor);
			registrarMensajeEnviado(CATCH_POKEMON,socket_suscriptor);
		}
}


void enviarColaCaughtPokemon(CaughtPokemon* caughtPokemon,int socket_suscriptor){

		int cantidadDeCaughtPokemon = list_size(Caught_Pokemon);
		for(int i=0;i<=cantidadDeCaughtPokemon;i++){
			enviarCaughtPokemon(caughtPokemon,socket_suscriptor);
			registrarMensajeEnviado(CAUGHT_POKEMON,socket_suscriptor);
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
	for(int i=0;i<=localizedPokemon->cantidadParesOrdenados*2;i++){
		CoordenadasXY* coordenadas = malloc(sizeof(CoordenadasXY));
		memcpy(buffer + desplazamiento,&(coordenadas->posicionX),sizeof(uint32_t));
		desplazamiento+= sizeof(uint32_t);
		memcpy(buffer + desplazamiento,&(coordenadas->posicionX),sizeof(uint32_t));
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

