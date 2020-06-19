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

	char* loQueVoyALoguear = "Se creo una conexión.";
	log_info(logger, loQueVoyALoguear);

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
	void* algoARecibir;

	NewPokemon* unNewPokemon;
	LocalizedPokemon* unLocalizedPokemon;
	GetPokemon* unGetPokemon;
	AppearedPokemon* unAppearedPokemon;
	CatchPokemon* unCatchPokemon;
	CaughtPokemon* unCaughtPokemon;

	MensajeNewPokemon2* mensajeNewPokemon2;
	MensajeLocalizedPokemon2* mensajeLocalizedPokemon2;
	MensajeGetPokemon2* unMensajeGetPokemon2;
	MensajeAppearedPokemon2* mensajeAppearedPokemon2;
	MensajeCatchPokemon2* mensajeCatchPokemon2;
	MensajeCaughtPokemon2* mensajeCaughtPokemon2;

	NewPokemonConIDs* newConIDs;
	LocalizedPokemonConIDs* unLocalizedPokemonConIDCorrelativo;
	GetPokemonConIDs* getConIDs;
	AppearedPokemonConIDs* appearedConIDs;
	CatchPokemonConIDs* catchConIDs;
	CaughtPokemonConIDs* unCaughtPokemonConIDCorrelativo;

	char* loQueVoyALoguear;
	int idCorrelativo;
	printf("Recibi el cod op: %i\n",cod_op);

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
					loQueVoyALoguear = "Recibi un mensaje NewPokemon.";
					log_info(logger, loQueVoyALoguear);
					newConIDs = recibir_NEW_POKEMON(cliente_fd, &size,0);
					unNewPokemon = newConIDs->newPokemon;
					free(newConIDs);
					mensajeNewPokemon2 = guardarMensajeNewPokemon(unNewPokemon);
					enviarNewPokemonASuscriptores(mensajeNewPokemon2);
					break;
				case LOCALIZED_POKEMON:
					loQueVoyALoguear = "Recibi un mensaje LocalizedPokemon.";
					log_info(logger, loQueVoyALoguear);
					unLocalizedPokemonConIDCorrelativo = recibir_LOCALIZED_POKEMON(cliente_fd,&size,0);
					unLocalizedPokemon = unLocalizedPokemonConIDCorrelativo->localizedPokemon;
					idCorrelativo = unLocalizedPokemonConIDCorrelativo->IDcorrelativo;
					free(unLocalizedPokemonConIDCorrelativo);
					mensajeLocalizedPokemon2 = guardarMensajeLocalizedPokemon(unLocalizedPokemon,idCorrelativo);
					enviarLocalizedPokemonASuscriptores(mensajeLocalizedPokemon2);
					break;
				case GET_POKEMON:
					loQueVoyALoguear = "Recibi un mensaje GetPokemon.";
					log_info(logger, loQueVoyALoguear);
					getConIDs = recibir_GET_POKEMON(cliente_fd,&size,0);
					unGetPokemon = getConIDs->getPokemon;
					free(getConIDs);
					unMensajeGetPokemon2 = guardarMensajeGetPokemon(unGetPokemon);
					devolverID(cliente_fd,unMensajeGetPokemon2->ID);
					enviarGetPokemonASuscriptores(unMensajeGetPokemon2);
					break;
				case APPEARED_POKEMON:
					loQueVoyALoguear = "Recibi un mensaje AppearedPokemon.";
					log_info(logger, loQueVoyALoguear);
					appearedConIDs = recibir_APPEARED_POKEMON(cliente_fd,&size,0,1);
					unAppearedPokemon = appearedConIDs->appearedPokemon;
					free(appearedConIDs);
					mensajeAppearedPokemon2 = guardarMensajeAppearedPokemon(unAppearedPokemon);
					enviarAppearedPokemonASuscriptores(mensajeAppearedPokemon2);
					break;
				case CATCH_POKEMON:
					loQueVoyALoguear = "Recibi un mensaje CatchPokemon.";
					log_info(logger, loQueVoyALoguear);
					catchConIDs = recibir_CATCH_POKEMON(cliente_fd,&size,0);
					unCatchPokemon = catchConIDs->catchPokemon;
					free(catchConIDs);
					mensajeCatchPokemon2 = guardarMensajeCatchPokemon(unCatchPokemon);
					devolverID(cliente_fd,mensajeCatchPokemon2->ID);
					enviarCatchPokemonASuscriptores(mensajeCatchPokemon2);
					break;
				case CAUGHT_POKEMON:
					loQueVoyALoguear = "Recibi un mensaje CaughtPokemon.";
					log_info(logger, loQueVoyALoguear);
					unCaughtPokemonConIDCorrelativo = recibir_CAUGHT_POKEMON(cliente_fd,&size,0);
					unCaughtPokemon = unCaughtPokemonConIDCorrelativo->caughtPokemon;
					idCorrelativo = unCaughtPokemonConIDCorrelativo->IDCorrelativo;
					free(unCaughtPokemonConIDCorrelativo);
					mensajeCaughtPokemon2 = guardarMensajeCaughtPokemon(unCaughtPokemon,idCorrelativo);
					enviarCaughtPokemonASuscriptores(mensajeCaughtPokemon2);
					break;
				case SUSCRIPTOR_NEWPOKEMON:
					loQueVoyALoguear = "Recibi suscripción para la cola de mensajes NewPokemon.";
					log_info(logger, loQueVoyALoguear);
					recibirSuscripcionNewPokemon(cliente_fd);
					break;
				case SUSCRIPTOR_LOCALIZEDPOKEMON:
					loQueVoyALoguear = "Recibi suscripción para la cola de mensajes LocalizedPokemon.";
					log_info(logger, loQueVoyALoguear);
					recibirSuscripcionLocalizedPokemon(cliente_fd);
					break;
				case SUSCRIPTOR_GETPOKEMON:
					loQueVoyALoguear = "Recibi suscripción para la cola de mensajes GetPokemon.";
					log_info(logger, loQueVoyALoguear);
					recibirSuscripcionGetPokemon(cliente_fd);
					break;
				case SUSCRIPTOR_APPEAREDPOKEMON:
					loQueVoyALoguear = "Recibi suscripción para la cola de mensajes AppearedPokemon.";
					log_info(logger, loQueVoyALoguear);
					recibirSuscripcionAppearedPokemon(cliente_fd);
					break;
				case SUSCRIPTOR_CATCHPOKEMON:
					loQueVoyALoguear = "Recibi suscripción para la cola de mensajes CatchPokemon.";
					log_info(logger, loQueVoyALoguear);
					recibirSuscripcionCatchPokemon(cliente_fd);
					break;
				case SUSCRIPTOR_CAUGHTPOKEMON:
					loQueVoyALoguear = "Recibi suscripción para la cola de mensajes CaughtPokemon.";
					log_info(logger, loQueVoyALoguear);
					recibirSuscripcionCaughtPokemon(cliente_fd);
					break;
	}
}

void devolverID (int socket, int id){
	send(socket,&id,sizeof(int),0);
}


void enviarCaughtPokemonASuscriptores(MensajeCaughtPokemon2* unMensajeCaughtPokemon){
		Suscriptor* unSuscriptor;
		pthread_mutex_lock(&mutexMemoriaInterna);
		CaughtPokemon* unCaughtPokemon = sacarDeMemoriaElCaughtPokemon(unMensajeCaughtPokemon);
		pthread_mutex_unlock(&mutexMemoriaInterna);
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int tamanioSuscriptoresCaughtPokemon = list_size(suscriptores_caught_pokemon);
		char* loQueVoyALoguear,loQueVoyALoguear2;
		printf("El tamaño de la lista suscriptores caught pokemon es %d",tamanioSuscriptoresCaughtPokemon);

		for(int i=0;i<tamanioSuscriptoresCaughtPokemon;i++){
			unSuscriptor = list_get(suscriptores_caught_pokemon,i);

			loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
			log_info(logger, loQueVoyALoguear,unMensajeCaughtPokemon->ID,unSuscriptor->socketSuscriptor);

			enviarCaughtPokemon(unCaughtPokemon,unSuscriptor->socketSuscriptor,unMensajeCaughtPokemon->ID,unMensajeCaughtPokemon->IDCorrelativo);
			list_add(unMensajeCaughtPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));

			loQueDevuelveElRecv = recv(unSuscriptor->socketSuscriptor,&ack,sizeof(int),MSG_WAITALL);
			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("Se cayó la conexion...");
			}
			if(ack == 1){
				list_add(unMensajeCaughtPokemon->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				loQueVoyALoguear2 = "El suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
				log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
			}
		}

		free(unCaughtPokemon);
}

