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
		printf("Error en la conexion");

	freeaddrinfo(server_info);

	return socket_cliente;
}

void process_request(int cod_op, int cliente_fd) {
	int size;
	Suscriptor* elQueSeMeSuscribe;
	void* algoARecibir;

	NewPokemon* unNewPokemon;
	MensajeNewPokemon* mensajeNewPokemon;
	LocalizedPokemon* unLocalizedPokemon;
	MensajeLocalizedPokemon* mensajeLocalizedPokemon;
	GetPokemon* unGetPokemon;
	MensajeGetPokemon* unMensajeGetPokemon;
	AppearedPokemon* unAppearedPokemon;
	MensajeAppearedPokemon* mensajeAppearedPokemon;
	CatchPokemon* unCatchPokemon;
	MensajeCatchPokemon* mensajeCatchPokemon;
	CaughtPokemon* unCaughtPokemon;
	MensajeCaughtPokemon* mensajeCaughtPokemon;

	NewPokemonConIDs* newConIDs;
	LocalizedPokemonConIDs* unLocalizedPokemonConIDCorrelativo;
	GetPokemonConIDs* getConIDs;
	AppearedPokemonConIDs* appearedConIDs;
	CatchPokemonConIDs* catchConIDs;
	CaughtPokemonConIDs* unCaughtPokemonConIDCorrelativo;

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
					newConIDs = recibir_NEW_POKEMON(cliente_fd, &size,0);
					unNewPokemon = newConIDs->newPokemon;
					free(newConIDs);
					mensajeNewPokemon = guardarMensajeNewPokemon(unNewPokemon);
					enviarNewPokemonASuscriptores(mensajeNewPokemon);
					break;
				case LOCALIZED_POKEMON:
					unLocalizedPokemonConIDCorrelativo = recibir_LOCALIZED_POKEMON(cliente_fd,&size,0);
					unLocalizedPokemon = unLocalizedPokemonConIDCorrelativo->localizedPokemon;
					idCorrelativo = unLocalizedPokemonConIDCorrelativo->IDcorrelativo;
					free(unLocalizedPokemonConIDCorrelativo);
					mensajeLocalizedPokemon = guardarMensajeLocalizedPokemon(unLocalizedPokemon,idCorrelativo);
					enviarLocalizedPokemonASuscriptores(mensajeLocalizedPokemon);
					break;
				case GET_POKEMON:
					getConIDs = recibir_GET_POKEMON(cliente_fd,&size,0);
					unGetPokemon = getConIDs->getPokemon;
					free(getConIDs);
					unMensajeGetPokemon = guardarMensajeGetPokemon(unGetPokemon);
					enviarGetPokemonASuscriptores(unMensajeGetPokemon);
					break;
				case APPEARED_POKEMON:
					appearedConIDs = recibir_APPEARED_POKEMON(cliente_fd,&size,0,1);
					unAppearedPokemon = appearedConIDs->appearedPokemon;
					free(appearedConIDs);
					mensajeAppearedPokemon = guardarMensajeAppearedPokemon(unAppearedPokemon);
					enviarAppearedPokemonASuscriptores(mensajeAppearedPokemon);
					break;
				case CATCH_POKEMON:
					catchConIDs = recibir_CATCH_POKEMON(cliente_fd,&size,0);
					unCatchPokemon = catchConIDs->catchPokemon;
					free(catchConIDs);
					mensajeCatchPokemon = guardarMensajeCatchPokemon(unCatchPokemon);
					enviarCatchPokemonASuscriptores(mensajeCatchPokemon);
					break;
				case CAUGHT_POKEMON:
					unCaughtPokemonConIDCorrelativo = recibir_CAUGHT_POKEMON(cliente_fd,&size,0);
					unCaughtPokemon = unCaughtPokemonConIDCorrelativo->caughtPokemon;
					idCorrelativo = unCaughtPokemonConIDCorrelativo->IDCorrelativo;
					free(unCaughtPokemonConIDCorrelativo);
					mensajeCaughtPokemon = guardarMensajeCaughtPokemon(unCaughtPokemon,idCorrelativo);
					enviarCaughtPokemonASuscriptores(mensajeCaughtPokemon);
					break;
				case SUSCRIPTOR_NEWPOKEMON:
					elQueSeMeSuscribe =	recibirSuscripcionNewPokemon(cliente_fd);
					break;
				case SUSCRIPTOR_LOCALIZEDPOKEMON:
					elQueSeMeSuscribe =	recibirSuscripcionLocalizedPokemon(cliente_fd);
					break;
				case SUSCRIPTOR_GETPOKEMON:
					elQueSeMeSuscribe = recibirSuscripcionGetPokemon(cliente_fd);
					break;
				case SUSCRIPTOR_APPEAREDPOKEMON:
					elQueSeMeSuscribe = recibirSuscripcionAppearedPokemon(cliente_fd);
					break;
				case SUSCRIPTOR_CATCHPOKEMON:
					elQueSeMeSuscribe = recibirSuscripcionCatchPokemon(cliente_fd);
					break;
				case SUSCRIPTOR_CAUGHTPOKEMON:
					elQueSeMeSuscribe = recibirSuscripcionCaughtPokemon(cliente_fd);
					break;
	}
}



void enviarCaughtPokemonASuscriptores(MensajeCaughtPokemon* unMensajeCaughtPokemon){
		Suscriptor* unSuscriptor;
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int tamanioSuscriptoresCaughtPokemon = list_size(suscriptores_caught_pokemon);

		for(int i=0;i<tamanioSuscriptoresCaughtPokemon;i++){
			unSuscriptor = list_get(suscriptores_caught_pokemon,i);
			enviarCaughtPokemon(unMensajeCaughtPokemon->contenidoDelMensaje,unSuscriptor->socketSuscriptor,unMensajeCaughtPokemon->ID,unMensajeCaughtPokemon->IDCorrelativo);
			list_add(unMensajeCaughtPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));

			loQueDevuelveElRecv = recv(unSuscriptor->socketSuscriptor,&ack,sizeof(int),MSG_WAITALL);
			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("Se cayó la conexion...");
			}
			if(ack == 1){
				list_add(unMensajeCaughtPokemon->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				printf("El suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK",unSuscriptor->IDsuscriptor);
			}

		}
}

