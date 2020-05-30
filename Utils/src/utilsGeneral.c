#include "utilsGeneral.h"


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
	Suscriptor* elQueSeMeSuscribe;
	void* algoARecibir;
	NewPokemon* unNewPokemon;
	LocalizedPokemon* unLocalizedPokemon;
	GetPokemon* unGetPokemon;
	AppearedPokemon* unAppearedPokemon;
	CatchPokemon* unCatchPokemon;
	CaughtPokemon* unCaughtPokemon;
	LocalizedPokemonConIDCorrelativo* unLocalizedPokemonConIDCorrelativo;
	CaughtPokemonConIDCorrelativo* unCaughtPokemonConIDCorrelativo;
	int id;
	int idCorrelativo;

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
					unNewPokemon = recibir_NEW_POKEMON(cliente_fd, &size,0);
					id = guardarMensajeNewPokemon(unNewPokemon);
					enviarNewPokemonASuscriptores(unNewPokemon,id);
					break;
				case LOCALIZED_POKEMON:
					unLocalizedPokemonConIDCorrelativo = recibir_LOCALIZED_POKEMON(cliente_fd,&size);
					unLocalizedPokemon = unLocalizedPokemonConIDCorrelativo->localizedPokemon;
					idCorrelativo = unLocalizedPokemonConIDCorrelativo->IDCorrelativo;
					free(unLocalizedPokemonConIDCorrelativo);
					guardarMensajeLocalizedPokemon(unLocalizedPokemon,idCorrelativo);
					enviarLocalizedPokemonASuscriptores(unLocalizedPokemon,idCorrelativo);
					break;
				case GET_POKEMON:
					unGetPokemon = recibir_GET_POKEMON(cliente_fd,&size,0);
					id = guardarMensajeGetPokemon(unGetPokemon);
					respoderConIDAlTeam(id,cliente_fd);
					enviarGetPokemonASuscriptores(unGetPokemon,id);
					break;
				case APPEARED_POKEMON:
					unAppearedPokemon = recibir_APPEARED_POKEMON(cliente_fd,&size,1);
					guardarMensajeAppearedPokemon(unAppearedPokemon);
					enviarAppearedPokemonASuscriptores(unAppearedPokemon);
					break;
				case CATCH_POKEMON:
					unCatchPokemon = recibir_CATCH_POKEMON(cliente_fd,&size,0);
					id = guardarMensajeCatchPokemon(unCatchPokemon);
					respoderConIDAlTeam(id,cliente_fd);
					enviarCatchPokemonASuscriptores(unCatchPokemon,id);
					break;
				case CAUGHT_POKEMON:
					unCaughtPokemonConIDCorrelativo = recibir_CAUGHT_POKEMON(cliente_fd,&size);
					unCaughtPokemon = unCaughtPokemonConIDCorrelativo->caughtPokemon;
					idCorrelativo = unCaughtPokemonConIDCorrelativo->IDCorrelativo;
					free(unCaughtPokemonConIDCorrelativo);
					guardarMensajeCaughtPokemon(unCaughtPokemon,idCorrelativo);
					enviarCaughtPokemonASuscriptores(unCaughtPokemon,idCorrelativo);
					break;
				case SUSCRIPTOR_NEWPOKEMON:
					elQueSeMeSuscribe =	recibirSuscripcionNewPokemon(cliente_fd);
					enviarColaNewPokemon(cliente_fd,elQueSeMeSuscribe);
					break;
				case SUSCRIPTOR_LOCALIZEDPOKEMON:
					elQueSeMeSuscribe =	recibirSuscripcionLocalizedPokemon(cliente_fd);
					enviarColaLocalizedPokemon(cliente_fd,elQueSeMeSuscribe);
					break;
				case SUSCRIPTOR_GETPOKEMON:
					elQueSeMeSuscribe = recibirSuscripcionGetPokemon(cliente_fd);
					enviarColaGetPokemon(cliente_fd,elQueSeMeSuscribe);
					break;
				case SUSCRIPTOR_APPEAREDPOKEMON:
					elQueSeMeSuscribe = recibirSuscripcionAppearedPokemon(cliente_fd);
					enviarColaAppearedPokemon(cliente_fd,elQueSeMeSuscribe);
					break;
				case SUSCRIPTOR_CATCHPOKEMON:
					elQueSeMeSuscribe = recibirSuscripcionCatchPokemon(cliente_fd);
					enviarColaCatchPokemon(cliente_fd,elQueSeMeSuscribe);
					break;
				case SUSCRIPTOR_CAUGHTPOKEMON:
					elQueSeMeSuscribe = recibirSuscripcionCaughtPokemon(cliente_fd);
					enviarColaCaughtPokemon(cliente_fd,elQueSeMeSuscribe);
					break;


	}
}

void enviarCaughtPokemonASuscriptores(CaughtPokemon* unCaughtPokemon,int idCorrelativo){
		Suscriptor* unSuscriptor;
		int tamanioSuscriptoresCaughtPokemon = list_size(suscriptores_caught_pokemon);
		for(int i=0;i<tamanioSuscriptoresCaughtPokemon;i++){
			unSuscriptor = list_get(suscriptores_caught_pokemon,i);
			enviarCaughtPokemon(unCaughtPokemon,unSuscriptor->socketSuscriptor,idCorrelativo);
		}
}