void enviarCatchPokemonASuscriptores(MensajeCatchPokemon2* unMensajeCatchPokemon){
		Suscriptor* unSuscriptor;
		CatchPokemon* unCatchPokemon = sacarDeMemoriaElCatchPokemon(unMensajeCatchPokemon);
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int tamanioSuscriptoresCatchPokemon = list_size(suscriptores_catch_pokemon);
		char* loQueVoyALoguear,loQueVoyALoguear2;

		for(int i=0;i<tamanioSuscriptoresCatchPokemon;i++){
			unSuscriptor = list_get(suscriptores_catch_pokemon,i);

			loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
			log_info(logger, loQueVoyALoguear,unMensajeCatchPokemon->ID,unSuscriptor->socketSuscriptor);

			enviarCatchPokemon(unCatchPokemon,unSuscriptor->socketSuscriptor,unMensajeCatchPokemon->ID);
			list_add(unMensajeCatchPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));

			loQueDevuelveElRecv = recv(unSuscriptor->socketSuscriptor,&ack,sizeof(int),MSG_WAITALL);
			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("Se cayó la conexion...");
			}
			if(ack == 1){
				list_add(unMensajeCatchPokemon->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				loQueVoyALoguear2 = "El suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
				log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
			}
		}

		free(unCatchPokemon);
}

void enviarLocalizedPokemonASuscriptores(MensajeLocalizedPokemon2* unMensajeLocalizedPokemon){
		Suscriptor* unSuscriptor;
		pthread_mutex_lock(&mutexMemoriaInterna);
		LocalizedPokemon* unLocalizedPokemon = sacarDeMemoriaElLocalizedPokemon(unMensajeLocalizedPokemon);
		pthread_mutex_unlock(&mutexMemoriaInterna);
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int tamanioSuscriptoresLocalizedPokemon = list_size(suscriptores_localized_pokemon);
		char* loQueVoyALoguear,loQueVoyALoguear2;

		for(int i=0;i<tamanioSuscriptoresLocalizedPokemon;i++){
			unSuscriptor = list_get(suscriptores_localized_pokemon,i);

			loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
			log_info(logger, loQueVoyALoguear,unMensajeLocalizedPokemon->ID,unSuscriptor->socketSuscriptor);

			enviarLocalizedPokemon(unLocalizedPokemon,unSuscriptor->socketSuscriptor,unMensajeLocalizedPokemon->ID,unMensajeLocalizedPokemon->IDCorrelativo);
			list_add(unMensajeLocalizedPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));

			loQueDevuelveElRecv = recv(unSuscriptor->socketSuscriptor,&ack,sizeof(int),MSG_WAITALL);
			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("Se cayó la conexion...");
			}
			if(ack == 1){
				list_add(unMensajeLocalizedPokemon->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				loQueVoyALoguear2 = "El suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
				log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
			}
		}

		free(unLocalizedPokemon); //falta el quilombo extra
}


void enviarGetPokemonASuscriptores(MensajeGetPokemon2* unMensajeGetPokemon){
		Suscriptor* unSuscriptor;
		pthread_mutex_lock(&mutexMemoriaInterna);
		GetPokemon* unGetPokemon = sacarDeMemoriaElGetPokemon(unMensajeGetPokemon);
		pthread_mutex_unlock(&mutexMemoriaInterna);
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int tamanioSuscriptoresGetPokemon = list_size(suscriptores_get_pokemon);
		char* loQueVoyALoguear,loQueVoyALoguear2;

		for(int i=0;i<tamanioSuscriptoresGetPokemon;i++){
			unSuscriptor = list_get(suscriptores_get_pokemon,i);

			loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
			log_info(logger, loQueVoyALoguear,unMensajeGetPokemon->ID,unSuscriptor->socketSuscriptor);

			enviarGetPokemon(unGetPokemon,unSuscriptor->socketSuscriptor,unMensajeGetPokemon->ID);
			list_add(unMensajeGetPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));

			loQueDevuelveElRecv = recv(unSuscriptor->socketSuscriptor,&ack,sizeof(int),MSG_WAITALL);
			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("Se cayó la conexion...");
			}
			if(ack == 1){
				list_add(unMensajeGetPokemon->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				loQueVoyALoguear2 = "El suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
				log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
			}
		}

		free(unGetPokemon);
}

void enviarAppearedPokemonASuscriptores(MensajeAppearedPokemon2* unMensajeAppearedPokemon){
		Suscriptor* unSuscriptor;
		pthread_mutex_lock(&mutexMemoriaInterna);
		AppearedPokemon* unAppearedPokemon = sacarDeMemoriaElAppearedPokemon(unMensajeAppearedPokemon);
		pthread_mutex_unlock(&mutexMemoriaInterna);
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int tamanioSuscriptoresAppearedPokemon = list_size(suscriptores_appeared_pokemon);
		char* loQueVoyALoguear,loQueVoyALoguear2;

		for(int i=0;i<tamanioSuscriptoresAppearedPokemon;i++){
			unSuscriptor = list_get(suscriptores_appeared_pokemon,i);

			loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
			log_info(logger, loQueVoyALoguear,unMensajeAppearedPokemon->ID,unSuscriptor->socketSuscriptor);

			enviarAppearedPokemon(unAppearedPokemon,unSuscriptor->socketSuscriptor,unMensajeAppearedPokemon->ID,0);
			list_add(unMensajeAppearedPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));

			loQueDevuelveElRecv = recv(unSuscriptor->socketSuscriptor,&ack,sizeof(int),MSG_WAITALL);
			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("Se cayó la conexion...");
			}
			if(ack == 1){
				list_add(unMensajeAppearedPokemon->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				loQueVoyALoguear2 = "El suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
				log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
			}
		}

		free(unAppearedPokemon);
}