void enviarCatchPokemonASuscriptores(MensajeCatchPokemon* unMensajeCatchPokemon){
		Suscriptor* unSuscriptor;
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int tamanioSuscriptoresCatchPokemon = list_size(suscriptores_catch_pokemon);

		for(int i=0;i<tamanioSuscriptoresCatchPokemon;i++){
			unSuscriptor = list_get(suscriptores_catch_pokemon,i);
			enviarCatchPokemon(unMensajeCatchPokemon->contenidoDelMensaje,unSuscriptor->socketSuscriptor,unMensajeCatchPokemon->ID);
			list_add(unMensajeCatchPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));

			loQueDevuelveElRecv = recv(unSuscriptor->socketSuscriptor,&ack,sizeof(int),MSG_WAITALL);
			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("Se cayó la conexion...");
			}
			if(ack == 1){
				list_add(unMensajeCatchPokemon->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				printf("El suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK",unSuscriptor->IDsuscriptor);
			}
		}
}

void enviarLocalizedPokemonASuscriptores(MensajeLocalizedPokemon* unMensajeLocalizedPokemon){
		Suscriptor* unSuscriptor;
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int tamanioSuscriptoresLocalizedPokemon = list_size(suscriptores_localized_pokemon);

		for(int i=0;i<tamanioSuscriptoresLocalizedPokemon;i++){
			unSuscriptor = list_get(suscriptores_localized_pokemon,i);
			enviarLocalizedPokemon(unMensajeLocalizedPokemon->contenidoDelMensaje,unSuscriptor->socketSuscriptor,unMensajeLocalizedPokemon->ID,unMensajeLocalizedPokemon->IDCorrelativo);
			list_add(unMensajeLocalizedPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));

			loQueDevuelveElRecv = recv(unSuscriptor->socketSuscriptor,&ack,sizeof(int),MSG_WAITALL);
			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("Se cayó la conexion...");
			}
			if(ack == 1){
				list_add(unMensajeLocalizedPokemon->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				printf("El suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK",unSuscriptor->IDsuscriptor);
			}
		}
}


void enviarGetPokemonASuscriptores(MensajeGetPokemon* unMensajeGetPokemon){
		Suscriptor* unSuscriptor;
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int tamanioSuscriptoresGetPokemon = list_size(suscriptores_get_pokemon);

		for(int i=0;i<tamanioSuscriptoresGetPokemon;i++){
			unSuscriptor = list_get(suscriptores_get_pokemon,i);
			enviarGetPokemon(unMensajeGetPokemon->contenidoDelMensaje,unSuscriptor->socketSuscriptor,unMensajeGetPokemon->ID);
			list_add(unMensajeGetPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));

			loQueDevuelveElRecv = recv(unSuscriptor->socketSuscriptor,&ack,sizeof(int),MSG_WAITALL);
			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("Se cayó la conexion...");
			}
			if(ack == 1){
				list_add(unMensajeGetPokemon->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				printf("El suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK",unSuscriptor->IDsuscriptor);
			}
		}
}

void enviarAppearedPokemonASuscriptores(MensajeAppearedPokemon* unMensajeAppearedPokemon){
		Suscriptor* unSuscriptor;
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int tamanioSuscriptoresAppearedPokemon = list_size(suscriptores_appeared_pokemon);

		for(int i=0;i<tamanioSuscriptoresAppearedPokemon;i++){
			unSuscriptor = list_get(suscriptores_appeared_pokemon,i);
			enviarAppearedPokemon(unMensajeAppearedPokemon->contenidoDelMensaje,unSuscriptor->socketSuscriptor,unMensajeAppearedPokemon->ID,0);
			list_add(unMensajeAppearedPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));

			loQueDevuelveElRecv = recv(unSuscriptor->socketSuscriptor,&ack,sizeof(int),MSG_WAITALL);
			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("Se cayó la conexion...");
			}
			if(ack == 1){
				list_add(unMensajeAppearedPokemon->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				printf("El suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK",unSuscriptor->IDsuscriptor);
			}
		}
}

void enviarNewPokemonASuscriptores(MensajeNewPokemon* unMensajeNewPokemon){
		Suscriptor* unSuscriptor;
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int tamanioSuscriptoresNewPokemon = list_size(suscriptores_new_pokemon);

		for(int i=0;i<tamanioSuscriptoresNewPokemon;i++){
			unSuscriptor = list_get(suscriptores_new_pokemon,i);
			enviarNewPokemon(unMensajeNewPokemon->contenidoDelMensaje,unSuscriptor->socketSuscriptor,unMensajeNewPokemon->ID);
			list_add(unMensajeNewPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
			puts("Suscriptor agregado a la lista de atendidos");

			loQueDevuelveElRecv = recv(unSuscriptor->socketSuscriptor,&ack,sizeof(int),MSG_WAITALL);
			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("Se cayó la conexion...");
			}
			if(ack == 1){
				list_add(unMensajeNewPokemon->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				printf("El suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK",unSuscriptor->IDsuscriptor);
			}
		}
}

Suscriptor* recibirSuscripcionCaughtPokemon(int socket_suscriptor){

		Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));
		int tamanioListaSuscriptores = list_size(suscriptores_caught_pokemon);
		int idSuscriptor,idGeneradoEnElMomento;

		t_buffer* buffer = malloc(sizeof(t_buffer));
		recv(socket_suscriptor,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(socket_suscriptor,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&idSuscriptor,stream,sizeof(int));
		printf("\nRecibi solicitud de suscripcion para CaughtPokemon de socket: %d\n",socket_suscriptor);
		printf("Su ID es: %d\n",idSuscriptor);

		if(idSuscriptor == 0){
			pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
			generadorDeIDsSuscriptor++;
			unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
			pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);
			unSuscriptor->socketSuscriptor = socket_suscriptor;

			printf("Pero ahora su ID es: %d\n",unSuscriptor->IDsuscriptor);

			pthread_mutex_lock(&mutexListaSuscriptores);
			list_add(suscriptores_caught_pokemon,unSuscriptor);
			pthread_mutex_unlock(&mutexListaSuscriptores);

			printf("El tamaño de la lista de suscriptores de CAP ahora es de: %d\n",list_size(suscriptores_caught_pokemon));
			printf("El suscriptor %d con id = %d fue agregado a la lista caught pokemon\n",socket_suscriptor,unSuscriptor->IDsuscriptor);
			send(socket_suscriptor,&(unSuscriptor->IDsuscriptor),sizeof(int),0);

			idGeneradoEnElMomento = 1;
			enviarColaCaughtPokemon(idGeneradoEnElMomento,socket_suscriptor,unSuscriptor);
		} else {
			Suscriptor* otroSuscriptor;
			for(int i=0; i<tamanioListaSuscriptores;i++){
				otroSuscriptor = list_get(suscriptores_caught_pokemon,i);
				if(otroSuscriptor->IDsuscriptor == idSuscriptor){
					otroSuscriptor->socketSuscriptor = socket_suscriptor;

					idGeneradoEnElMomento = 0;
					enviarColaCaughtPokemon(idGeneradoEnElMomento,socket_suscriptor,otroSuscriptor);
					otroSuscriptor = unSuscriptor;
				}
					puts("Sigo buscando...\n");
			}
		}

		free(buffer->stream);
		free(buffer);
		return unSuscriptor;
}