void enviarCatchPokemonASuscriptores(CatchPokemon* unCatchPokemon,int id){
		Suscriptor* unSuscriptor;
		int tamanioSuscriptoresCatchPokemon = list_size(suscriptores_catch_pokemon);
		for(int i=0;i<tamanioSuscriptoresCatchPokemon;i++){
			unSuscriptor = list_get(suscriptores_catch_pokemon,i);
			enviarCatchPokemon(unCatchPokemon,unSuscriptor->socketSuscriptor,id);
		}
}

void enviarLocalizedPokemonASuscriptores(LocalizedPokemon* unLocalizedPokemon,int idCorrelativo){
		Suscriptor* unSuscriptor;
		int tamanioSuscriptoresLocalizedPokemon = list_size(suscriptores_localized_pokemon);
		for(int i=0;i<tamanioSuscriptoresLocalizedPokemon;i++){
			unSuscriptor = list_get(suscriptores_localized_pokemon,i);
			enviarLocalizedPokemon(unLocalizedPokemon,unSuscriptor->socketSuscriptor,idCorrelativo);
		}
}

void respoderConIDAlTeam(int id,int cliente_fd){
		send(cliente_fd,&id,sizeof(int),MSG_WAITALL);
}

void enviarGetPokemonASuscriptores(GetPokemon* unGetPokemon,int id){
		Suscriptor* unSuscriptor;
		int tamanioSuscriptoresGetPokemon = list_size(suscriptores_get_pokemon);

		for(int i=0;i<tamanioSuscriptoresGetPokemon;i++){
			unSuscriptor = list_get(suscriptores_get_pokemon,i);
			enviarGetPokemon(unGetPokemon,unSuscriptor->socketSuscriptor,id);
		}

}

void enviarAppearedPokemonASuscriptores(AppearedPokemon* unAppearedPokemon){
		Suscriptor* unSuscriptor;
		int tamanioSuscriptoresAppearedPokemon = list_size(suscriptores_appeared_pokemon);

		for(int i=0;i<tamanioSuscriptoresAppearedPokemon;i++){
			unSuscriptor = list_get(suscriptores_appeared_pokemon,i);
			enviarAppearedPokemon(unAppearedPokemon,unSuscriptor->socketSuscriptor,0);
		}
}

void enviarNewPokemonASuscriptores(NewPokemon* unNewPokemon,int id){
		Suscriptor* unSuscriptor;
		int tamanioSuscriptoresNewPokemon = list_size(suscriptores_new_pokemon);

		for(int i=0;i<tamanioSuscriptoresNewPokemon;i++){
			unSuscriptor = list_get(suscriptores_new_pokemon,i);
			enviarNewPokemon(unNewPokemon,unSuscriptor->socketSuscriptor,id);
		}
}

Suscriptor* recibirSuscripcionCaughtPokemon(int socket_suscriptor){

		Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));

		printf("Recibi solicitud de suscripcion para CaughtPokemon de socket: %d\n",socket_suscriptor);

		pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
		generadorDeIDsSuscriptor++;
		unSuscriptor->socketSuscriptor = socket_suscriptor;
		unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
		pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);
		pthread_mutex_lock(&mutexListaSuscriptores);
		list_add(suscriptores_caught_pokemon,unSuscriptor);
		pthread_mutex_unlock(&mutexListaSuscriptores);

		printf("El tamaño de la lista de suscriptores de GP ahora es de: %d\n",list_size(suscriptores_caught_pokemon));

		printf("El suscriptor %d con id = %d fue agregago a la lista get pokemon\n",socket_suscriptor,generadorDeIDsSuscriptor);
		send(socket_suscriptor,&(unSuscriptor->IDsuscriptor),sizeof(int),0);

		return unSuscriptor;
}

Suscriptor* recibirSuscripcionCatchPokemon(int socket_suscriptor){

		Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));

		printf("Recibi solicitud de suscripcion para CatchPokemon de socket: %d\n",socket_suscriptor);

		pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
		generadorDeIDsSuscriptor++;
		unSuscriptor->socketSuscriptor = socket_suscriptor;
		unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
		pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);
		pthread_mutex_lock(&mutexListaSuscriptores);
		list_add(suscriptores_catch_pokemon,unSuscriptor);
		pthread_mutex_unlock(&mutexListaSuscriptores);

		printf("El tamaño de la lista de suscriptores de CP ahora es de: %d\n",list_size(suscriptores_catch_pokemon));

		printf("El suscriptor %d con id = %d fue agregago a la lista catch pokemon\n",socket_suscriptor,generadorDeIDsSuscriptor);
		send(socket_suscriptor,&(unSuscriptor->IDsuscriptor),sizeof(int),0);

		return unSuscriptor;
}

Suscriptor* recibirSuscripcionAppearedPokemon(int socket_suscriptor){

		Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));

		printf("Recibi solicitud de suscripcion para AppearedPokemon de socket: %d\n",socket_suscriptor);

		pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
		generadorDeIDsSuscriptor++;
		unSuscriptor->socketSuscriptor = socket_suscriptor;
		unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
		pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);
		pthread_mutex_lock(&mutexListaSuscriptores);
		list_add(suscriptores_appeared_pokemon,unSuscriptor);
		pthread_mutex_unlock(&mutexListaSuscriptores);

		printf("El tamaño de la lista de suscriptores de GP ahora es de: %d\n",list_size(suscriptores_appeared_pokemon));

		printf("El suscriptor %d con id = %d fue agregago a la lista appeared pokemon\n",socket_suscriptor,generadorDeIDsSuscriptor);
		send(socket_suscriptor,&(unSuscriptor->IDsuscriptor),sizeof(int),0);

		return unSuscriptor;
}