void enviarNewPokemonASuscriptores(MensajeNewPokemon2* unMensajeNewPokemon){
		Suscriptor* unSuscriptor;
		pthread_mutex_lock(&mutexMemoriaInterna);
		NewPokemon* unNewPokemon = sacarDeMemoriaElNewPokemon(unMensajeNewPokemon);
		pthread_mutex_unlock(&mutexMemoriaInterna);
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int tamanioSuscriptoresNewPokemon = list_size(suscriptores_new_pokemon);
		char* loQueVoyALoguear,loQueVoyALoguear2;

		for(int i=0;i<tamanioSuscriptoresNewPokemon;i++){
			unSuscriptor = list_get(suscriptores_new_pokemon,i);
			loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
			log_info(logger, loQueVoyALoguear,unMensajeNewPokemon->ID,unSuscriptor->socketSuscriptor);

			enviarNewPokemon(unNewPokemon,unSuscriptor->socketSuscriptor,unMensajeNewPokemon->ID);
			list_add(unMensajeNewPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
			puts("Suscriptor agregado a la lista de atendidos");

			loQueDevuelveElRecv = recv(unSuscriptor->socketSuscriptor,&ack,sizeof(int),MSG_WAITALL);
			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("Se cayó la conexion...");
			}
			if(ack == 1){
				list_add(unMensajeNewPokemon->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				loQueVoyALoguear2 = "El suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
				log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
			}
		}
		free(unNewPokemon);
}

void recibirSuscripcionCaughtPokemon(int socket_suscriptor){

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
}

void recibirSuscripcionCatchPokemon(int socket_suscriptor){

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
}

void recibirSuscripcionAppearedPokemon(int socket_suscriptor){

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
}

void recibirSuscripcionGetPokemon(int socket_suscriptor){

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
}


void recibirSuscripcionLocalizedPokemon(int socket_suscriptor){

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
}

void recibirSuscripcionNewPokemon(int socket_suscriptor){

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
}


void ocuparPosicion(int tamanio, void* posicion, int colaALaQuePertenece, int ID){
	PosicionOcupada* unaPosicion = malloc(sizeof(PosicionOcupada));
	char* loQueVoyALoguear;

	unaPosicion->ID = ID;
	unaPosicion->colaALaQuePertenece = colaALaQuePertenece;
	unaPosicion->posicion = posicion;
	if(tamanio>= tamanioMinimoParticion){
		unaPosicion->tamanio = tamanio;
	} else {
		unaPosicion->tamanio = tamanioMinimoParticion;
	}

	list_add(listaPosicionesOcupadas, unaPosicion);
	printf("\nOcupe exitosamente una posicion del mensaje con ID %d\n", unaPosicion->ID);
	uint32_t posicionRelativa = unaPosicion->posicion - memoriaInterna;
	printf("\nGuarde ese dato en la posicion de memoria %u\n", posicionRelativa);
	printf("\nAhora tengo %d posiciones ocupadas", list_size(listaPosicionesOcupadas));
	loQueVoyALoguear = "Almacené en memoria el mensaje con ID=%d,donde comienza en: %d";
	log_info(logger, loQueVoyALoguear,unaPosicion->ID,posicionRelativa);


}

//---------------------------------------------------Particiones Dinamicas

void borrarFIFO(){
	//pthread_mutex_lock(&mutexMemoriaInterna);
	int tamanioOcupados = list_size(listaPosicionesOcupadas);
	printf("\nhay %d posiciones ocupadas", list_size(listaPosicionesOcupadas));
	//pthread_mutex_unlock(&mutexMemoriaInterna);
	int posicionABorrar;
	int auxID = 99999999;
	PosicionOcupada* unaPosicionOcupada;
	for(int i=0; i<tamanioOcupados; i++){
		unaPosicionOcupada = list_get(listaPosicionesOcupadas,i);
		if(unaPosicionOcupada->ID < auxID){
			auxID = unaPosicionOcupada->ID;
			posicionABorrar = i;
		}
	}
	unaPosicionOcupada = list_get(listaPosicionesOcupadas,posicionABorrar);
	PosicionLibre* unaPosicionLibre = malloc(sizeof(PosicionLibre));
	unaPosicionLibre->posicion = unaPosicionOcupada->posicion;
	unaPosicionLibre->tamanio = unaPosicionOcupada->tamanio;
	borrarDeColaDeMensajes(unaPosicionOcupada->colaALaQuePertenece,unaPosicionOcupada->ID);
	free(unaPosicionOcupada);
	list_remove(listaPosicionesOcupadas,posicionABorrar);
	list_add(listaPosicionesLibres,unaPosicionLibre);
}

void borrarLRU(){

}

void borrarPosicion(){
	puts("estoy en borrar posicion");
	if(!strcmp(algoritmoReemplazo,"FIFO")){
		puts("voy a borrar por FIFO");
		borrarFIFO();
	} else{
		borrarLRU();
	}

}

PosicionLibre* pedirPosicionFF(int tamanio){
	PosicionLibre* unaPosicionLibre;
	PosicionLibre* posicionFalsa = malloc(sizeof(PosicionLibre));
	posicionFalsa->tamanio = 0;
	if(tamanio < tamanioMinimoParticion){
		tamanio= tamanioMinimoParticion;
	}
	int tamanioPosicionesLibres = list_size(listaPosicionesLibres);
	for(int i=0;i<tamanioPosicionesLibres;i++){
		unaPosicionLibre = list_get(listaPosicionesLibres,i);
		if (unaPosicionLibre->tamanio >= tamanio){
			free(posicionFalsa);
			puts("voy a retornar una posicion buena");
			return unaPosicionLibre;
		}
	}
	puts("voy a retornar una posicion falsa");
	return posicionFalsa;

}

PosicionLibre* pedirPosicionBestFit(int tamanio) {
	PosicionLibre* unaPosicionLibre;
	PosicionLibre* otraPosicionLibreBis;
	PosicionLibre* posicionQueVoyARetornar;
	PosicionLibre* posicionFalsa = malloc(sizeof(PosicionLibre));
	posicionFalsa->tamanio = 0;
	t_list* posicionesPotablesAElegir = list_create();
	int tamanioListaPosicionesPotables;
	int variableAux;
	int otraVariableAux;

	if(tamanio < tamanioMinimoParticion){
			tamanio = tamanioMinimoParticion;
	}

	int tamanioPosicionesLibres = list_size(listaPosicionesLibres);
	printf("las posiciones libres son %i", tamanioPosicionesLibres);
		for(int i=0;i<tamanioPosicionesLibres;i++){
			puts("entre al primer for");
			unaPosicionLibre = list_get(listaPosicionesLibres,i);
			if(unaPosicionLibre->tamanio >= tamanio){
				variableAux = unaPosicionLibre->tamanio - tamanio;
				if(variableAux > 0){
					list_add(posicionesPotablesAElegir,unaPosicionLibre);
				}else{
					free(posicionFalsa);
					return unaPosicionLibre;
				}
			}
		}
		tamanioListaPosicionesPotables = list_size(posicionesPotablesAElegir);
		printf("las posiciones potables son %i", tamanioListaPosicionesPotables);
		if (tamanioListaPosicionesPotables == 0){
			return posicionFalsa;
		}
		posicionQueVoyARetornar= list_get(posicionesPotablesAElegir,0);
		for(int i=1;i<tamanioListaPosicionesPotables;i++){
			puts("entre al segundo for");
			otraPosicionLibreBis = list_get(posicionesPotablesAElegir,i);
			variableAux = posicionQueVoyARetornar->tamanio - tamanio;
			otraVariableAux = otraPosicionLibreBis->tamanio - tamanio;
			if(variableAux > otraVariableAux){
				posicionQueVoyARetornar = otraPosicionLibreBis;
			}
		}
		list_destroy(posicionesPotablesAElegir);
		free(posicionFalsa);
		return posicionQueVoyARetornar;
}

PosicionLibre* pedirPosicion(int tamanio){

	PosicionLibre* posicionARetornar;
	//algoritmoParticionLibre = config_get_string_value(config,"ALGORITMO_PARTICION_LIBRE");
	if(!strcmp(algoritmoParticionLibre,"FF")){
		posicionARetornar = pedirPosicionFF(tamanio);
		if(posicionARetornar->tamanio == 0){
			puts("me llego una pos falsa");
			free(posicionARetornar);
			borrarPosicion();
			pthread_mutex_lock(&mutexBusquedasFallidas);
			busquedasFallidasPreviasACompactacion--;
			/*if(busquedasFallidasPreviasACompactacion == 0){
				compacta();
			}*/ //resolverCompactacion, dentro de compactar, reiniciar las busquedasfallidas
			pthread_mutex_unlock(&mutexBusquedasFallidas);
			return pedirPosicion(tamanio);
		}
		return posicionARetornar;
	}else {
		puts("estoy en el else de pedirPosicion");
		posicionARetornar = pedirPosicionBestFit(tamanio);
		if(posicionARetornar->tamanio == 0){
			free(posicionARetornar);
			borrarPosicion();
			pthread_mutex_lock(&mutexBusquedasFallidas);
			busquedasFallidasPreviasACompactacion--;
			/*if(busquedasFallidasPreviasACompactacion == 0){
				compacta();
			}*/ //resolverCompactacion, dentro de compactar, reiniciar las busquedasfallidas
			pthread_mutex_unlock(&mutexBusquedasFallidas);
			return pedirPosicion(tamanio);
		}
		return posicionARetornar;
	}
}



//---------------------------------------------------Particiones Dinamicas

void limpiarPosicionesEn0(){
	PosicionLibre* unaPosicionLibre;
	int tamanioPosicionesLibres = list_size(listaPosicionesLibres);
	for (int i =0; i<tamanioPosicionesLibres; i++){
		unaPosicionLibre = list_get(listaPosicionesLibres,i);
		if(unaPosicionLibre->tamanio == 0){
			free(unaPosicionLibre);
			list_remove(listaPosicionesLibres,i);
		}

	}
}

void grabarNewPokemonAMemoriaInterna(NewPokemon* unNewPokemon, int tamanio, PosicionLibre* unaPosicionLibre){

	memcpy(unaPosicionLibre->posicion,&(unNewPokemon->tamanioNombrePokemon),sizeof(uint32_t));
	unaPosicionLibre->posicion+=sizeof(uint32_t);
	memcpy(unaPosicionLibre->posicion,unNewPokemon->nombre,unNewPokemon->tamanioNombrePokemon);
	unaPosicionLibre->posicion+=unNewPokemon->tamanioNombrePokemon;
	memcpy(unaPosicionLibre->posicion,&(unNewPokemon->coordenadas.posicionX),sizeof(uint32_t));
	unaPosicionLibre->posicion+=sizeof(uint32_t);
	memcpy(unaPosicionLibre->posicion,&(unNewPokemon->coordenadas.posicionY),sizeof(uint32_t));
	unaPosicionLibre->posicion+=sizeof(uint32_t);
	memcpy(unaPosicionLibre->posicion,&(unNewPokemon->cantidad),sizeof(uint32_t));
	unaPosicionLibre->posicion+=sizeof(uint32_t);
	if(tamanio>=tamanioMinimoParticion){
		unaPosicionLibre->tamanio-= tamanio;
		puts("soy mayor que el tamanio minimo de particion");
	} else{
		unaPosicionLibre->tamanio-= tamanioMinimoParticion;
		unaPosicionLibre->posicion+= (tamanioMinimoParticion-tamanio);
	}
}

MensajeNewPokemon2* guardarMensajeNewPokemon(NewPokemon* unNewPokemon) {
		MensajeNewPokemon2* mensaje = malloc(sizeof(MensajeNewPokemon2));

		pthread_mutex_lock(&mutexGeneradorIDMensaje);
		generadorDeIDsMensaje++;
		mensaje->ID = generadorDeIDsMensaje;
		pthread_mutex_unlock(&mutexGeneradorIDMensaje);
		int tamanioDeNew = sizeof(uint32_t) * 4 + unNewPokemon->tamanioNombrePokemon;

		pthread_mutex_lock(&mutexMemoriaInterna);
		PosicionLibre* unaPosicionLibre = pedirPosicion(tamanioDeNew);
		printf("Me quedan %d bytes libres",unaPosicionLibre->tamanio);
		if(unaPosicionLibre->tamanio >= tamanioDeNew){
		void* principioContenidoDelMensaje;

		memcpy(&principioContenidoDelMensaje,&(unaPosicionLibre->posicion),sizeof(void*));
		mensaje->contenidoDelMensaje = principioContenidoDelMensaje;
		grabarNewPokemonAMemoriaInterna(unNewPokemon,tamanioDeNew,unaPosicionLibre);
		ocuparPosicion(tamanioDeNew,mensaje->contenidoDelMensaje,2,mensaje->ID);
		limpiarPosicionesEn0();
		pthread_mutex_unlock(&mutexMemoriaInterna);

		if(tamanioDeNew>=tamanioMinimoParticion){
			mensaje->tamanioEnMemoria-= tamanioDeNew;
		} else{
			mensaje->tamanioEnMemoria-= tamanioMinimoParticion;
		}

		pthread_mutex_lock(&mutexColaNewPokemon);
		list_add(New_Pokemon, mensaje);
		pthread_cond_signal(&no_vacioNP);
		pthread_mutex_unlock(&mutexColaNewPokemon);

		printf("\nEl tamaño de la lista ahora es de: %d\n", list_size(New_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();
		char* nombreDelPokemonQueGuarde = unNewPokemon->nombre;

		printf("[NewPokemon] Guarde el mensaje: %s\n", nombreDelPokemonQueGuarde);
		printf("[NewPokemon] Su ID es: %d\n",mensaje->ID);

		} else {
			puts("No hay memoria disponible.");
		}
		free(unNewPokemon);

		puts("\nGrabado en memoria correctamente.");

		return mensaje;
}

void grabarLocalizedPokemonAMemoriaInterna(LocalizedPokemon* unLocalizedPokemon, int tamanio, PosicionLibre* unaPosicionLibre){


	memcpy(unaPosicionLibre->posicion,&(unLocalizedPokemon->tamanioNombrePokemon),sizeof(uint32_t));
	unaPosicionLibre->posicion+= sizeof(uint32_t);
	printf("\nVoy a grabar en memoria el localized de tamaño: %d",unLocalizedPokemon->tamanioNombrePokemon);
	memcpy(unaPosicionLibre->posicion,unLocalizedPokemon->nombre,unLocalizedPokemon->tamanioNombrePokemon);
	unaPosicionLibre->posicion+= unLocalizedPokemon->tamanioNombrePokemon;
	printf("\nVoy a grabar en memoria la cantidad de pares: %d",unLocalizedPokemon->cantidadParesOrdenados);
	memcpy(unaPosicionLibre->posicion,&(unLocalizedPokemon->cantidadParesOrdenados),sizeof(uint32_t));
	unaPosicionLibre->posicion+= sizeof(uint32_t);
	for(int i=0;i<unLocalizedPokemon->cantidadParesOrdenados;i++){
		CoordenadasXY* coordenadas = list_get(unLocalizedPokemon->paresOrdenados,i);
		printf ("\nVoy a grabar a memoria interna X: %d, Y: %d",coordenadas->posicionX, coordenadas->posicionY);
		memcpy(unaPosicionLibre->posicion,&(coordenadas->posicionX),sizeof(uint32_t));
		unaPosicionLibre->posicion+= sizeof(uint32_t);
		memcpy(unaPosicionLibre->posicion,&(coordenadas->posicionY),sizeof(uint32_t));
		unaPosicionLibre->posicion+= sizeof(uint32_t);
	}
	if(tamanio>=tamanioMinimoParticion){
		unaPosicionLibre->tamanio-= tamanio;
	} else{
		unaPosicionLibre->tamanio-= tamanioMinimoParticion;
		unaPosicionLibre->posicion+= (tamanioMinimoParticion-tamanio);
	}

}

MensajeLocalizedPokemon2* guardarMensajeLocalizedPokemon(LocalizedPokemon* unLocalizedPokemon,int idCorrelativo){
		MensajeLocalizedPokemon2* mensaje = malloc(sizeof(MensajeLocalizedPokemon2));
		pthread_mutex_lock(&mutexGeneradorIDMensaje);
			generadorDeIDsMensaje++;
			mensaje->ID = generadorDeIDsMensaje;
			mensaje->IDCorrelativo = idCorrelativo;
		pthread_mutex_unlock(&mutexGeneradorIDMensaje);
		int tamanioParesOrdenados = list_size(unLocalizedPokemon->paresOrdenados);
		int tamanioDeLocalized = sizeof(uint32_t) * tamanioParesOrdenados * 2 + (unLocalizedPokemon->tamanioNombrePokemon);

		pthread_mutex_lock(&mutexMemoriaInterna);

		PosicionLibre* unaPosicionLibre = pedirPosicion(tamanioDeLocalized);
		printf("Me quedan %d bytes libres",unaPosicionLibre->tamanio);
		if(unaPosicionLibre->tamanio >= tamanioDeLocalized){
		void* principioContenidoDelMensaje;

		memcpy(&principioContenidoDelMensaje,&(unaPosicionLibre->posicion),sizeof(void*));
		mensaje->contenidoDelMensaje = principioContenidoDelMensaje;
		grabarLocalizedPokemonAMemoriaInterna(unLocalizedPokemon,tamanioDeLocalized,unaPosicionLibre);
		ocuparPosicion(tamanioDeLocalized,mensaje->contenidoDelMensaje,3,mensaje->ID);
		limpiarPosicionesEn0();
		pthread_mutex_unlock(&mutexMemoriaInterna);


		if(tamanioDeLocalized>=tamanioMinimoParticion){
			mensaje->tamanioEnMemoria-= tamanioDeLocalized;
		} else{
			mensaje->tamanioEnMemoria-= tamanioMinimoParticion;
		}


		pthread_mutex_lock(&mutexColaLocalizedPokemon);
		list_add(Localized_Pokemon, mensaje);
		pthread_cond_signal(&no_vacioLP);
		pthread_mutex_unlock(&mutexColaLocalizedPokemon);

		printf("\nEl tamaño de la lista ahora es de: %d\n", list_size(Localized_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();
		char* nombreDelPokemonQueGuarde = unLocalizedPokemon->nombre;

		printf("[LocalizedPokemon] Guarde el mensaje: %s\n", nombreDelPokemonQueGuarde);
		printf("[LocalizedPokemon] Su ID es: %d, y su IDCorrelativo es: %d\n",mensaje->ID,mensaje->IDCorrelativo);

		} else {
			puts("No hay memoria disponible.");
		}

		free(unLocalizedPokemon);

		puts("\nGrabado en memoria correctamente.");

		return mensaje;
}

void grabarGetPokemonAMemoriaInterna(GetPokemon* unGetPokemon, int tamanio, PosicionLibre* unaPosicionLibre){

	memcpy(unaPosicionLibre->posicion,&(unGetPokemon->tamanioNombrePokemon),sizeof(uint32_t));
	unaPosicionLibre->posicion+= sizeof(uint32_t);
	memcpy(unaPosicionLibre->posicion,unGetPokemon->nombre,unGetPokemon->tamanioNombrePokemon);
	unaPosicionLibre->posicion+= unGetPokemon->tamanioNombrePokemon;

	if(tamanio>=tamanioMinimoParticion){
		unaPosicionLibre->tamanio-= tamanio;
	} else{
		unaPosicionLibre->tamanio-= tamanioMinimoParticion;
		unaPosicionLibre->posicion+= (tamanioMinimoParticion-tamanio);
	}

}

MensajeGetPokemon2* guardarMensajeGetPokemon(GetPokemon* unGetPokemon){
		MensajeGetPokemon2* mensaje = malloc(sizeof(MensajeGetPokemon2));
		pthread_mutex_lock(&mutexGeneradorIDMensaje);
				generadorDeIDsMensaje++;
				mensaje->ID = generadorDeIDsMensaje;
				pthread_mutex_unlock(&mutexGeneradorIDMensaje);

		int tamanioDeGet = sizeof(uint32_t) + unGetPokemon->tamanioNombrePokemon;
		pthread_mutex_lock(&mutexMemoriaInterna);
		PosicionLibre* unaPosicionLibre = pedirPosicion(tamanioDeGet);
		printf("Me quedan %d bytes libres",unaPosicionLibre->tamanio);
		if(unaPosicionLibre->tamanio >= tamanioDeGet){
		void* principioContenidoDelMensaje;

		memcpy(&principioContenidoDelMensaje,&(unaPosicionLibre->posicion),sizeof(void*));
		mensaje->contenidoDelMensaje = principioContenidoDelMensaje;
		grabarGetPokemonAMemoriaInterna(unGetPokemon,tamanioDeGet,unaPosicionLibre);
		ocuparPosicion(tamanioDeGet,mensaje->contenidoDelMensaje,4,mensaje->ID);
		limpiarPosicionesEn0();
		pthread_mutex_unlock(&mutexMemoriaInterna);

		if(tamanioDeGet>=tamanioMinimoParticion){
			mensaje->tamanioEnMemoria-= tamanioDeGet;
		} else{
			mensaje->tamanioEnMemoria-= tamanioMinimoParticion;
		}




		pthread_mutex_lock(&mutexColaGetPokemon);
		list_add(Get_Pokemon, mensaje);
		pthread_cond_signal(&no_vacioGP);
		pthread_mutex_unlock(&mutexColaGetPokemon);

		printf("\nEl tamaño de la lista ahora es de: %d\n", list_size(Get_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();
		char* nombreDelPokemonQueGuarde = unGetPokemon->nombre;

		printf("[GetPokemon] Guarde el mensaje: %s\n", nombreDelPokemonQueGuarde);
		printf("[GetPokemon] Su ID es: %d\n",mensaje->ID);

		} else {
			puts("No hay memoria disponible.");
		}

		free(unGetPokemon);

		puts("\nGrabado en memoria correctamente.");

		return mensaje;
}

void grabarAppearedPokemonAMemoriaInterna(AppearedPokemon* unAppearedPokemon, int tamanio,PosicionLibre* unaPosicionLibre){

	memcpy(unaPosicionLibre->posicion,&(unAppearedPokemon->tamanioNombrePokemon),sizeof(uint32_t));
	unaPosicionLibre->posicion+= sizeof(uint32_t);
	memcpy(unaPosicionLibre->posicion,unAppearedPokemon->nombre,unAppearedPokemon->tamanioNombrePokemon);
	unaPosicionLibre->posicion+= unAppearedPokemon->tamanioNombrePokemon;
	memcpy(unaPosicionLibre->posicion,&(unAppearedPokemon->coordenadas.posicionX),sizeof(uint32_t));
	unaPosicionLibre->posicion+= sizeof(uint32_t);
	memcpy(unaPosicionLibre->posicion,&(unAppearedPokemon->coordenadas.posicionY),sizeof(uint32_t));
	unaPosicionLibre->posicion+= sizeof(uint32_t);

	if(tamanio>=tamanioMinimoParticion){
		unaPosicionLibre->tamanio-= tamanio;
	} else{
		unaPosicionLibre->tamanio-= tamanioMinimoParticion;
		unaPosicionLibre->posicion+= (tamanioMinimoParticion-tamanio);
	}

}

MensajeAppearedPokemon2* guardarMensajeAppearedPokemon(AppearedPokemon* unAppearedPokemon){
		MensajeAppearedPokemon2* mensaje = malloc(sizeof(MensajeAppearedPokemon2));
		pthread_mutex_lock(&mutexGeneradorIDMensaje);
				generadorDeIDsMensaje++;
				mensaje->ID = generadorDeIDsMensaje;
				pthread_mutex_unlock(&mutexGeneradorIDMensaje);
		int tamanioDeAppeared = sizeof(uint32_t) * 3 + unAppearedPokemon->tamanioNombrePokemon;
		pthread_mutex_lock(&mutexMemoriaInterna);
		PosicionLibre* unaPosicionLibre = pedirPosicion(tamanioDeAppeared);
		printf("Me quedan %d bytes libres",unaPosicionLibre->tamanio);
		if(unaPosicionLibre->tamanio >= tamanioDeAppeared){
		void* principioContenidoDelMensaje;

		memcpy(&principioContenidoDelMensaje,&(unaPosicionLibre->posicion),sizeof(void*));
		mensaje->contenidoDelMensaje = principioContenidoDelMensaje;
		grabarAppearedPokemonAMemoriaInterna(unAppearedPokemon,tamanioDeAppeared,unaPosicionLibre);
		ocuparPosicion(tamanioDeAppeared,mensaje->contenidoDelMensaje,5,mensaje->ID);
		limpiarPosicionesEn0();
		pthread_mutex_unlock(&mutexMemoriaInterna);
		if(tamanioDeAppeared>=tamanioMinimoParticion){
			mensaje->tamanioEnMemoria-= tamanioDeAppeared;
		} else{
			mensaje->tamanioEnMemoria-= tamanioMinimoParticion;
		}


		pthread_mutex_lock(&mutexColaAppearedPokemon);
		list_add(Appeared_Pokemon, mensaje);
		pthread_cond_signal(&no_vacioAP);
		pthread_mutex_unlock(&mutexColaAppearedPokemon);

		printf("\nEl tamaño de la lista ahora es de: %d\n", list_size(Appeared_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();
		char* nombreDelPokemonQueGuarde = unAppearedPokemon->nombre;

		printf("[AppearedPokemon] Guarde el mensaje: %s\n", nombreDelPokemonQueGuarde);
		printf("[AppearedPokemon] Su ID es: %d\n",mensaje->ID);
		} else {
			puts("No hay memoria disponible.");
		}

		free(unAppearedPokemon);

		puts("\nGrabado en memoria correctamente.");

		return mensaje;
}

void grabarCatchPokemonAMemoriaInterna(CatchPokemon* unCatchPokemon, int tamanio, PosicionLibre* unaPosicionLibre){

	memcpy(unaPosicionLibre->posicion,&(unCatchPokemon->tamanioNombrePokemon),sizeof(uint32_t));
	unaPosicionLibre->posicion+= sizeof(uint32_t);
	memcpy(unaPosicionLibre->posicion,unCatchPokemon->nombre,unCatchPokemon->tamanioNombrePokemon);
	unaPosicionLibre->posicion+= unCatchPokemon->tamanioNombrePokemon;
	memcpy(unaPosicionLibre->posicion,&(unCatchPokemon->coordenadas.posicionX),sizeof(uint32_t));
	unaPosicionLibre->posicion+= sizeof(uint32_t);
	memcpy(unaPosicionLibre->posicion,&(unCatchPokemon->coordenadas.posicionY),sizeof(uint32_t));
	unaPosicionLibre->posicion+= sizeof(uint32_t);

	if(tamanio>=tamanioMinimoParticion){
		unaPosicionLibre->tamanio-= tamanio;
	} else{
		unaPosicionLibre->tamanio-= tamanioMinimoParticion;
		unaPosicionLibre->posicion+= (tamanioMinimoParticion-tamanio);
	}

}

MensajeCatchPokemon2* guardarMensajeCatchPokemon(CatchPokemon* unCatchPokemon){
		MensajeCatchPokemon2* mensaje = malloc(sizeof(MensajeCatchPokemon2));
		pthread_mutex_lock(&mutexGeneradorIDMensaje);
		generadorDeIDsMensaje++;
		mensaje->ID = generadorDeIDsMensaje;
		pthread_mutex_unlock(&mutexGeneradorIDMensaje);
		int tamanioDeCatch = sizeof(uint32_t) * 3 + unCatchPokemon->tamanioNombrePokemon;
		pthread_mutex_lock(&mutexMemoriaInterna);
		PosicionLibre* unaPosicionLibre = pedirPosicion(tamanioDeCatch);
		printf("Me quedan %d bytes libres",unaPosicionLibre->tamanio);
		if(unaPosicionLibre->tamanio >= tamanioDeCatch){
		void* principioContenidoDelMensaje;

		memcpy(&principioContenidoDelMensaje,&(unaPosicionLibre->posicion),sizeof(void*));
		mensaje->contenidoDelMensaje = principioContenidoDelMensaje;
		grabarCatchPokemonAMemoriaInterna(unCatchPokemon,tamanioDeCatch,unaPosicionLibre);
		ocuparPosicion(tamanioDeCatch,mensaje->contenidoDelMensaje,6,mensaje->ID);
		limpiarPosicionesEn0();
		pthread_mutex_unlock(&mutexMemoriaInterna);
		if(tamanioDeCatch>=tamanioMinimoParticion){
			mensaje->tamanioEnMemoria-= tamanioDeCatch;
		} else{
			mensaje->tamanioEnMemoria-= tamanioMinimoParticion;
		}


		pthread_mutex_lock(&mutexColaCatchPokemon);
		list_add(Catch_Pokemon, mensaje);
		pthread_cond_signal(&no_vacioCP);
		pthread_mutex_unlock(&mutexColaCatchPokemon);

		printf("\nEl tamaño de la lista ahora es de: %d\n", list_size(Catch_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();
		char* nombreDelPokemonQueGuarde = unCatchPokemon->nombre;

		printf("[CatchPokemon] Guarde el mensaje: %s\n", nombreDelPokemonQueGuarde);
		printf("[CatchPokemon] Su ID es: %d\n",mensaje->ID);
		} else {
			puts("No hay memoria disponible.");
		}

		free(unCatchPokemon);

		puts("\nGrabado en memoria correctamente.");

		return mensaje;

}

void grabarCaughtPokemonAMemoriaInterna(CaughtPokemon* unCaughtPokemon, int tamanio,PosicionLibre* unaPosicionLibre){

	memcpy(unaPosicionLibre->posicion,&(unCaughtPokemon->atrapar),sizeof(uint32_t));
	unaPosicionLibre->posicion+= sizeof(uint32_t);

	if(tamanio>=tamanioMinimoParticion){
		unaPosicionLibre->tamanio-= tamanio;
	} else{
		unaPosicionLibre->tamanio-= tamanioMinimoParticion;
		unaPosicionLibre->posicion+= (tamanioMinimoParticion-tamanio);
	}

}

MensajeCaughtPokemon2* guardarMensajeCaughtPokemon(CaughtPokemon* unCaughtPokemon,int idCorrelativo){

		MensajeCaughtPokemon2* mensaje = malloc(sizeof(MensajeCaughtPokemon2));
		pthread_mutex_lock(&mutexGeneradorIDMensaje);
				generadorDeIDsMensaje++;
				mensaje->ID = generadorDeIDsMensaje;
				mensaje->IDCorrelativo = idCorrelativo;
				pthread_mutex_unlock(&mutexGeneradorIDMensaje);
		int tamanioDeCaught = sizeof(uint32_t);

		pthread_mutex_lock(&mutexMemoriaInterna);
		PosicionLibre* unaPosicionLibre = pedirPosicion(tamanioDeCaught);
		printf("Me quedan %d bytes libres",unaPosicionLibre->tamanio);
		if(unaPosicionLibre->tamanio >= tamanioDeCaught){
		void* principioContenidoDelMensaje;

		memcpy(&principioContenidoDelMensaje,&(unaPosicionLibre->posicion),sizeof(void*));
		mensaje->contenidoDelMensaje = principioContenidoDelMensaje;

		grabarCaughtPokemonAMemoriaInterna(unCaughtPokemon,tamanioDeCaught,unaPosicionLibre);
		ocuparPosicion(tamanioDeCaught,mensaje->contenidoDelMensaje,CAUGHT_POKEMON,mensaje->ID);
		limpiarPosicionesEn0();
		pthread_mutex_unlock(&mutexMemoriaInterna);

		if(tamanioDeCaught>=tamanioMinimoParticion){
			mensaje->tamanioEnMemoria-= tamanioDeCaught;
		} else{
			mensaje->tamanioEnMemoria-= tamanioMinimoParticion;
		}


		pthread_mutex_lock(&mutexColaCaughtPokemon);
		list_add(Caught_Pokemon, mensaje);
		pthread_cond_signal(&no_vacioCAP);
		pthread_mutex_unlock(&mutexColaCaughtPokemon);

		printf("\nEl tamaño de la lista ahora es de: %d\n", list_size(Caught_Pokemon));
		mensaje->suscriptoresAtendidos = list_create();
		mensaje->suscriptoresACK = list_create();

		printf("[CaughtPokemon] Guarde el mensaje: %d\n",unCaughtPokemon->atrapar);
		printf("[CaughtPokemon] Su ID es: %d, y su IDCorrelativo es: %d\n",mensaje->ID,mensaje->IDCorrelativo);

		} else {
			puts("No hay memoria disponible.");
		}

		free(unCaughtPokemon);

		puts("\nGrabado en memoria correctamente.");

		return mensaje;
}

NewPokemon* sacarDeMemoriaElNewPokemon(MensajeNewPokemon2* mensajeNew2){
		NewPokemon* unNewPokemon = malloc (sizeof(NewPokemon));
		void* inicioDelNP = mensajeNew2->contenidoDelMensaje;
		int offset = 0;

		memcpy(&(unNewPokemon->tamanioNombrePokemon),inicioDelNP + offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		unNewPokemon->nombre = malloc(unNewPokemon->tamanioNombrePokemon);
		memcpy(unNewPokemon->nombre,inicioDelNP+offset,unNewPokemon->tamanioNombrePokemon);
		offset+=unNewPokemon->tamanioNombrePokemon;
		memcpy(&(unNewPokemon->coordenadas.posicionX),inicioDelNP+offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		printf("Saque de memoria el posX de un new: %d",unNewPokemon->coordenadas.posicionX);
		memcpy(&(unNewPokemon->coordenadas.posicionY),inicioDelNP+offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		memcpy(&(unNewPokemon->cantidad),inicioDelNP+offset,sizeof(uint32_t));

		return unNewPokemon;
}

AppearedPokemon* sacarDeMemoriaElAppearedPokemon(MensajeAppearedPokemon2* mensajeAppeared2){
		AppearedPokemon* unAppearedPokemon = malloc (sizeof(AppearedPokemon));
		void* inicioDelAP = mensajeAppeared2->contenidoDelMensaje;
		int offset = 0;

		memcpy(&(unAppearedPokemon->tamanioNombrePokemon),inicioDelAP + offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		unAppearedPokemon->nombre = malloc(unAppearedPokemon->tamanioNombrePokemon);
		memcpy(unAppearedPokemon->nombre,inicioDelAP+offset,unAppearedPokemon->tamanioNombrePokemon);
		offset+=unAppearedPokemon->tamanioNombrePokemon;
		memcpy(&(unAppearedPokemon->coordenadas.posicionX),inicioDelAP+offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		memcpy(&(unAppearedPokemon->coordenadas.posicionY),inicioDelAP+offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);

		return unAppearedPokemon;
}

LocalizedPokemon* sacarDeMemoriaElLocalizedPokemon(MensajeLocalizedPokemon2* mensajeLocalized2){
		LocalizedPokemon* unLocalizedPokemon = malloc (sizeof(LocalizedPokemon));
		void* inicioDelLP = mensajeLocalized2->contenidoDelMensaje;
		int offset = 0;

		memcpy(&(unLocalizedPokemon->tamanioNombrePokemon),inicioDelLP+offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		printf("\nEl tamaño del localized en memoria es %d: ",unLocalizedPokemon->tamanioNombrePokemon);
		unLocalizedPokemon->nombre = malloc(unLocalizedPokemon->tamanioNombrePokemon);
		memcpy(unLocalizedPokemon->nombre,inicioDelLP+offset,unLocalizedPokemon->tamanioNombrePokemon);
		offset+=unLocalizedPokemon->tamanioNombrePokemon;
		memcpy(&(unLocalizedPokemon->cantidadParesOrdenados),inicioDelLP+offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);

		int cantidadDePares = (unLocalizedPokemon->cantidadParesOrdenados);
		t_list* unasCoordenadas = list_create();

		for(int i=0;i<cantidadDePares;i++){
			CoordenadasXY* coordenadas = malloc(sizeof(CoordenadasXY));
			memcpy(&(coordenadas->posicionX),inicioDelLP+offset,sizeof(uint32_t));
			offset+=sizeof(uint32_t);
			memcpy(&(coordenadas->posicionY),inicioDelLP+offset,sizeof(uint32_t));
			offset+=sizeof(uint32_t);
			list_add(unasCoordenadas,coordenadas);
		}
		unLocalizedPokemon->paresOrdenados = unasCoordenadas;
		return unLocalizedPokemon;
}

GetPokemon* sacarDeMemoriaElGetPokemon(MensajeGetPokemon2* mensajeGet2){
		GetPokemon* unGetPokemon = malloc (sizeof(GetPokemon));
		void* inicioDelGP = mensajeGet2->contenidoDelMensaje;
		int offset = 0;

		memcpy(&(unGetPokemon->tamanioNombrePokemon),inicioDelGP + offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		unGetPokemon->nombre = malloc(unGetPokemon->tamanioNombrePokemon);
		memcpy(unGetPokemon->nombre,inicioDelGP+offset,unGetPokemon->tamanioNombrePokemon);
		offset+=unGetPokemon->tamanioNombrePokemon;

		return unGetPokemon;
}

CatchPokemon* sacarDeMemoriaElCatchPokemon(MensajeCatchPokemon2* mensajeCatch2){
		CatchPokemon* unCatchPokemon = malloc (sizeof(CatchPokemon));
		void* inicioDelCP = mensajeCatch2->contenidoDelMensaje;
		int offset = 0;

		memcpy(&(unCatchPokemon->tamanioNombrePokemon),inicioDelCP + offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		unCatchPokemon->nombre = malloc(unCatchPokemon->tamanioNombrePokemon);
		memcpy(unCatchPokemon->nombre,inicioDelCP+offset,unCatchPokemon->tamanioNombrePokemon);
		offset+=unCatchPokemon->tamanioNombrePokemon;
		memcpy(&(unCatchPokemon->coordenadas.posicionX),inicioDelCP+offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		memcpy(&(unCatchPokemon->coordenadas.posicionY),inicioDelCP+offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);

		return unCatchPokemon;
}

CaughtPokemon* sacarDeMemoriaElCaughtPokemon(MensajeCaughtPokemon2* mensajeCaught2){
		CaughtPokemon* unCaughtPokemon = malloc (sizeof(CaughtPokemon));
		void* inicioDelCAP = mensajeCaught2->contenidoDelMensaje;
		int offset = 0;

		memcpy(&(unCaughtPokemon->atrapar),inicioDelCAP + offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);

		return unCaughtPokemon;
}

void enviarColaNewPokemon(int idGeneradoEnElMomento,int socket_suscriptor,Suscriptor* unSuscriptor){
	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
		MensajeNewPokemon2* mensaje;
		NewPokemon* unNewPokemon;
		int tamanioListaSuscriptoresACK;
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int cantidadDeNewPokemon = list_size(New_Pokemon);
		char* loQueVoyALoguear,loQueVoyALoguear2;

		if(idGeneradoEnElMomento == 1){ //caso feliz
		send(socket_suscriptor,&cantidadDeNewPokemon,sizeof(int),0);
		for(int i=0;i<cantidadDeNewPokemon;i++){
			mensaje = list_get(New_Pokemon,i);
			pthread_mutex_lock(&mutexMemoriaInterna);
			unNewPokemon = sacarDeMemoriaElNewPokemon(mensaje);
			pthread_mutex_unlock(&mutexMemoriaInterna);

			loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
			log_info(logger, loQueVoyALoguear,mensaje->ID,unSuscriptor->socketSuscriptor);

			printf("\n\nNombre: %s",unNewPokemon->nombre);
			pthread_mutex_lock(&mutexColaNewPokemon);
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
			loQueVoyALoguear2 = "ACK recibido,el suscriptor con ID= %d fue agregado a la lista de SuscriptoresACK";
			log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);

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

					pthread_mutex_lock(&mutexColaNewPokemon);
					pthread_mutex_lock(&mutexMemoriaInterna);
					unNewPokemon = sacarDeMemoriaElNewPokemon(mensaje);
					pthread_mutex_unlock(&mutexMemoriaInterna);

					loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
					log_info(logger, loQueVoyALoguear,mensaje->ID,unSuscriptor->socketSuscriptor);

					printf("\n\nNombre: %s",unNewPokemon->nombre);
					enviarNewPokemon(unNewPokemon,socket_suscriptor,mensaje->ID);
					printf("Mensaje enviado: %s\n",unNewPokemon->nombre);
					list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
					printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d\n",list_size(mensaje->suscriptoresAtendidos));

					loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
					printf("ACK recibido: %d\n",ack);

					if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
						puts("Se cayó la conexion...");
						pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
					}
					list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
					loQueVoyALoguear2 = "ACK recibido,el suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
					log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
					pthread_mutex_unlock(&mutexColaNewPokemon);
				}
				mensajeYaEnviado = 0;
			}
		} //fin del caso triste
		free(unNewPokemon);
		puts("\nSuscriptor ok");
}


void enviarColaLocalizedPokemon(int idGeneradoEnElMomento,int socket_suscriptor,Suscriptor* unSuscriptor){

	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
		MensajeLocalizedPokemon2* mensaje;
		LocalizedPokemon* unLocalizedPokemon;
		int tamanioListaSuscriptoresACK;
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int cantidadDeLocalizedPokemon = list_size(Localized_Pokemon);
		char* loQueVoyALoguear,loQueVoyALoguear2;

		if(idGeneradoEnElMomento == 1){ //caso feliz
		send(socket_suscriptor,&cantidadDeLocalizedPokemon,sizeof(int),0);
		for(int i=0;i<cantidadDeLocalizedPokemon;i++){
			mensaje = list_get(Localized_Pokemon,i);
			pthread_mutex_lock(&mutexMemoriaInterna);
			unLocalizedPokemon = sacarDeMemoriaElLocalizedPokemon(mensaje);
			pthread_mutex_unlock(&mutexMemoriaInterna);

			loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
			log_info(logger, loQueVoyALoguear,mensaje->ID,unSuscriptor->socketSuscriptor);

			printf("\n\nNombre: %s",unLocalizedPokemon->nombre);
			pthread_mutex_lock(&mutexColaLocalizedPokemon);
			enviarLocalizedPokemon(unLocalizedPokemon,socket_suscriptor,mensaje->ID,0);
			printf("\nMensaje enviado: %s",unLocalizedPokemon->nombre);
			list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
			printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));

			loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
			printf("\nACK recibido: %d",ack);

			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("\nSe cayó la conexion...");
				pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
			}
			list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
			loQueVoyALoguear2 = "ACK recibido,el suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
			log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
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

					pthread_mutex_lock(&mutexColaLocalizedPokemon);
					pthread_mutex_lock(&mutexMemoriaInterna);
					unLocalizedPokemon = sacarDeMemoriaElLocalizedPokemon(mensaje);
					pthread_mutex_unlock(&mutexMemoriaInterna);

					loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
					log_info(logger, loQueVoyALoguear,mensaje->ID,unSuscriptor->socketSuscriptor);

					printf("\n\nNombre: %s",unLocalizedPokemon->nombre);
					enviarLocalizedPokemon(unLocalizedPokemon,socket_suscriptor,mensaje->ID,0);
					printf("Mensaje enviado: %s\n",unLocalizedPokemon->nombre);
					list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
					printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d\n",list_size(mensaje->suscriptoresAtendidos));

					loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
					printf("ACK WHATTT recibido: %d\n",ack);

					if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
						puts("Se cayó la conexion...");
						pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
					}
					list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
					loQueVoyALoguear2 = "ACK recibido,el suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
					log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
					pthread_mutex_unlock(&mutexColaLocalizedPokemon);
				}
				mensajeYaEnviado = 0;
			}
		} //fin del caso triste
		free(unLocalizedPokemon);
		puts("\nSuscriptor ok");
}

void enviarColaGetPokemon(int idGeneradoEnElMomento,int socket_suscriptor,Suscriptor* unSuscriptor){

	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
		MensajeGetPokemon2* mensaje;
		GetPokemon* unGetPokemon;
		int tamanioListaSuscriptoresACK;
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int cantidadDeGetPokemon = list_size(Get_Pokemon);
		char* loQueVoyALoguear,loQueVoyALoguear2;

		if(idGeneradoEnElMomento == 1){ //caso feliz
		send(socket_suscriptor,&cantidadDeGetPokemon,sizeof(int),0);
		for(int i=0;i<cantidadDeGetPokemon;i++){
			mensaje = list_get(Get_Pokemon,i);
			pthread_mutex_lock(&mutexMemoriaInterna);
			unGetPokemon = sacarDeMemoriaElGetPokemon(mensaje);
			pthread_mutex_unlock(&mutexMemoriaInterna);

			loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
			log_info(logger, loQueVoyALoguear,mensaje->ID,unSuscriptor->socketSuscriptor);

			printf("\n\nNombre: %s",unGetPokemon->nombre);
			pthread_mutex_lock(&mutexColaGetPokemon);
			enviarGetPokemon(unGetPokemon,socket_suscriptor,mensaje->ID);
			printf("\nMensaje enviado: %s",unGetPokemon->nombre);
			list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
			printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));

			loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
			printf("\nACK recibido: %d",ack);

			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("\nSe cayó la conexion...");
				pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
			}
			list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
			loQueVoyALoguear2 = "ACK recibido,el suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
			log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
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

					pthread_mutex_lock(&mutexColaGetPokemon);
					pthread_mutex_lock(&mutexMemoriaInterna);
					unGetPokemon = sacarDeMemoriaElGetPokemon(mensaje);
					pthread_mutex_unlock(&mutexMemoriaInterna);

					loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
					log_info(logger, loQueVoyALoguear,mensaje->ID,unSuscriptor->socketSuscriptor);

					printf("\n\nNombre: %s",unGetPokemon->nombre);
					enviarGetPokemon(unGetPokemon,socket_suscriptor,mensaje->ID);
					printf("Mensaje enviado: %s\n",unGetPokemon->nombre);
					list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
					printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d\n",list_size(mensaje->suscriptoresAtendidos));

					loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
					printf("ACK WHATTT recibido: %d\n",ack);

					if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
						puts("Se cayó la conexion...");
						pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
					}
					list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
					loQueVoyALoguear2 = "ACK recibido,el suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
					log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
					pthread_mutex_unlock(&mutexColaGetPokemon);
				}
				mensajeYaEnviado = 0;
			}
		} //fin del caso triste
		free(unGetPokemon);
		puts("\nSuscriptor ok");
}


void enviarColaAppearedPokemon(int idGeneradoEnElMomento,int socket_suscriptor,Suscriptor* unSuscriptor){

	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
		MensajeAppearedPokemon2* mensaje;
		AppearedPokemon* unAppearedPokemon;
		int tamanioListaSuscriptoresACK;
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int cantidadDeAppearedPokemon = list_size(Appeared_Pokemon);
		char* loQueVoyALoguear,loQueVoyALoguear2;

		if(idGeneradoEnElMomento == 1){ //caso feliz
		send(socket_suscriptor,&cantidadDeAppearedPokemon,sizeof(int),0);
		for(int i=0;i<cantidadDeAppearedPokemon;i++){
			mensaje = list_get(Appeared_Pokemon,i);
			pthread_mutex_lock(&mutexMemoriaInterna);
			unAppearedPokemon = sacarDeMemoriaElAppearedPokemon(mensaje);
			pthread_mutex_unlock(&mutexMemoriaInterna);

			loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
			log_info(logger, loQueVoyALoguear,mensaje->ID,unSuscriptor->socketSuscriptor);

			printf("\n\nNombre: %s",unAppearedPokemon->nombre);
			pthread_mutex_lock(&mutexColaAppearedPokemon);
			enviarAppearedPokemon(unAppearedPokemon,socket_suscriptor,mensaje->ID,0);
			printf("\nMensaje enviado: %s",unAppearedPokemon->nombre);
			list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
			printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));

			loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
			printf("\nACK recibido: %d",ack);

			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("\nSe cayó la conexion...");
				pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
			}
			list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
			loQueVoyALoguear2 = "ACK recibido,el suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
			log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
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

					pthread_mutex_lock(&mutexColaAppearedPokemon);
					pthread_mutex_lock(&mutexMemoriaInterna);
					unAppearedPokemon = sacarDeMemoriaElAppearedPokemon(mensaje);
					pthread_mutex_unlock(&mutexMemoriaInterna);

					loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
					log_info(logger, loQueVoyALoguear,mensaje->ID,unSuscriptor->socketSuscriptor);

					printf("\n\nNombre: %s",unAppearedPokemon->nombre);
					enviarAppearedPokemon(unAppearedPokemon,socket_suscriptor,mensaje->ID,0);

					printf("Mensaje enviado: %s\n",unAppearedPokemon->nombre);
					list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
					printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d\n",list_size(mensaje->suscriptoresAtendidos));

					loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
					printf("ACK WHATTT recibido: %d\n",ack);

					if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
						puts("Se cayó la conexion...");
						pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
					}
					list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
					loQueVoyALoguear2 = "ACK recibido,el suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
					log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
					pthread_mutex_unlock(&mutexColaAppearedPokemon);
				}
				mensajeYaEnviado = 0;
			}
		} //fin del caso triste
		free(unAppearedPokemon);
		puts("\nSuscriptor ok");
}

void enviarColaCatchPokemon(int idGeneradoEnElMomento,int socket_suscriptor,Suscriptor* unSuscriptor){
	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
		MensajeCatchPokemon2* mensaje;
		CatchPokemon* unCatchPokemon;
		int tamanioListaSuscriptoresACK;
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int cantidadDeCatchPokemon = list_size(Catch_Pokemon);
		char* loQueVoyALoguear,loQueVoyALoguear2;

		if(idGeneradoEnElMomento == 1){ //caso feliz
		send(socket_suscriptor,&cantidadDeCatchPokemon,sizeof(int),0);
		for(int i=0;i<cantidadDeCatchPokemon;i++){
			mensaje = list_get(Catch_Pokemon,i);
			pthread_mutex_lock(&mutexMemoriaInterna);
			unCatchPokemon = sacarDeMemoriaElCatchPokemon(mensaje);
			pthread_mutex_unlock(&mutexMemoriaInterna);
			loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
			log_info(logger, loQueVoyALoguear,mensaje->ID,unSuscriptor->socketSuscriptor);

			printf("\n\nNombre: %s",unCatchPokemon->nombre);
			pthread_mutex_lock(&mutexColaCatchPokemon);
			enviarCatchPokemon(unCatchPokemon,socket_suscriptor,mensaje->ID);
			printf("\nMensaje enviado: %s",unCatchPokemon->nombre);
			list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
			printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));

			loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
			printf("\nACK recibido: %d",ack);

			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("\nSe cayó la conexion...");
				pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
			}
			list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
			loQueVoyALoguear2 = "ACK recibido,el suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
			log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
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

					pthread_mutex_lock(&mutexColaCatchPokemon);
					pthread_mutex_lock(&mutexMemoriaInterna);
					unCatchPokemon = sacarDeMemoriaElCatchPokemon(mensaje);
					pthread_mutex_unlock(&mutexMemoriaInterna);

					loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
					log_info(logger, loQueVoyALoguear,mensaje->ID,unSuscriptor->socketSuscriptor);

					printf("\n\nNombre: %s",unCatchPokemon->nombre);
					enviarCatchPokemon(unCatchPokemon,socket_suscriptor,mensaje->ID);
					printf("Mensaje enviado: %s\n",unCatchPokemon->nombre);
					list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
					printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d\n",list_size(mensaje->suscriptoresAtendidos));

					loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
					printf("ACK WHATTT recibido: %d\n",ack);

					if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
						puts("Se cayó la conexion...");
						pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
					}
					list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
					loQueVoyALoguear2 = "ACK recibido,el suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
					log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
					pthread_mutex_unlock(&mutexColaCatchPokemon);
				}
				mensajeYaEnviado = 0;
			}
		} //fin del caso triste
		free(unCatchPokemon);
		puts("\nSuscriptor ok");
}