Suscriptor* recibirSuscripcionCatchPokemon(int socket_suscriptor){

		Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));
		int tamanioListaSuscriptores = list_size(suscriptores_catch_pokemon);
		int idSuscriptor,idGeneradoEnElMomento;

		t_buffer* buffer = malloc(sizeof(t_buffer));
		recv(socket_suscriptor,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(socket_suscriptor,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&idSuscriptor,stream,sizeof(int));
		printf("\nRecibi solicitud de suscripcion para CatchPokemon de socket: %d\n",socket_suscriptor);
		printf("Su ID es: %d\n",idSuscriptor);

		if(idSuscriptor == 0){
			pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
			generadorDeIDsSuscriptor++;
			unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
			pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);
			unSuscriptor->socketSuscriptor = socket_suscriptor;

			printf("Pero ahora su ID es: %d\n",unSuscriptor->IDsuscriptor);

			pthread_mutex_lock(&mutexListaSuscriptores);
			list_add(suscriptores_catch_pokemon,unSuscriptor);
			pthread_mutex_unlock(&mutexListaSuscriptores);

			printf("El tamaño de la lista de suscriptores de CP ahora es de: %d\n",list_size(suscriptores_catch_pokemon));
			printf("El suscriptor %d con id = %d fue agregado a la lista catch pokemon\n",socket_suscriptor,unSuscriptor->IDsuscriptor);
			send(socket_suscriptor,&(unSuscriptor->IDsuscriptor),sizeof(int),0);

			idGeneradoEnElMomento = 1;
			enviarColaCatchPokemon(idGeneradoEnElMomento,socket_suscriptor,unSuscriptor);
		} else {
			Suscriptor* otroSuscriptor;
			for(int i=0; i<tamanioListaSuscriptores;i++){
				otroSuscriptor = list_get(suscriptores_catch_pokemon,i);
				if(otroSuscriptor->IDsuscriptor == idSuscriptor){
					otroSuscriptor->socketSuscriptor = socket_suscriptor;

					idGeneradoEnElMomento = 0;
					enviarColaCatchPokemon(idGeneradoEnElMomento,socket_suscriptor,otroSuscriptor);
					otroSuscriptor = unSuscriptor;
				}
					puts("Sigo buscando...\n");
			}
		}

		free(buffer->stream);
		free(buffer);
		return unSuscriptor;
}

Suscriptor* recibirSuscripcionAppearedPokemon(int socket_suscriptor){

		Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));
		int tamanioListaSuscriptores = list_size(suscriptores_appeared_pokemon);
		int idSuscriptor,idGeneradoEnElMomento;

		t_buffer* buffer = malloc(sizeof(t_buffer));
		recv(socket_suscriptor,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(socket_suscriptor,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&idSuscriptor,stream,sizeof(int));
		printf("\nRecibi solicitud de suscripcion para AppearedPokemon de socket: %d\n",socket_suscriptor);
		printf("Su ID es: %d\n",idSuscriptor);

		if(idSuscriptor == 0){
			pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
			generadorDeIDsSuscriptor++;
			unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
			pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);
			unSuscriptor->socketSuscriptor = socket_suscriptor;

			printf("Pero ahora su ID es: %d\n",unSuscriptor->IDsuscriptor);

			pthread_mutex_lock(&mutexListaSuscriptores);
			list_add(suscriptores_appeared_pokemon,unSuscriptor);
			pthread_mutex_unlock(&mutexListaSuscriptores);

			printf("El tamaño de la lista de suscriptores de AP ahora es de: %d\n",list_size(suscriptores_appeared_pokemon));
			printf("El suscriptor %d con id = %d fue agregado a la lista appeared pokemon\n",socket_suscriptor,unSuscriptor->IDsuscriptor);
			send(socket_suscriptor,&(unSuscriptor->IDsuscriptor),sizeof(int),0);

			idGeneradoEnElMomento = 1;
			enviarColaAppearedPokemon(idGeneradoEnElMomento,socket_suscriptor,unSuscriptor);
		} else {
			Suscriptor* otroSuscriptor;
			for(int i=0; i<tamanioListaSuscriptores;i++){
				otroSuscriptor = list_get(suscriptores_appeared_pokemon,i);
				if(otroSuscriptor->IDsuscriptor == idSuscriptor){
					otroSuscriptor->socketSuscriptor = socket_suscriptor;

					idGeneradoEnElMomento = 0;
					enviarColaAppearedPokemon(idGeneradoEnElMomento,socket_suscriptor,otroSuscriptor);
					otroSuscriptor = unSuscriptor;
				}
					puts("Sigo buscando...\n");
			}
		}

		free(buffer->stream);
		free(buffer);
		return unSuscriptor;
}