Suscriptor* recibirSuscripcionGetPokemon(int socket_suscriptor){

		Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));

		printf("Recibi solicitud de suscripcion para GetPokemon de socket: %d\n",socket_suscriptor);

		pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
		generadorDeIDsSuscriptor++;
		unSuscriptor->socketSuscriptor = socket_suscriptor;
		unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
		pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);
		pthread_mutex_lock(&mutexListaSuscriptores);
		list_add(suscriptores_get_pokemon,unSuscriptor);
		pthread_mutex_unlock(&mutexListaSuscriptores);

		printf("El tamaño de la lista de suscriptores de GP ahora es de: %d\n",list_size(suscriptores_get_pokemon));

		printf("El suscriptor %d con id = %d fue agregago a la lista get pokemon\n",socket_suscriptor,generadorDeIDsSuscriptor);
		send(socket_suscriptor,&(unSuscriptor->IDsuscriptor),sizeof(int),0);

		return unSuscriptor;
}


Suscriptor* recibirSuscripcionLocalizedPokemon(int socket_suscriptor){

		Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));

		printf("Recibi solicitud de suscripcion para LocalizedPokemon de socket: %d\n",socket_suscriptor);

		pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
		generadorDeIDsSuscriptor++;
		unSuscriptor->socketSuscriptor = socket_suscriptor;
		unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
		pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);
		pthread_mutex_lock(&mutexListaSuscriptores);
		list_add(suscriptores_localized_pokemon,unSuscriptor);
		pthread_mutex_unlock(&mutexListaSuscriptores);

		printf("El tamaño de la lista de suscriptores de LP ahora es de: %d\n",list_size(suscriptores_localized_pokemon));

		printf("El suscriptor %d con id = %d fue agregago a la lista localized pokemon\n",socket_suscriptor,generadorDeIDsSuscriptor);
		send(socket_suscriptor,&(unSuscriptor->IDsuscriptor),sizeof(int),0);

		return unSuscriptor;
}

Suscriptor* recibirSuscripcionNewPokemon(int socket_suscriptor){

		Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));

		printf("Recibi solicitud de suscripcion para NewPokemon de socket: %d\n",socket_suscriptor);

		pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
		generadorDeIDsSuscriptor++;
		unSuscriptor->socketSuscriptor = socket_suscriptor;
		unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
		pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);
		pthread_mutex_lock(&mutexListaSuscriptores);
		list_add(suscriptores_new_pokemon,unSuscriptor);
		pthread_mutex_unlock(&mutexListaSuscriptores);
		printf("El tamaño de la lista de suscriptores de NP ahora es de: %d\n",list_size(suscriptores_new_pokemon));

		printf("El suscriptor %d con id = %d fue agregago a la lista new pokemon\n",socket_suscriptor,generadorDeIDsSuscriptor);
		send(socket_suscriptor,&(unSuscriptor->IDsuscriptor),sizeof(int),0);

		return unSuscriptor;
}


void* recibir_mensaje(int socket_cliente, int* size)
{
	void * buffer;
		recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
		buffer = malloc(*size);
		recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}