void enviarColaCaughtPokemon(int idGeneradoEnElMomento,int socket_suscriptor,Suscriptor* unSuscriptor){
	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
		MensajeCaughtPokemon2* mensaje;
		CaughtPokemon* unCaughtPokemon;
		int tamanioListaSuscriptoresACK;
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int cantidadDeCaughtPokemon = list_size(Caught_Pokemon);
		char* loQueVoyALoguear,loQueVoyALoguear2;

		if(idGeneradoEnElMomento == 1){ //caso feliz
		send(socket_suscriptor,&cantidadDeCaughtPokemon,sizeof(int),0);
		for(int i=0;i<cantidadDeCaughtPokemon;i++){
			mensaje = list_get(Caught_Pokemon,i);
			pthread_mutex_lock(&mutexMemoriaInterna);
			unCaughtPokemon = sacarDeMemoriaElCaughtPokemon(mensaje);
			pthread_mutex_unlock(&mutexMemoriaInterna);

			loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
			log_info(logger, loQueVoyALoguear,mensaje->ID,unSuscriptor->socketSuscriptor);

			printf("\n\nAtrapado?: %d",unCaughtPokemon->atrapar);
			pthread_mutex_lock(&mutexColaCaughtPokemon);
			enviarCaughtPokemon(unCaughtPokemon,socket_suscriptor,mensaje->ID,0);
			printf("\nMensaje enviado: %d",unCaughtPokemon->atrapar);
			list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
			printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));

			loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
			printf("\nACK recibido: %d",ack);

			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("\nSe cayó la conexion...");
				pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
			}
				list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				loQueVoyALoguear2 = "ACK recibido,el suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
				log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
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

				pthread_mutex_lock(&mutexColaCaughtPokemon);
				pthread_mutex_lock(&mutexMemoriaInterna);
				unCaughtPokemon = sacarDeMemoriaElCaughtPokemon(mensaje);
				pthread_mutex_unlock(&mutexMemoriaInterna);

				loQueVoyALoguear = "Voy a enviar el mensaje: %d al suscriptor: %d";
				log_info(logger, loQueVoyALoguear,mensaje->ID,unSuscriptor->socketSuscriptor);

				printf("\n\nAtrapado?: %d",unCaughtPokemon->atrapar);
				enviarCaughtPokemon(unCaughtPokemon,socket_suscriptor,mensaje->ID,0);
				printf("Mensaje enviado: %d\n",unCaughtPokemon->atrapar);
				list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
				printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d\n",list_size(mensaje->suscriptoresAtendidos));

				loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
				printf("ACK recibido: %d\n",ack);

				if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
					puts("Se cayó la conexion...");
					pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
				}
				list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
				loQueVoyALoguear2 = "ACK recibido,el suscriptor con ID=%d fue agregado a la lista de SuscriptoresACK";
				log_info(logger, loQueVoyALoguear2,unSuscriptor->IDsuscriptor);
				pthread_mutex_unlock(&mutexColaCaughtPokemon);
			}
				mensajeYaEnviado = 0;
			}
		} //fin del caso triste
			free(unCaughtPokemon);
			puts("\nSuscriptor ok");
}


//----------------------------------------------------PARTICIONES DINÁMICAS-----------------