Suscriptor* recibirSuscripcionGetPokemon(int socket_suscriptor){

		Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));
		int tamanioListaSuscriptores = list_size(suscriptores_get_pokemon);
		int idSuscriptor,idGeneradoEnElMomento;

		t_buffer* buffer = malloc(sizeof(t_buffer));
		recv(socket_suscriptor,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(socket_suscriptor,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&idSuscriptor,stream,sizeof(int));
		printf("\nRecibi solicitud de suscripcion para GetPokemon de socket: %d\n",socket_suscriptor);
		printf("Su ID es: %d\n",idSuscriptor);

		if(idSuscriptor == 0){
			pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
			generadorDeIDsSuscriptor++;
			unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
			pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);
			unSuscriptor->socketSuscriptor = socket_suscriptor;

			printf("Pero ahora su ID es: %d\n",unSuscriptor->IDsuscriptor);

			pthread_mutex_lock(&mutexListaSuscriptores);
			list_add(suscriptores_get_pokemon,unSuscriptor);
			pthread_mutex_unlock(&mutexListaSuscriptores);

			printf("El tamaño de la lista de suscriptores de GP ahora es de: %d\n",list_size(suscriptores_get_pokemon));
			printf("El suscriptor %d con id = %d fue agregado a la lista get pokemon\n",socket_suscriptor,unSuscriptor->IDsuscriptor);
			send(socket_suscriptor,&(unSuscriptor->IDsuscriptor),sizeof(int),0);

			idGeneradoEnElMomento = 1;
			enviarColaGetPokemon(idGeneradoEnElMomento,socket_suscriptor,unSuscriptor);
		} else {
			Suscriptor* otroSuscriptor;
			for(int i=0; i<tamanioListaSuscriptores;i++){
				otroSuscriptor = list_get(suscriptores_get_pokemon,i);
				if(otroSuscriptor->IDsuscriptor == idSuscriptor){
					otroSuscriptor->socketSuscriptor = socket_suscriptor;

					idGeneradoEnElMomento = 0;
					enviarColaGetPokemon(idGeneradoEnElMomento,socket_suscriptor,otroSuscriptor);
					otroSuscriptor = unSuscriptor;
				}
					puts("Sigo buscando...\n");
			}
		}

		free(buffer->stream);
		free(buffer);
		return unSuscriptor;
}


Suscriptor* recibirSuscripcionLocalizedPokemon(int socket_suscriptor){

		Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));
		int tamanioListaSuscriptores = list_size(suscriptores_localized_pokemon);
		int idSuscriptor,idGeneradoEnElMomento;

		t_buffer* buffer = malloc(sizeof(t_buffer));
		recv(socket_suscriptor,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(socket_suscriptor,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&idSuscriptor,stream,sizeof(int));
		printf("\nRecibi solicitud de suscripcion para LocalizedPokemon de socket: %d\n",socket_suscriptor);
		printf("Su ID es: %d\n",idSuscriptor);

		if(idSuscriptor == 0){
			pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
			generadorDeIDsSuscriptor++;
			unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
			pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);
			unSuscriptor->socketSuscriptor = socket_suscriptor;

			printf("Pero ahora su ID es: %d\n",unSuscriptor->IDsuscriptor);

			pthread_mutex_lock(&mutexListaSuscriptores);
			list_add(suscriptores_localized_pokemon,unSuscriptor);
			pthread_mutex_unlock(&mutexListaSuscriptores);

			printf("El tamaño de la lista de suscriptores de LP ahora es de: %d\n",list_size(suscriptores_localized_pokemon));
			printf("El suscriptor %d con id = %d fue agregado a la lista localized pokemon\n",socket_suscriptor,unSuscriptor->IDsuscriptor);
			send(socket_suscriptor,&(unSuscriptor->IDsuscriptor),sizeof(int),0);

			idGeneradoEnElMomento = 1;
			enviarColaLocalizedPokemon(idGeneradoEnElMomento,socket_suscriptor,unSuscriptor);
		} else {
			Suscriptor* otroSuscriptor;
			for(int i=0; i<tamanioListaSuscriptores;i++){
				otroSuscriptor = list_get(suscriptores_localized_pokemon,i);
				if(otroSuscriptor->IDsuscriptor == idSuscriptor){
					otroSuscriptor->socketSuscriptor = socket_suscriptor;

					idGeneradoEnElMomento = 0;
					enviarColaLocalizedPokemon(idGeneradoEnElMomento,socket_suscriptor,otroSuscriptor);
					otroSuscriptor = unSuscriptor;
				}
					puts("Sigo buscando...\n");
			}
		}

		free(buffer->stream);
		free(buffer);
		return unSuscriptor;
}

Suscriptor* recibirSuscripcionNewPokemon(int socket_suscriptor){

		Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));
		int tamanioListaSuscriptores = list_size(suscriptores_new_pokemon);
		int idSuscriptor,idGeneradoEnElMomento;

		t_buffer* buffer = malloc(sizeof(t_buffer));
		recv(socket_suscriptor,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(socket_suscriptor,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&idSuscriptor,stream,sizeof(int));
		printf("\nRecibi solicitud de suscripcion para NewPokemon de socket: %d\n",socket_suscriptor);
		printf("Su ID es: %d\n",idSuscriptor);

		if(idSuscriptor == 0){
			pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
			generadorDeIDsSuscriptor++;
			unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
			pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);
			unSuscriptor->socketSuscriptor = socket_suscriptor;

			printf("Pero ahora su ID es: %d\n",unSuscriptor->IDsuscriptor);

			pthread_mutex_lock(&mutexListaSuscriptores);
			list_add(suscriptores_new_pokemon,unSuscriptor);
			pthread_mutex_unlock(&mutexListaSuscriptores);

			printf("El tamaño de la lista de suscriptores de NP ahora es de: %d\n",list_size(suscriptores_new_pokemon));
			printf("El suscriptor %d con id = %d fue agregado a la lista new pokemon\n",socket_suscriptor,unSuscriptor->IDsuscriptor);
			send(socket_suscriptor,&(unSuscriptor->IDsuscriptor),sizeof(int),0);

			idGeneradoEnElMomento = 1;
			enviarColaNewPokemon(idGeneradoEnElMomento,socket_suscriptor,unSuscriptor);
		} else {
			Suscriptor* otroSuscriptor;
			for(int i=0; i<tamanioListaSuscriptores;i++){
				otroSuscriptor = list_get(suscriptores_new_pokemon,i);
					if(otroSuscriptor->IDsuscriptor == idSuscriptor){
						otroSuscriptor->socketSuscriptor = socket_suscriptor;

						idGeneradoEnElMomento = 0;
						enviarColaNewPokemon(idGeneradoEnElMomento,socket_suscriptor,otroSuscriptor);
						otroSuscriptor = unSuscriptor;
					}
					puts("Sigo buscando...\n");
			}
		}

		free(buffer->stream);
		free(buffer);
		return unSuscriptor;
}