int guardarMensajeNewPokemon(NewPokemon* unNewPokemon) {
		MensajeNewPokemon* mensaje = malloc(sizeof(MensajeNewPokemon));

		pthread_mutex_lock(&mutexGeneradorIDMensaje);
		generadorDeIDsMensaje++;
		mensaje->contenidoDelMensaje = unNewPokemon;
		mensaje->ID = generadorDeIDsMensaje;
		pthread_mutex_unlock(&mutexGeneradorIDMensaje);

		pthread_mutex_lock(&mutexColaNewPokemon);
		list_add(New_Pokemon, mensaje);
		pthread_cond_signal(&no_vacioNP);
		pthread_mutex_unlock(&mutexColaNewPokemon);

		printf("El tamaño de la lista ahora es de: %d\n", list_size(New_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();
		char* nombreDelPokemonQueGuarde = mensaje->contenidoDelMensaje->nombre;

		printf("Guarde el mensaje: %s\n", nombreDelPokemonQueGuarde);
		printf("Su id es: %d\n",mensaje->ID);

		return mensaje->ID;
}

NewPokemon* recibir_NEW_POKEMON(int cliente_fd,int* size,int reciboID){

		NewPokemon* unNewPokemon = malloc(sizeof(NewPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));
		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		int id;
		if(reciboID > 0){
			memcpy(&id,stream,sizeof(int));
			stream+= sizeof(int);
		}
		memcpy(&(unNewPokemon->tamanioNombrePokemon),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		unNewPokemon->nombre = malloc(unNewPokemon->tamanioNombrePokemon);
		memcpy(unNewPokemon->nombre,stream,unNewPokemon->tamanioNombrePokemon);
		stream+=unNewPokemon->tamanioNombrePokemon;
		memcpy(&(unNewPokemon->coordenadas.posicionX),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		memcpy(&(unNewPokemon->coordenadas.posicionY),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		memcpy(&(unNewPokemon->cantidad),stream,sizeof(uint32_t));

		printf("\nMe llego el mensaje: %s\n",unNewPokemon->nombre);

		free(buffer->stream);
		free(buffer);

		return unNewPokemon;
}

void guardarMensajeLocalizedPokemon(LocalizedPokemon* unLocalizedPokemon,int idCorrelativo){

		MensajeLocalizedPokemon* mensaje = malloc(sizeof(MensajeLocalizedPokemon));

		pthread_mutex_lock(&mutexGeneradorIDMensaje);
		generadorDeIDsMensaje++;
		mensaje->contenidoDelMensaje = unLocalizedPokemon;
		mensaje->ID = generadorDeIDsMensaje;
		mensaje->IDCorrelativo = idCorrelativo;
		pthread_mutex_unlock(&mutexGeneradorIDMensaje);

		pthread_mutex_lock(&mutexColaLocalizedPokemon);
		list_add(Localized_Pokemon, mensaje);
		pthread_cond_signal(&no_vacioLP);
		pthread_mutex_unlock(&mutexColaLocalizedPokemon);

		printf("El tamaño de la lista ahora es de: %d\n", list_size(Localized_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();
		char* nombreDelPokemonQueGuarde = mensaje->contenidoDelMensaje->nombre;

		printf("Guarde el mensaje: %s\n", nombreDelPokemonQueGuarde);
}


LocalizedPokemonConIDCorrelativo* recibir_LOCALIZED_POKEMON(int cliente_fd,int* size){

		LocalizedPokemon* unLocalizedPokemon = malloc(sizeof(LocalizedPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));
		LocalizedPokemonConIDCorrelativo* localizedConIdCorrelativo = malloc(sizeof(LocalizedPokemonConIDCorrelativo));
		localizedConIdCorrelativo->localizedPokemon = unLocalizedPokemon;
		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		int idCorrelativo;

		memcpy(&idCorrelativo,stream,sizeof(int));
		stream+=sizeof(int);
		localizedConIdCorrelativo->IDCorrelativo = idCorrelativo;

		memcpy(&(unLocalizedPokemon->tamanioNombrePokemon),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		unLocalizedPokemon->nombre = malloc(unLocalizedPokemon->tamanioNombrePokemon);
		memcpy(unLocalizedPokemon->nombre,stream,unLocalizedPokemon->tamanioNombrePokemon);
		stream+=unLocalizedPokemon->tamanioNombrePokemon;
		memcpy(&(unLocalizedPokemon->cantidadParesOrdenados),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);

		int cantidadDePares = (unLocalizedPokemon->cantidadParesOrdenados);
		int coordenadasXY = cantidadDePares * 2;
		t_list* unasCoordenadas = list_create();
		for(int i=0;i<coordenadasXY;i++){
			CoordenadasXY* coordenadas = malloc(sizeof(CoordenadasXY));
			memcpy(&(coordenadas->posicionX),stream,sizeof(uint32_t));
			stream+=sizeof(uint32_t);
			memcpy(&(coordenadas->posicionY),stream,sizeof(uint32_t));
			stream+=sizeof(uint32_t);
			list_add(unasCoordenadas,coordenadas);

		}

		unasCoordenadas = unLocalizedPokemon->paresOrdenados;

		printf("\nMe llego el mensaje: %s\n",unLocalizedPokemon->nombre);

		free(buffer->stream);
		free(buffer);

		return localizedConIdCorrelativo;
}

int guardarMensajeGetPokemon(GetPokemon* unGetPokemon){
		MensajeGetPokemon* mensaje = malloc(sizeof(MensajeGetPokemon));

		pthread_mutex_lock(&mutexGeneradorIDMensaje);
		generadorDeIDsMensaje++;
		mensaje->contenidoDelMensaje = unGetPokemon;
		mensaje->ID = generadorDeIDsMensaje;
		pthread_mutex_unlock(&mutexGeneradorIDMensaje);

		pthread_mutex_lock(&mutexColaGetPokemon);
		list_add(Get_Pokemon, mensaje);
		pthread_cond_signal(&no_vacioGP);
		pthread_mutex_unlock(&mutexColaGetPokemon);

		printf("El tamaño de la lista ahora es de: %d\n", list_size(Get_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();
		char* nombreDelPokemonQueGuarde = mensaje->contenidoDelMensaje->nombre;

		printf("Guarde el mensaje: %s\n", nombreDelPokemonQueGuarde);

		return mensaje->ID;
}


GetPokemon* recibir_GET_POKEMON(int cliente_fd, int* size,int reciboID){

		GetPokemon* unGetPokemon = malloc(sizeof(GetPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		int id;
		if(reciboID){
			memcpy(&id,stream,sizeof(int));
			stream+=sizeof(int);
		}
		memcpy(&(unGetPokemon->tamanioNombrePokemon),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		unGetPokemon->nombre = malloc(unGetPokemon->tamanioNombrePokemon);
		memcpy(unGetPokemon->nombre,stream,unGetPokemon->tamanioNombrePokemon);
		stream+=unGetPokemon->tamanioNombrePokemon;

		printf("\nMe llego el mensaje: %s\n",unGetPokemon->nombre);

		free(buffer->stream);
		free(buffer);

		return unGetPokemon;
}

void guardarMensajeAppearedPokemon(AppearedPokemon* unAppearedPokemon){
		MensajeAppearedPokemon* mensaje = malloc(sizeof(MensajeAppearedPokemon));

		pthread_mutex_lock(&mutexGeneradorIDMensaje);
		generadorDeIDsMensaje++;
		mensaje->contenidoDelMensaje = unAppearedPokemon;
		mensaje->ID = generadorDeIDsMensaje;
		pthread_mutex_unlock(&mutexGeneradorIDMensaje);

		pthread_mutex_lock(&mutexColaAppearedPokemon);
		list_add(Appeared_Pokemon, mensaje);
		pthread_cond_signal(&no_vacioAP);
		pthread_mutex_unlock(&mutexColaAppearedPokemon);

		printf("El tamaño de la lista ahora es de: %d\n", list_size(Appeared_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();
		char* nombreDelPokemonQueGuarde = mensaje->contenidoDelMensaje->nombre;

		printf("Guarde el mensaje: %s\n", nombreDelPokemonQueGuarde);
}


AppearedPokemon* recibir_APPEARED_POKEMON(int cliente_fd,int* size,int reciboIDCorrelativo){

		AppearedPokemon* unAppearedPokemon = malloc(sizeof(AppearedPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		int idCorrelativo;

		if(reciboIDCorrelativo){
			memcpy(&idCorrelativo,stream,sizeof(int));
			stream+= sizeof(int);
		}
		memcpy(&(unAppearedPokemon->tamanioNombrePokemon),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		unAppearedPokemon->nombre = malloc(unAppearedPokemon->tamanioNombrePokemon);
		memcpy(unAppearedPokemon->nombre,stream,unAppearedPokemon->tamanioNombrePokemon);
		stream+=unAppearedPokemon->tamanioNombrePokemon;
		memcpy(&(unAppearedPokemon->coordenadas.posicionX),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		memcpy(&(unAppearedPokemon->coordenadas.posicionY),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);

		printf("\nMe llego el mensaje: %s\n",unAppearedPokemon->nombre);

		free(buffer->stream);
		free(buffer);

		return unAppearedPokemon;
}

int guardarMensajeCatchPokemon(CatchPokemon* unCatchPokemon){
		MensajeCatchPokemon* mensaje = malloc(sizeof(MensajeCatchPokemon));

		pthread_mutex_lock(&mutexGeneradorIDMensaje);
		generadorDeIDsMensaje++;
		mensaje->contenidoDelMensaje = unCatchPokemon;
		mensaje->ID = generadorDeIDsMensaje;
		pthread_mutex_unlock(&mutexGeneradorIDMensaje);

		pthread_mutex_lock(&mutexColaCatchPokemon);
		list_add(Catch_Pokemon, mensaje);
		pthread_cond_signal(&no_vacioCP);
		pthread_mutex_unlock(&mutexColaCatchPokemon);

		printf("El tamaño de la lista ahora es de: %d\n", list_size(Catch_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();
		char* nombreDelPokemonQueGuarde = mensaje->contenidoDelMensaje->nombre;

		printf("Guarde el mensaje: %s\n", nombreDelPokemonQueGuarde);

		return mensaje->ID;

}


CatchPokemon* recibir_CATCH_POKEMON(int cliente_fd,int*size,int reciboID){

		CatchPokemon* unCatchPokemon = malloc(sizeof(CatchPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		int id;
		if(reciboID){
			memcpy(&id,stream,sizeof(int));
			stream+= sizeof(int);
		}

		memcpy(&(unCatchPokemon->tamanioNombrePokemon),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		unCatchPokemon->nombre = malloc(unCatchPokemon->tamanioNombrePokemon);
		memcpy(unCatchPokemon->nombre,stream,unCatchPokemon->tamanioNombrePokemon);
		stream+=unCatchPokemon->tamanioNombrePokemon;
		memcpy(&(unCatchPokemon->coordenadas.posicionX),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		memcpy(&(unCatchPokemon->coordenadas.posicionY),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);

		printf("\nMe llego el mensaje: %s\n",unCatchPokemon->nombre);

		free(buffer->stream);
		free(buffer);

		return unCatchPokemon;
}

void guardarMensajeCaughtPokemon(CaughtPokemon* unCaughtPokemon,int idCorrelativo){

		MensajeCaughtPokemon* mensaje = malloc(sizeof(MensajeCaughtPokemon));

		pthread_mutex_lock(&mutexGeneradorIDMensaje);
		generadorDeIDsMensaje++;
		mensaje->contenidoDelMensaje = unCaughtPokemon;
		mensaje->ID = generadorDeIDsMensaje;
		mensaje->IDCorrelativo = idCorrelativo;
		pthread_mutex_unlock(&mutexGeneradorIDMensaje);

		pthread_mutex_lock(&mutexColaCaughtPokemon);
		list_add(Caught_Pokemon, mensaje);
		pthread_cond_signal(&no_vacioCAP);
		pthread_mutex_unlock(&mutexColaCaughtPokemon);

		printf("El tamaño de la lista ahora es de: %d\n", list_size(Caught_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();

		printf("Guarde el mensaje: %d\n",mensaje->contenidoDelMensaje->atrapar);
}

CaughtPokemonConIDCorrelativo* recibir_CAUGHT_POKEMON(int cliente_fd,int* size){

		CaughtPokemon* unCaughtPokemon = malloc(sizeof(CaughtPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));
		CaughtPokemonConIDCorrelativo* unCaughtPokemonConIDCorrelativo = malloc(sizeof(CaughtPokemonConIDCorrelativo));
		unCaughtPokemonConIDCorrelativo->caughtPokemon = unCaughtPokemon;
		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		int idCorrelativo;

		memcpy(&idCorrelativo,stream,sizeof(int));
		stream+=sizeof(int);
		unCaughtPokemonConIDCorrelativo->IDCorrelativo = idCorrelativo;

		memcpy(&(unCaughtPokemon->atrapar),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);

		printf("\nEl pokemon fue atrapado?: %d\n",unCaughtPokemon->atrapar);

		free(buffer->stream);
		free(buffer);

		return unCaughtPokemonConIDCorrelativo;
}


void enviarColaNewPokemon(int socket_suscriptor, Suscriptor* unSuscriptor){
	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
		MensajeNewPokemon* mensaje;
		NewPokemon* unNewPokemon;

		int cantidadDeNewPokemon = list_size(New_Pokemon);
		send(socket_suscriptor,&cantidadDeNewPokemon,sizeof(int),0);
		int ack = 0;

		pthread_mutex_lock(&mutexColaNewPokemon);
		while(cantidadDeNewPokemon==0){
			pthread_cond_wait(&no_vacioLP,&mutexColaNewPokemon);
			printf("Esperando a que llegue algo");

		}
		pthread_mutex_unlock(&mutexColaNewPokemon);

		for(int i=0;i<cantidadDeNewPokemon;i++){

		mensaje = list_get(New_Pokemon,i);
		printf("\nTe voy a enviar el mensaje: %s\n",mensaje->contenidoDelMensaje->nombre);

		pthread_mutex_lock(&mutexColaNewPokemon);
		unNewPokemon = mensaje->contenidoDelMensaje;
		enviarNewPokemon(unNewPokemon,socket_suscriptor,mensaje->ID);
		printf("\nMensaje enviado: %s",unNewPokemon->nombre);

		list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
		printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));
		recv(socket_suscriptor,&ack,sizeof(int),0);
		printf("\nEl ack del mensaje es: %d\n",ack);
		list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
		pthread_mutex_unlock(&mutexColaNewPokemon);

		}
}


void enviarColaLocalizedPokemon(int socket_suscriptor,Suscriptor* unSuscriptor){

	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
			MensajeLocalizedPokemon* mensaje;
			LocalizedPokemon* unLocalizedPokemon;
			int cantidadDeLocalizedPokemon = list_size(Localized_Pokemon);
			send(socket_suscriptor,&cantidadDeLocalizedPokemon,sizeof(int),0);
			int ack = 0;

			pthread_mutex_lock(&mutexColaLocalizedPokemon);
			while(cantidadDeLocalizedPokemon==0){
				pthread_cond_wait(&no_vacioLP,&mutexColaLocalizedPokemon);
				printf("Esperando a que llegue algo");

			}
			pthread_mutex_unlock(&mutexColaLocalizedPokemon);

			for(int i=0;i<cantidadDeLocalizedPokemon;i++){
				mensaje = list_get(Localized_Pokemon,i);
				printf("\nTe voy a enviar el mensaje: %s\n",mensaje->contenidoDelMensaje->nombre);

				pthread_mutex_lock(&mutexColaLocalizedPokemon);
				unLocalizedPokemon = mensaje->contenidoDelMensaje;
				enviarLocalizedPokemon(unLocalizedPokemon,socket_suscriptor,mensaje->IDCorrelativo);
				printf("\nMensaje enviado: %s",unLocalizedPokemon->nombre);

				list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
				printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));
				recv(socket_suscriptor,&ack,sizeof(int),0);
				printf("\nEl ack del mensaje es: %d\n",ack);
				list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				puts("\nSuscriptor ok");

				pthread_mutex_unlock(&mutexColaLocalizedPokemon);

				}
}

void enviarColaGetPokemon(int socket_suscriptor,Suscriptor* unSuscriptor){

	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
			MensajeGetPokemon* mensaje;
			GetPokemon* unGetPokemon;
			int cantidadDeGetPokemon = list_size(Get_Pokemon);
			send(socket_suscriptor,&cantidadDeGetPokemon,sizeof(int),0);
			int ack = 0;

			pthread_mutex_lock(&mutexColaGetPokemon);
			while(cantidadDeGetPokemon==0){
				pthread_cond_wait(&no_vacioGP,&mutexColaGetPokemon);
				printf("Esperando a que llegue algo");

			}
			pthread_mutex_unlock(&mutexColaGetPokemon);

			for(int i=0;i<cantidadDeGetPokemon;i++){
				mensaje = list_get(Get_Pokemon,i);
				printf("\nTe voy a enviar el mensaje: %s\n",mensaje->contenidoDelMensaje->nombre);

				pthread_mutex_lock(&mutexColaGetPokemon);
				unGetPokemon = mensaje->contenidoDelMensaje;
				enviarGetPokemon(unGetPokemon,socket_suscriptor,mensaje->ID);
				printf("\nMensaje enviado: %s",unGetPokemon->nombre);

				list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
				printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));
				recv(socket_suscriptor,&ack,sizeof(int),0);
				printf("\nEl ack del mensaje es: %d\n",ack);
				list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				puts("\nSuscriptor ok");

				pthread_mutex_unlock(&mutexColaGetPokemon);

				}

}


void enviarColaAppearedPokemon(int socket_suscriptor,Suscriptor* unSuscriptor){

	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
			MensajeAppearedPokemon* mensaje;
			AppearedPokemon* unAppearedPokemon;
			int cantidadDeAppearedPokemon = list_size(Appeared_Pokemon);
			send(socket_suscriptor,&cantidadDeAppearedPokemon,sizeof(int),0);
			int ack = 0;

			pthread_mutex_lock(&mutexColaAppearedPokemon);
			while(cantidadDeAppearedPokemon==0){
				pthread_cond_wait(&no_vacioAP,&mutexColaAppearedPokemon);
				printf("Esperando a que llegue algo");

			}
			pthread_mutex_unlock(&mutexColaAppearedPokemon);

			for(int i=0;i<cantidadDeAppearedPokemon;i++){
					mensaje = list_get(Appeared_Pokemon,i);
					printf("\nTe voy a enviar el mensaje: %s\n",mensaje->contenidoDelMensaje->nombre);

					pthread_mutex_lock(&mutexColaAppearedPokemon);
					unAppearedPokemon = mensaje->contenidoDelMensaje;
					enviarAppearedPokemon(unAppearedPokemon,socket_suscriptor,mensaje->ID);
					printf("\nMensaje enviado: %s",unAppearedPokemon->nombre);

					list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
					printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));
					recv(socket_suscriptor,&ack,sizeof(int),0);
					printf("\nEl ack del mensaje es: %d\n",ack);
					list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
					puts("\nSuscriptor ok");

					pthread_mutex_unlock(&mutexColaAppearedPokemon);

			}
}

void enviarColaCatchPokemon(int socket_suscriptor,Suscriptor* unSuscriptor){
	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
			MensajeCatchPokemon* mensaje;
			CatchPokemon* unCatchPokemon;
			int cantidadDeCatchPokemon = list_size(Catch_Pokemon);
			send(socket_suscriptor,&cantidadDeCatchPokemon,sizeof(int),0);
			int ack = 0;
			int mensajeID;

			pthread_mutex_lock(&mutexColaCatchPokemon);
			while(cantidadDeCatchPokemon==0){
				pthread_cond_wait(&no_vacioCP,&mutexColaCatchPokemon);
				printf("Esperando a que llegue algo");

			}
			pthread_mutex_unlock(&mutexColaCatchPokemon);

			for(int i=0;i<cantidadDeCatchPokemon;i++){
					mensaje = list_get(Catch_Pokemon,i);
					printf("\nTe voy a enviar el mensaje: %s\n",mensaje->contenidoDelMensaje->nombre);
					mensajeID = mensaje->ID;
					printf("\nEl ID del mensaje es: %d",mensaje->ID);
					send(socket_suscriptor,&mensajeID,sizeof(int),0);
					printf("\nID enviado: %d",mensaje->ID);

					pthread_mutex_lock(&mutexColaCatchPokemon);
					unCatchPokemon = mensaje->contenidoDelMensaje;
					enviarCatchPokemon(unCatchPokemon,socket_suscriptor,mensaje->ID);
					printf("\nMensaje enviado: %s",unCatchPokemon->nombre);

					list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
					printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));
					recv(socket_suscriptor,&ack,sizeof(int),0);
					printf("\nEl ack del mensaje es: %d\n",ack);
					list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
					puts("\nSuscriptor ok");

					pthread_mutex_unlock(&mutexColaCatchPokemon);

					}
}


void enviarColaCaughtPokemon(int socket_suscriptor,Suscriptor* unSuscriptor){
	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
			MensajeCaughtPokemon* mensaje;
			CaughtPokemon* unCaughtPokemon;
			int cantidadDeCaughtPokemon = list_size(Caught_Pokemon);
			send(socket_suscriptor,&cantidadDeCaughtPokemon,sizeof(int),0);
			int ack = 0;
			int mensajeID;

			pthread_mutex_lock(&mutexColaCaughtPokemon);
			while(cantidadDeCaughtPokemon==0){
				pthread_cond_wait(&no_vacioCAP,&mutexColaCaughtPokemon);
				printf("Esperando a que llegue algo");

			}
			pthread_mutex_unlock(&mutexColaCaughtPokemon);

			for(int i=0;i<cantidadDeCaughtPokemon;i++){
					mensaje = list_get(Caught_Pokemon,i);
					printf("\nTe voy a enviar el mensaje: %d\n",mensaje->contenidoDelMensaje->atrapar);
					mensajeID = mensaje->ID;
					printf("\nEl ID del mensaje es: %d",mensaje->ID);
					send(socket_suscriptor,&mensajeID,sizeof(int),0);
					printf("\nID enviado: %d",mensaje->ID);

					pthread_mutex_lock(&mutexColaCaughtPokemon);
					unCaughtPokemon = mensaje->contenidoDelMensaje;
					enviarCaughtPokemon(unCaughtPokemon,socket_suscriptor,mensaje->IDCorrelativo);
					printf("\nMensaje enviado: %d",unCaughtPokemon->atrapar);

					list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
					printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));
					recv(socket_suscriptor,&ack,sizeof(int),0);
					printf("\nEl ack del mensaje es: %d\n",ack);
					list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
					puts("\nSuscriptor ok");

					pthread_mutex_unlock(&mutexColaCaughtPokemon);

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





void* serializarLocalizedPokemon(LocalizedPokemon* localizedPokemon,int bytes, int idCorrelativo){

	void* buffer = malloc(bytes);
	uint32_t tamanioNombre = strlen(localizedPokemon->nombre) + 1;
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento,&idCorrelativo,sizeof(int));
	desplazamiento += sizeof(int);

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

void* serializarGetPokemon(GetPokemon* getPokemon,int bytes,int id){

	void* buffer = malloc(bytes);
	uint32_t tamanioNombre = strlen(getPokemon->nombre) + 1;
	int desplazamiento = 0;

	if(id > 0){
		memcpy(buffer + desplazamiento,&id,sizeof(int));
		desplazamiento+= sizeof(int);
	}

	memcpy(buffer + desplazamiento,&tamanioNombre,sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,getPokemon->nombre,tamanioNombre);
	desplazamiento+= tamanioNombre;

	return buffer;
}

void* serializarAppearedPokemon(AppearedPokemon* appearedPokemon,int bytes,int idCorrelativo){

	void* buffer = malloc(bytes);
	uint32_t tamanioNombre = strlen(appearedPokemon->nombre) + 1;
	int desplazamiento = 0;

	if(idCorrelativo > 0){
		memcpy(buffer + desplazamiento,&idCorrelativo,sizeof(int));
		desplazamiento+= sizeof(int);
	}
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

void* serializarCatchPokemon(CatchPokemon* catchPokemon,int bytes,int id){

	void* buffer = malloc(bytes);
	uint32_t tamanioNombre = strlen(catchPokemon->nombre) + 1;
	int desplazamiento = 0;

	if(id > 0){
		memcpy(buffer + desplazamiento,&id,sizeof(int));
		desplazamiento+= sizeof(int);
	}

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

void* serializarCaughtPokemon(CaughtPokemon* caughtPokemon,int bytes,int idCorrelativo){

	void* buffer = malloc(bytes);
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento,&idCorrelativo,sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(buffer + desplazamiento,&(caughtPokemon->atrapar),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	return buffer;
}

void* serializarNewPokemon(NewPokemon* newPokemon,int bytes,int id){

	void* stream = malloc(bytes);
	uint32_t tamanioNombre = strlen(newPokemon->nombre) + 1;
	int desplazamiento = 0;

	if(id > 0){
		memcpy(stream + desplazamiento,&id,sizeof(int));
		desplazamiento+= sizeof(int);
	}
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

void enviarNewPokemon(NewPokemon* unNewPokemon, int socket_suscriptor,int id){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	uint32_t tamanioNombrePokemon = strlen(unNewPokemon->nombre) +1;
	void* stream;
	if(id > 0){
		buffer->size = sizeof(uint32_t) * 4 + tamanioNombrePokemon + sizeof(int);
	} else {
		buffer->size = sizeof(uint32_t) * 4 + tamanioNombrePokemon;
	}
	stream = serializarNewPokemon(unNewPokemon,buffer->size,id);
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

void enviarLocalizedPokemon(LocalizedPokemon* localized_pokemon,int socket_suscriptor,int idCorrelativo){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	int tamanioNombrePokemon = strlen(localized_pokemon->nombre) +1;
	int coordenadas = localized_pokemon->cantidadParesOrdenados * 2;
	buffer->size = sizeof(uint32_t) * 2 * coordenadas + tamanioNombrePokemon + sizeof(int);
	void* stream = serializarLocalizedPokemon(localized_pokemon,buffer->size,idCorrelativo);
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

void enviarGetPokemon(GetPokemon* get_pokemon,int socket_suscriptor,int id){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	uint32_t tamanioNombrePokemon = strlen(get_pokemon->nombre) +1;

	if(id > 0){
		buffer->size = sizeof(uint32_t) + tamanioNombrePokemon + sizeof(int);
	}else {
		buffer->size = sizeof(uint32_t) + tamanioNombrePokemon;
	}
	void* stream = serializarGetPokemon(get_pokemon,buffer->size,id);
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

void enviarAppearedPokemon(AppearedPokemon* appeared_pokemon,int socket_suscriptor,int idCorrelativo){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	uint32_t tamanioNombrePokemon = strlen(appeared_pokemon->nombre) +1;
	void* stream;
	if(idCorrelativo > 0){
		buffer->size = sizeof(uint32_t) * 3 + tamanioNombrePokemon + sizeof(int);
		stream = serializarAppearedPokemon(appeared_pokemon,buffer->size,idCorrelativo);
	} else {
		buffer->size = sizeof(uint32_t) * 3 + tamanioNombrePokemon;
		stream = serializarAppearedPokemon(appeared_pokemon,buffer->size,0);
	}
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

void enviarCatchPokemon(CatchPokemon* catch_pokemon,int socket_suscriptor,int id){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	uint32_t tamanioNombrePokemon = strlen(catch_pokemon->nombre) +1;
	if(id > 0){
		buffer->size = sizeof(uint32_t) + tamanioNombrePokemon + sizeof(int);
	}else {
		buffer->size = sizeof(uint32_t) + tamanioNombrePokemon;
	}
	void* stream = serializarCatchPokemon(catch_pokemon,buffer->size,id);
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

void enviarCaughtPokemon(CaughtPokemon* caught_pokemon,int socket_suscriptor,int idCorrelativo){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t) + sizeof(int);
	void* stream = serializarCaughtPokemon(caught_pokemon,buffer->size,idCorrelativo);
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