MensajeNewPokemon* guardarMensajeNewPokemon(NewPokemon* unNewPokemon) {
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

		printf("\nEl tamaño de la lista ahora es de: %d\n", list_size(New_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();
		char* nombreDelPokemonQueGuarde = mensaje->contenidoDelMensaje->nombre;

		printf("[NewPokemon] Guarde el mensaje: %s\n", nombreDelPokemonQueGuarde);
		printf("[NewPokemon] Su ID es: %d\n",mensaje->ID);

		return mensaje;
}

MensajeLocalizedPokemon* guardarMensajeLocalizedPokemon(LocalizedPokemon* unLocalizedPokemon,int idCorrelativo){

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

		printf("\nEl tamaño de la lista ahora es de: %d\n", list_size(Localized_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();
		char* nombreDelPokemonQueGuarde = mensaje->contenidoDelMensaje->nombre;

		printf("[LocalizedPokemon] Guarde el mensaje: %s\n", nombreDelPokemonQueGuarde);
		printf("[LocalizedPokemon] Su ID es: %d, y su IDCorrelativo es: %d\n",mensaje->ID,mensaje->IDCorrelativo);

		return mensaje;
}

MensajeGetPokemon* guardarMensajeGetPokemon(GetPokemon* unGetPokemon){
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

		printf("\nEl tamaño de la lista ahora es de: %d\n", list_size(Get_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();
		char* nombreDelPokemonQueGuarde = mensaje->contenidoDelMensaje->nombre;

		printf("[GetPokemon] Guarde el mensaje: %s\n", nombreDelPokemonQueGuarde);
		printf("[GetPokemon] Su ID es: %d\n",mensaje->ID);

		return mensaje;
}


MensajeAppearedPokemon* guardarMensajeAppearedPokemon(AppearedPokemon* unAppearedPokemon){
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

		printf("\nEl tamaño de la lista ahora es de: %d\n", list_size(Appeared_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();
		char* nombreDelPokemonQueGuarde = mensaje->contenidoDelMensaje->nombre;

		printf("[AppearedPokemon] Guarde el mensaje: %s\n", nombreDelPokemonQueGuarde);
		printf("[AppearedPokemon] Su ID es: %d\n",mensaje->ID);

		return mensaje;
}



MensajeCatchPokemon* guardarMensajeCatchPokemon(CatchPokemon* unCatchPokemon){
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

		printf("\nEl tamaño de la lista ahora es de: %d\n", list_size(Catch_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();

		char* nombreDelPokemonQueGuarde = mensaje->contenidoDelMensaje->nombre;

		printf("[CatchPokemon] Guarde el mensaje: %s\n", nombreDelPokemonQueGuarde);
		printf("[CatchPokemon] Su ID es: %d\n",mensaje->ID);

		return mensaje;

}


MensajeCaughtPokemon* guardarMensajeCaughtPokemon(CaughtPokemon* unCaughtPokemon,int idCorrelativo){

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

		printf("\nEl tamaño de la lista ahora es de: %d\n", list_size(Caught_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();

		printf("[CaughtPokemon] Guarde el mensaje: %d\n",mensaje->contenidoDelMensaje->atrapar);
		printf("[CaughtPokemon] Su ID es: %d, y su IDCorrelativo es: %d\n",mensaje->ID,mensaje->IDCorrelativo);

		return mensaje;
}


void enviarColaNewPokemon(int idGeneradoEnElMomento,int socket_suscriptor,Suscriptor* unSuscriptor){
	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
		MensajeNewPokemon* mensaje;
		NewPokemon* unNewPokemon;
		int tamanioListaSuscriptoresACK;
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int cantidadDeNewPokemon = list_size(New_Pokemon);

		if(idGeneradoEnElMomento == 1){ //caso feliz
		send(socket_suscriptor,&cantidadDeNewPokemon,sizeof(int),0);
		for(int i=0;i<cantidadDeNewPokemon;i++){
			mensaje = list_get(New_Pokemon,i);
			printf("\n\nTe voy a enviar el mensaje: %s",mensaje->contenidoDelMensaje->nombre);

			pthread_mutex_lock(&mutexColaNewPokemon);
			unNewPokemon = mensaje->contenidoDelMensaje;
			enviarNewPokemon(unNewPokemon,socket_suscriptor,mensaje->ID);
			printf("\nMensaje enviado: %s",unNewPokemon->nombre);
			list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
			printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));

			loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
			printf("\nACK recibido: %d",ack);

			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("\nSe cayó la conexion...");
				pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
			}
			list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
			printf("\nEl suscriptor fue agregado a la lista de SuscriptoresACK, ACK del mensaje %d",mensaje->ID);

			pthread_mutex_unlock(&mutexColaNewPokemon);

			}
		} else{ //caso triste
			Suscriptor* suscriptorQueItera;
			for(int i=0;i<cantidadDeNewPokemon;i++){
				mensaje = list_get(New_Pokemon,i);
				tamanioListaSuscriptoresACK = list_size(mensaje->suscriptoresACK);
				for(int i=0;i<tamanioListaSuscriptoresACK;i++){
					suscriptorQueItera = list_get(mensaje->suscriptoresACK,i);
					if(suscriptorQueItera->IDsuscriptor == unSuscriptor->IDsuscriptor){
						cantidadDeNewPokemon--;
					}
				}
			}
			send(socket_suscriptor,&cantidadDeNewPokemon,sizeof(int),0);
			int mensajeYaEnviado = 0;
			for(int i=0;i<cantidadDeNewPokemon;i++){
				mensaje = list_get(New_Pokemon,i);
				for(int i=0;i<tamanioListaSuscriptoresACK;i++){
					suscriptorQueItera = list_get(mensaje->suscriptoresACK,i);
					if(suscriptorQueItera->IDsuscriptor == unSuscriptor->IDsuscriptor){
						mensajeYaEnviado = 1;
					}
				}
				if(mensajeYaEnviado == 0){
				printf("\nTe voy a enviar el mensaje: %s\n",mensaje->contenidoDelMensaje->nombre);

					pthread_mutex_lock(&mutexColaNewPokemon);
					unNewPokemon = mensaje->contenidoDelMensaje;
					enviarNewPokemon(unNewPokemon,socket_suscriptor,mensaje->ID);
					printf("Mensaje enviado: %s\n",unNewPokemon->nombre);
					list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
					printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d\n",list_size(mensaje->suscriptoresAtendidos));

					loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
					printf("ACK WHATTT recibido: %d\n",ack);

					if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
						puts("Se cayó la conexion...");
						pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
					}
					list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
					printf("El suscriptor fue agregado a la lista de SuscriptoresACK, ACK del mensaje %d\n",mensaje->ID);

					pthread_mutex_unlock(&mutexColaNewPokemon);
				}
				mensajeYaEnviado = 0;
			}
		} //fin del caso triste
		puts("\nSuscriptor ok");
}


void enviarColaLocalizedPokemon(int idGeneradoEnElMomento,int socket_suscriptor,Suscriptor* unSuscriptor){

	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
		MensajeLocalizedPokemon* mensaje;
		LocalizedPokemon* unLocalizedPokemon;
		int tamanioListaSuscriptoresACK;
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int cantidadDeLocalizedPokemon = list_size(Localized_Pokemon);

		if(idGeneradoEnElMomento == 0){ //caso feliz
			send(socket_suscriptor,&cantidadDeLocalizedPokemon,sizeof(int),0);
		for(int i=0;i<cantidadDeLocalizedPokemon;i++){
			mensaje = list_get(Localized_Pokemon,i);
			printf("\n\nTe voy a enviar el mensaje: %s",mensaje->contenidoDelMensaje->nombre);

			pthread_mutex_lock(&mutexColaLocalizedPokemon);
			unLocalizedPokemon = mensaje->contenidoDelMensaje;
			enviarLocalizedPokemon(unLocalizedPokemon,socket_suscriptor,mensaje->ID,mensaje->IDCorrelativo);
			printf("\nMensaje enviado: %s",unLocalizedPokemon->nombre);
			list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
			printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));

			loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
			printf("\nACK recibido: %d\n",ack);

			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("\nSe cayó la conexion...");
				pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
			}
			list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
			printf("El suscriptor fue agregado a la lista de SuscriptoresACK, ACK del mensaje %d\n",mensaje->ID);

			pthread_mutex_unlock(&mutexColaLocalizedPokemon);
			}
		} else{ //caso triste
				Suscriptor* suscriptorQueItera;
				for(int i=0;i<cantidadDeLocalizedPokemon;i++){
					mensaje = list_get(Localized_Pokemon,i);
					tamanioListaSuscriptoresACK = list_size(mensaje->suscriptoresACK);
					for(int i=0;i<tamanioListaSuscriptoresACK;i++){
						suscriptorQueItera = list_get(mensaje->suscriptoresACK,i);
						if(suscriptorQueItera->IDsuscriptor == unSuscriptor->IDsuscriptor){
							cantidadDeLocalizedPokemon--;
						}
					}
				}
				send(socket_suscriptor,&cantidadDeLocalizedPokemon,sizeof(int),0);
				int mensajeYaEnviado = 0;
					for(int i=0;i<cantidadDeLocalizedPokemon;i++){
						mensaje = list_get(Localized_Pokemon,i);
						for(int i=0;i<tamanioListaSuscriptoresACK;i++){
							suscriptorQueItera = list_get(mensaje->suscriptoresACK,i);
							if(suscriptorQueItera->IDsuscriptor == unSuscriptor->IDsuscriptor){
								mensajeYaEnviado = 1;
								}
							}
							if(mensajeYaEnviado == 0){
							printf("\n\nTe voy a enviar el mensaje: %s",mensaje->contenidoDelMensaje->nombre);

							pthread_mutex_lock(&mutexColaLocalizedPokemon);
							unLocalizedPokemon = mensaje->contenidoDelMensaje;
							enviarLocalizedPokemon(unLocalizedPokemon,socket_suscriptor,mensaje->ID,mensaje->IDCorrelativo);
							printf("\nMensaje enviado: %s",unLocalizedPokemon->nombre);
							list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
							printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));

							loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
							printf("\nACK recibido: %d\n",ack);

							if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
								puts("\nSe cayó la conexion...");
								pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
							}
							list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
							printf("El suscriptor fue agregado a la lista de SuscriptoresACK, ACK del mensaje %d\n",mensaje->ID);

							pthread_mutex_unlock(&mutexColaLocalizedPokemon);
							}
							mensajeYaEnviado = 0;
						}
					} //fin del caso triste
				puts("\nSuscriptor ok");
}

void enviarColaGetPokemon(int idGeneradoEnElMomento,int socket_suscriptor,Suscriptor* unSuscriptor){

	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
		MensajeGetPokemon* mensaje;
		GetPokemon* unGetPokemon;
		int tamanioListaSuscriptoresACK;
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int cantidadDeGetPokemon = list_size(Get_Pokemon);

		if(idGeneradoEnElMomento == 0){ //caso feliz
			send(socket_suscriptor,&cantidadDeGetPokemon,sizeof(int),0);
		for(int i=0;i<cantidadDeGetPokemon;i++){
			mensaje = list_get(Get_Pokemon,i);
			printf("\n\nTe voy a enviar el mensaje: %s",mensaje->contenidoDelMensaje->nombre);

			pthread_mutex_lock(&mutexColaGetPokemon);
			unGetPokemon = mensaje->contenidoDelMensaje;
			enviarGetPokemon(unGetPokemon,socket_suscriptor,mensaje->ID);
			printf("\nMensaje enviado: %s",unGetPokemon->nombre);
			list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
			printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));

			loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
			printf("\nACK recibido: %d\n",ack);

			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("\nSe cayó la conexion...");
				pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
			}
			list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
			printf("\nEl suscriptor fue agregado a la lista de SuscriptoresACK, ACK del mensaje %d\n",mensaje->ID);

			pthread_mutex_unlock(&mutexColaGetPokemon);
			}
		} else{ //caso triste
			Suscriptor* suscriptorQueItera;
				for(int i=0;i<cantidadDeGetPokemon;i++){
					mensaje = list_get(Get_Pokemon,i);
					tamanioListaSuscriptoresACK = list_size(mensaje->suscriptoresACK);
					for(int i=0;i<tamanioListaSuscriptoresACK;i++){
						suscriptorQueItera = list_get(mensaje->suscriptoresACK,i);
						if(suscriptorQueItera->IDsuscriptor == unSuscriptor->IDsuscriptor){
							cantidadDeGetPokemon--;
						}
					}
				}
				send(socket_suscriptor,&cantidadDeGetPokemon,sizeof(int),0);
				int mensajeYaEnviado = 0;
					for(int i=0;i<cantidadDeGetPokemon;i++){
						mensaje = list_get(Get_Pokemon,i);
						for(int i=0;i<tamanioListaSuscriptoresACK;i++){
							suscriptorQueItera = list_get(mensaje->suscriptoresACK,i);
							if(suscriptorQueItera->IDsuscriptor == unSuscriptor->IDsuscriptor){
								mensajeYaEnviado = 1;
							}
						}
						if(mensajeYaEnviado == 0){
						printf("\n\nTe voy a enviar el mensaje: %s",mensaje->contenidoDelMensaje->nombre);

						pthread_mutex_lock(&mutexColaGetPokemon);
						unGetPokemon = mensaje->contenidoDelMensaje;
						enviarGetPokemon(unGetPokemon,socket_suscriptor,mensaje->ID);
						printf("\nMensaje enviado: %s",unGetPokemon->nombre);
						list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
						printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));

						loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
						printf("\nACK recibido: %d\n",ack);

						if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
							puts("\nSe cayó la conexion...");
							pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
						}
						list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
						printf("\nEl suscriptor fue agregado a la lista de SuscriptoresACK, ACK del mensaje %d\n",mensaje->ID);

						pthread_mutex_unlock(&mutexColaGetPokemon);
						}
						mensajeYaEnviado = 0;
					}
				} //fin del caso triste
					puts("\nSuscriptor ok");
}


void enviarColaAppearedPokemon(int idGeneradoEnElMomento,int socket_suscriptor,Suscriptor* unSuscriptor){

	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
			MensajeAppearedPokemon* mensaje;
			AppearedPokemon* unAppearedPokemon;
			int tamanioListaSuscriptoresACK;
			int ack = 0;
			int loQueDevuelveElRecv = 0;
			int cantidadDeAppearedPokemon = list_size(Appeared_Pokemon);

			if(idGeneradoEnElMomento == 0){ //caso feliz
				send(socket_suscriptor,&cantidadDeAppearedPokemon,sizeof(int),0);
			for(int i=0;i<cantidadDeAppearedPokemon;i++){
				mensaje = list_get(Appeared_Pokemon,i);
				printf("\n\nTe voy a enviar el mensaje: %s",mensaje->contenidoDelMensaje->nombre);

				pthread_mutex_lock(&mutexColaAppearedPokemon);
				unAppearedPokemon = mensaje->contenidoDelMensaje;
				enviarAppearedPokemon(unAppearedPokemon,socket_suscriptor,mensaje->ID,0);
				printf("\nMensaje enviado: %s",unAppearedPokemon->nombre);
				list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
				printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));

				loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
				printf("\nACK recibido: %d\n",ack);

				if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
					puts("\nSe cayó la conexion...");
					pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
				}
				list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				printf("\nEl suscriptor fue agregado a la lista de SuscriptoresACK, ACK del mensaje %d\n",mensaje->ID);

				pthread_mutex_unlock(&mutexColaAppearedPokemon);
			}
			} else{ //caso triste
					Suscriptor* suscriptorQueItera;
						for(int i=0;i<cantidadDeAppearedPokemon;i++){
							mensaje = list_get(Appeared_Pokemon,i);
							tamanioListaSuscriptoresACK = list_size(mensaje->suscriptoresACK);
							for(int i=0;i<tamanioListaSuscriptoresACK;i++){
								suscriptorQueItera = list_get(mensaje->suscriptoresACK,i);
								if(suscriptorQueItera->IDsuscriptor == unSuscriptor->IDsuscriptor){
									cantidadDeAppearedPokemon--;
								}
							}
						}
						send(socket_suscriptor,&cantidadDeAppearedPokemon,sizeof(int),0);
						int mensajeYaEnviado = 0;
						for(int i=0;i<cantidadDeAppearedPokemon;i++){
							mensaje = list_get(Appeared_Pokemon,i);
							for(int i=0;i<tamanioListaSuscriptoresACK;i++){
								suscriptorQueItera = list_get(mensaje->suscriptoresACK,i);
								if(suscriptorQueItera->IDsuscriptor == unSuscriptor->IDsuscriptor){
									mensajeYaEnviado = 1;
								}
							}
							if(mensajeYaEnviado == 0){
							printf("\n\nTe voy a enviar el mensaje: %s",mensaje->contenidoDelMensaje->nombre);

							pthread_mutex_lock(&mutexColaAppearedPokemon);
							unAppearedPokemon = mensaje->contenidoDelMensaje;
							enviarAppearedPokemon(unAppearedPokemon,socket_suscriptor,mensaje->ID,0);
							printf("\nMensaje enviado: %s",unAppearedPokemon->nombre);
							list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
							printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));

							loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
							printf("\nACK recibido: %d\n",ack);

							if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
								puts("\nSe cayó la conexion...");
								pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
							}
							list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
							printf("\nEl suscriptor fue agregado a la lista de SuscriptoresACK, ACK del mensaje %d\n",mensaje->ID);

							pthread_mutex_unlock(&mutexColaAppearedPokemon);
						}
							mensajeYaEnviado = 0;
					}
				} //fin del caso triste
						puts("\nSuscriptor ok");
}

void enviarColaCatchPokemon(int idGeneradoEnElMomento,int socket_suscriptor,Suscriptor* unSuscriptor){
	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
			MensajeCatchPokemon* mensaje;
			CatchPokemon* unCatchPokemon;
			int tamanioListaSuscriptoresACK;
			int ack = 0;
			int loQueDevuelveElRecv = 0;
			int cantidadDeCatchPokemon = list_size(Catch_Pokemon);

			if(idGeneradoEnElMomento == 0){ //caso feliz
				send(socket_suscriptor,&cantidadDeCatchPokemon,sizeof(int),0);
			for(int i=0;i<cantidadDeCatchPokemon;i++){
				mensaje = list_get(Catch_Pokemon,i);
				printf("\n\nTe voy a enviar el mensaje: %s",mensaje->contenidoDelMensaje->nombre);

				pthread_mutex_lock(&mutexColaCatchPokemon);
				unCatchPokemon = mensaje->contenidoDelMensaje;
				enviarCatchPokemon(unCatchPokemon,socket_suscriptor,mensaje->ID);
				printf("\nMensaje enviado: %s",unCatchPokemon->nombre);
				list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
				printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));

				loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
				printf("\nACK recibido: %d\n",ack);

				if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("\nSe cayó la conexion...");
				pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
				}
				list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				printf("\nEl suscriptor fue agregado a la lista de SuscriptoresACK, ACK del mensaje %d\n",mensaje->ID);

				pthread_mutex_unlock(&mutexColaCatchPokemon);
			}
		} else{ //caso triste
				Suscriptor* suscriptorQueItera;
					for(int i=0;i<cantidadDeCatchPokemon;i++){
						mensaje = list_get(Catch_Pokemon,i);
						tamanioListaSuscriptoresACK = list_size(mensaje->suscriptoresACK);
						for(int i=0;i<tamanioListaSuscriptoresACK;i++){
							suscriptorQueItera = list_get(mensaje->suscriptoresACK,i);
							if(suscriptorQueItera->IDsuscriptor == unSuscriptor->IDsuscriptor){
								cantidadDeCatchPokemon--;
							}
						}
					}
					send(socket_suscriptor,&cantidadDeCatchPokemon,sizeof(int),0);
					int mensajeYaEnviado = 0;
					for(int i=0;i<cantidadDeCatchPokemon;i++){
						mensaje = list_get(Catch_Pokemon,i);
						for(int i=0;i<tamanioListaSuscriptoresACK;i++){
							suscriptorQueItera = list_get(mensaje->suscriptoresACK,i);
							if(suscriptorQueItera->IDsuscriptor == unSuscriptor->IDsuscriptor){
							mensajeYaEnviado = 1;
							}
						}
						if(mensajeYaEnviado == 0){
							printf("\n\nTe voy a enviar el mensaje: %s\n",mensaje->contenidoDelMensaje->nombre);

						pthread_mutex_lock(&mutexColaCatchPokemon);
						unCatchPokemon = mensaje->contenidoDelMensaje;
						enviarCatchPokemon(unCatchPokemon,socket_suscriptor,mensaje->ID);
						printf("\nMensaje enviado: %s",unCatchPokemon->nombre);
						list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
						printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));

						loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
						printf("\nACK recibido: %d\n",ack);

						if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
							puts("\nSe cayó la conexion...");
							pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
						}
						list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
						printf("\nEl suscriptor fue agregado a la lista de SuscriptoresACK, ACK del mensaje %d\n",mensaje->ID);

						pthread_mutex_unlock(&mutexColaCatchPokemon);
					}
						mensajeYaEnviado = 0;
				}
			} //fin del caso triste
					puts("\nSuscriptor ok");
}


void enviarColaCaughtPokemon(int idGeneradoEnElMomento,int socket_suscriptor,Suscriptor* unSuscriptor){
	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
			MensajeCaughtPokemon* mensaje;
			CaughtPokemon* unCaughtPokemon;
			int tamanioListaSuscriptoresACK;
			int ack = 0;
			int loQueDevuelveElRecv = 0;
			int cantidadDeCaughtPokemon = list_size(Caught_Pokemon);

			if(idGeneradoEnElMomento == 0){ //caso feliz
				send(socket_suscriptor,&cantidadDeCaughtPokemon,sizeof(int),0);
			for(int i=0;i<cantidadDeCaughtPokemon;i++){
				mensaje = list_get(Caught_Pokemon,i);
				printf("\n\nTe voy a enviar el mensaje si el pokemon fue atrapado(1) o no(0): %d",mensaje->contenidoDelMensaje->atrapar);

				pthread_mutex_lock(&mutexColaCaughtPokemon);
				unCaughtPokemon = mensaje->contenidoDelMensaje;
				enviarCaughtPokemon(unCaughtPokemon,socket_suscriptor,mensaje->ID,mensaje->IDCorrelativo);
				printf("\nMensaje enviado: %d",unCaughtPokemon->atrapar);
				list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
				printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));

				loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
				printf("\nACK recibido: %d\n",ack);

				if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
					puts("\nSe cayó la conexion...");
					pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
				}
				list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				printf("\nEl suscriptor fue agregado a la lista de SuscriptoresACK, ACK del mensaje %d\n",mensaje->ID);

				pthread_mutex_unlock(&mutexColaCaughtPokemon);
			}
		} else{ //caso triste
				Suscriptor* suscriptorQueItera;
					for(int i=0;i<cantidadDeCaughtPokemon;i++){
						mensaje = list_get(Caught_Pokemon,i);
						tamanioListaSuscriptoresACK = list_size(mensaje->suscriptoresACK);
						for(int i=0;i<tamanioListaSuscriptoresACK;i++){
							suscriptorQueItera = list_get(mensaje->suscriptoresACK,i);
							if(suscriptorQueItera->IDsuscriptor == unSuscriptor->IDsuscriptor){
								cantidadDeCaughtPokemon--;
							}
						}
					}
					send(socket_suscriptor,&cantidadDeCaughtPokemon,sizeof(int),0);
					int mensajeYaEnviado = 0;
					for(int i=0;i<cantidadDeCaughtPokemon;i++){
						mensaje = list_get(Caught_Pokemon,i);
						for(int i=0;i<tamanioListaSuscriptoresACK;i++){
							suscriptorQueItera = list_get(mensaje->suscriptoresACK,i);
							if(suscriptorQueItera->IDsuscriptor == unSuscriptor->IDsuscriptor){
								mensajeYaEnviado = 1;
							}
						}
						if(mensajeYaEnviado == 0){
						printf("\n\nTe voy a enviar el mensaje si el pokemon fue atrapado(1) o no(0): %d",mensaje->contenidoDelMensaje->atrapar);

						pthread_mutex_lock(&mutexColaCaughtPokemon);
						unCaughtPokemon = mensaje->contenidoDelMensaje;
						enviarCaughtPokemon(unCaughtPokemon,socket_suscriptor,mensaje->ID,mensaje->IDCorrelativo);
						printf("\nMensaje enviado: %d",unCaughtPokemon->atrapar);
						list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
						printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));

						loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
						printf("\nACK recibido: %d\n",ack);

						if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
							puts("\nSe cayó la conexion...");
							pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
						}
						list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
						printf("\nEl suscriptor fue agregado a la lista de SuscriptoresACK, ACK del mensaje %d\n",mensaje->ID);

						pthread_mutex_unlock(&mutexColaCaughtPokemon);
					}
						mensajeYaEnviado = 0;
				}
			} //fin del caso triste
				puts("\nSuscriptor ok");

	}

