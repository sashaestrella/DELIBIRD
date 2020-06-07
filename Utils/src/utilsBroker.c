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

void testACK(int socket){
	int ack = 27;
	int estado = 8;

	estado = recv(socket,&ack,sizeof(int),MSG_WAITALL);
	printf("recibi: %d",ack);
	printf("estado: %d",estado);
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
					//testACK(cliente_fd);
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
				/*case ACK:
					recibirACK(cliente_fd);
					break;*/
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
				printf("El suscriptor con id=%d fue agregado a la lista de SuscriptoresACK",unSuscriptor->IDsuscriptor);
			}

		}
}

void enviarCatchPokemonASuscriptores(MensajeCatchPokemon* unMensajeCatchPokemon){
		Suscriptor* unSuscriptor;
		int tamanioSuscriptoresCatchPokemon = list_size(suscriptores_catch_pokemon);
		for(int i=0;i<tamanioSuscriptoresCatchPokemon;i++){
			unSuscriptor = list_get(suscriptores_catch_pokemon,i);
			enviarCatchPokemon(unMensajeCatchPokemon->contenidoDelMensaje,unSuscriptor->socketSuscriptor,unMensajeCatchPokemon->ID);
			list_add(unMensajeCatchPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
		}
}

void enviarLocalizedPokemonASuscriptores(MensajeLocalizedPokemon* unMensajeLocalizedPokemon){
		Suscriptor* unSuscriptor;
		int tamanioSuscriptoresLocalizedPokemon = list_size(suscriptores_localized_pokemon);
		for(int i=0;i<tamanioSuscriptoresLocalizedPokemon;i++){
			unSuscriptor = list_get(suscriptores_localized_pokemon,i);
			enviarLocalizedPokemon(unMensajeLocalizedPokemon->contenidoDelMensaje,unSuscriptor->socketSuscriptor,unMensajeLocalizedPokemon->ID,unMensajeLocalizedPokemon->IDCorrelativo);
			list_add(unMensajeLocalizedPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
		}
}


void enviarGetPokemonASuscriptores(MensajeGetPokemon* unMensajeGetPokemon){
		Suscriptor* unSuscriptor;
		int tamanioSuscriptoresGetPokemon = list_size(suscriptores_get_pokemon);

		for(int i=0;i<tamanioSuscriptoresGetPokemon;i++){
			unSuscriptor = list_get(suscriptores_get_pokemon,i);
			enviarGetPokemon(unMensajeGetPokemon->contenidoDelMensaje,unSuscriptor->socketSuscriptor,unMensajeGetPokemon->ID);
			list_add(unMensajeGetPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
		}
}

void enviarAppearedPokemonASuscriptores(MensajeAppearedPokemon* unMensajeAppearedPokemon){
		Suscriptor* unSuscriptor;
		int tamanioSuscriptoresAppearedPokemon = list_size(suscriptores_appeared_pokemon);

		for(int i=0;i<tamanioSuscriptoresAppearedPokemon;i++){
			unSuscriptor = list_get(suscriptores_appeared_pokemon,i);
			enviarAppearedPokemon(unMensajeAppearedPokemon->contenidoDelMensaje,unSuscriptor->socketSuscriptor,unMensajeAppearedPokemon->ID,0);
			list_add(unMensajeAppearedPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
		}
}

void enviarNewPokemonASuscriptores(MensajeNewPokemon* unMensajeNewPokemon){
		Suscriptor* unSuscriptor;
		int tamanioSuscriptoresNewPokemon = list_size(suscriptores_new_pokemon);

		for(int i=0;i<tamanioSuscriptoresNewPokemon;i++){
			unSuscriptor = list_get(suscriptores_new_pokemon,i);
			enviarNewPokemon(unMensajeNewPokemon->contenidoDelMensaje,unSuscriptor->socketSuscriptor,unMensajeNewPokemon->ID);
			list_add(unMensajeNewPokemon->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
			puts("Suscriptor agregado a la lista de atendidos");
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

		Suscriptor* otroSuscriptor;
		Suscriptor* elSuscriptorQueVoyARetornar;
		int tamanioListaSuscriptores = list_size(suscriptores_new_pokemon);
		int idSuscriptor;

		t_buffer* buffer = malloc(sizeof(t_buffer));
		recv(socket_suscriptor,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(socket_suscriptor,buffer->stream,buffer->size,MSG_WAITALL);
		memcpy(&idSuscriptor,stream,sizeof(int));
		printf("\nRecibi solicitud de suscripcion para NewPokemon de socket: %d\n",socket_suscriptor);
		printf("Su id es: %d\n",idSuscriptor);

		if(idSuscriptor == 0){
			Suscriptor* unSuscriptor = malloc(sizeof(Suscriptor));
			pthread_mutex_lock(&mutexGeneradorIDSuscriptor);
			generadorDeIDsSuscriptor++;
			unSuscriptor->IDsuscriptor = generadorDeIDsSuscriptor;
			pthread_mutex_unlock(&mutexGeneradorIDSuscriptor);
			unSuscriptor->socketSuscriptor = socket_suscriptor;

			pthread_mutex_lock(&mutexListaSuscriptores);
			list_add(suscriptores_new_pokemon,unSuscriptor);
			pthread_mutex_unlock(&mutexListaSuscriptores);

			printf("El tamaño de la lista de suscriptores de NP ahora es de: %d\n",list_size(suscriptores_new_pokemon));
			printf("El suscriptor %d con id = %d fue agregado a la lista new pokemon\n",socket_suscriptor,unSuscriptor->IDsuscriptor);
			send(socket_suscriptor,&(unSuscriptor->IDsuscriptor),sizeof(int),0);

			elSuscriptorQueVoyARetornar = unSuscriptor;
		} else {
			for(int i=0; i<tamanioListaSuscriptores;i++){
				otroSuscriptor = list_get(suscriptores_new_pokemon,i);
					if(otroSuscriptor->IDsuscriptor == idSuscriptor){
						otroSuscriptor->socketSuscriptor = socket_suscriptor;
						elSuscriptorQueVoyARetornar =  otroSuscriptor;
					}
					puts("Sigo buscando...\n");
			}
		}

		free(buffer->stream);
		free(buffer);
		return elSuscriptorQueVoyARetornar;
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

void guardarElACKCaughtPokemon(int idMensaje,int idSuscriptor){
		MensajeCaughtPokemon* unMensajeCaughtPokemon;
		int tamanioDeLista = list_size(Caught_Pokemon);
		int idSuscriptorAGuardar = idSuscriptor;

		for(int i=0;i<tamanioDeLista;i++){
			unMensajeCaughtPokemon = list_get(Caught_Pokemon,i);
			if(unMensajeCaughtPokemon->ID == idMensaje){
				list_add(unMensajeCaughtPokemon->suscriptoresACK,&idSuscriptorAGuardar);
				break;
			}else {
				puts("\nNo es igual");
			}
		}

}

void guardarElACKCatchPokemon(int idMensaje,int idSuscriptor){
		MensajeCatchPokemon* unMensajeCatchPokemon;
		int tamanioDeLista = list_size(Catch_Pokemon);
		int idSuscriptorAGuardar = idSuscriptor;

		for(int i=0;i<tamanioDeLista;i++){
			unMensajeCatchPokemon = list_get(Catch_Pokemon,i);
			if(unMensajeCatchPokemon->ID == idMensaje){
				list_add(unMensajeCatchPokemon->suscriptoresACK,&idSuscriptorAGuardar);
				break;
			}else {
				puts("\nNo es igual");
			}
		}

}

void guardarElACKAppearedPokemon(int idMensaje,int idSuscriptor){
		MensajeAppearedPokemon* unMensajeAppearedPokemon;
		int tamanioDeLista = list_size(Appeared_Pokemon);
		int idSuscriptorAGuardar = idSuscriptor;

		for(int i=0;i<tamanioDeLista;i++){
			unMensajeAppearedPokemon = list_get(Appeared_Pokemon,i);
			if(unMensajeAppearedPokemon->ID == idMensaje){
				list_add(unMensajeAppearedPokemon->suscriptoresACK,&idSuscriptorAGuardar);
				break;
			}else {
				puts("\nNo es igual");
			}
		}

}

void guardarElACKGetPokemon(int idMensaje,int idSuscriptor){
		MensajeGetPokemon* unMensajeGetPokemon;
		int tamanioDeLista = list_size(Get_Pokemon);
		int idSuscriptorAGuardar = idSuscriptor;

		for(int i=0;i<tamanioDeLista;i++){
			unMensajeGetPokemon = list_get(Get_Pokemon,i);
			if(unMensajeGetPokemon->ID == idMensaje){
				list_add(unMensajeGetPokemon->suscriptoresACK,&idSuscriptorAGuardar);
				break;
			}else {
				puts("\nNo es igual");
			}
		}

}

void guardarElACKLocalizedPokemon(int idMensaje,int idSuscriptor){
		MensajeLocalizedPokemon* unMensajeLocalizedPokemon;
		int tamanioDeLista = list_size(Localized_Pokemon);
		int idSuscriptorAGuardar = idSuscriptor;

		for(int i=0;i<tamanioDeLista;i++){
			unMensajeLocalizedPokemon = list_get(Localized_Pokemon,i);
			if(unMensajeLocalizedPokemon->ID == idMensaje){
				list_add(unMensajeLocalizedPokemon->suscriptoresACK,&idSuscriptorAGuardar);
				break;
			}else {
				puts("\nNo es igual");
			}
		}
}

void guardarElACKNewPokemon(int idMensaje,int idSuscriptor){
		MensajeNewPokemon* unMensajeNewPokemon;
		int tamanioDeLista = list_size(New_Pokemon);
		int idSuscriptorAGuardar = idSuscriptor;

		for(int i=0;i<tamanioDeLista;i++){
			unMensajeNewPokemon = list_get(New_Pokemon,i);
			if(unMensajeNewPokemon->ID == idMensaje){
				list_add(unMensajeNewPokemon->suscriptoresACK,&idSuscriptorAGuardar);
				break;
			}else {
				puts("\nNo es igual");
			}
		}
}

void recibirACK(int cliente_fd){
		ACKmensaje* unACK = malloc(sizeof(ACKmensaje));
		t_buffer* buffer = malloc(sizeof(t_buffer));

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		memcpy(&(unACK->numeroDeColaALaQuePertenece),stream,sizeof(int));
		stream+= sizeof(int);
		memcpy(&(unACK->IDmensaje),stream,sizeof(int));
		stream+= sizeof(int);
		memcpy(&(unACK->IDsuscriptor),stream,sizeof(int));
		stream+= sizeof(int);

		switch(unACK->numeroDeColaALaQuePertenece){
				case NEW_POKEMON:
					guardarElACKNewPokemon(unACK->IDmensaje,unACK->IDsuscriptor);
					break;
				case LOCALIZED_POKEMON:
					guardarElACKLocalizedPokemon(unACK->IDmensaje,unACK->IDsuscriptor);
					break;
				case GET_POKEMON:
					guardarElACKGetPokemon(unACK->IDmensaje,unACK->IDsuscriptor);
					break;
				case APPEARED_POKEMON:
					guardarElACKAppearedPokemon(unACK->IDmensaje,unACK->IDsuscriptor);
					break;
				case CATCH_POKEMON:
					guardarElACKCatchPokemon(unACK->IDmensaje,unACK->IDsuscriptor);
					break;
				case CAUGHT_POKEMON:
					guardarElACKCaughtPokemon(unACK->IDmensaje,unACK->IDsuscriptor);
					break;
		}


		free(buffer->stream);
		free(buffer);
		free(unACK);
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


void enviarColaNewPokemon(int socket_suscriptor, Suscriptor* unSuscriptor){
	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
		MensajeNewPokemon* mensaje;
		NewPokemon* unNewPokemon;
		int id,tamanioListaSuscriptoresACK,idMensaje;
		int ack = 0;
		int loQueDevuelveElRecv = 0;
		int cantidadDeNewPokemon = list_size(New_Pokemon);
		int idRecibidoDelSuscriptor = unSuscriptor->IDsuscriptor;

		if(unSuscriptor->IDsuscriptor == 0){ //caso feliz
		send(socket_suscriptor,&cantidadDeNewPokemon,sizeof(int),0);
		for(int i=0;i<cantidadDeNewPokemon;i++){
			mensaje = list_get(New_Pokemon,i);
			printf("\nTe voy a enviar el mensaje: %s\n",mensaje->contenidoDelMensaje->nombre);

			pthread_mutex_lock(&mutexColaNewPokemon);
			unNewPokemon = mensaje->contenidoDelMensaje;
			enviarNewPokemon(unNewPokemon,socket_suscriptor,mensaje->ID);
			printf("Mensaje enviado: %s\n",unNewPokemon->nombre);

			list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
			printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));
			puts("\nSuscriptor ok");
			loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
			printf("ACK recibido: %d\n",ack);
			if(loQueDevuelveElRecv == -1 || loQueDevuelveElRecv == 0){
				puts("Se cayó la conexion...");
				pthread_exit(NULL); //Si no le pongo esto sigue ejecutando y no deberia
			}
			list_add(mensaje->suscriptoresACK,&(unSuscriptor->IDsuscriptor));
			printf("El suscriptor fue agregado a la lista de SuscriptoresACK, ACK del mensaje %d\n",mensaje->ID);

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
					printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));
					puts("\nSuscriptor ok");
					loQueDevuelveElRecv = recv(socket_suscriptor,&ack,sizeof(int),0);
					printf("ACK recibido: %d\n",ack);
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


void enviarColaLocalizedPokemon(int socket_suscriptor,Suscriptor* unSuscriptor){

	puts("Ahora tengo que ver si te puedo mandar los mensajes o no..");
			MensajeLocalizedPokemon* mensaje;
			LocalizedPokemon* unLocalizedPokemon;
			int cantidadDeLocalizedPokemon = list_size(Localized_Pokemon);
			send(socket_suscriptor,&cantidadDeLocalizedPokemon,sizeof(int),0);

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
				enviarLocalizedPokemon(unLocalizedPokemon,socket_suscriptor,mensaje->ID,mensaje->IDCorrelativo);
				printf("Mensaje enviado: %s\n",unLocalizedPokemon->nombre);

				list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
				printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));
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
				printf("Mensaje enviado: %s\n",unGetPokemon->nombre);

				list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
				printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));
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
					enviarAppearedPokemon(unAppearedPokemon,socket_suscriptor,mensaje->ID,0);
					printf("Mensaje enviado: %s\n",unAppearedPokemon->nombre);

					list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
					printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));
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

			pthread_mutex_lock(&mutexColaCatchPokemon);
			while(cantidadDeCatchPokemon==0){
				pthread_cond_wait(&no_vacioCP,&mutexColaCatchPokemon);
				printf("Esperando a que llegue algo");

			}
			pthread_mutex_unlock(&mutexColaCatchPokemon);

			for(int i=0;i<cantidadDeCatchPokemon;i++){
					mensaje = list_get(Catch_Pokemon,i);
					printf("\nTe voy a enviar el mensaje: %s\n",mensaje->contenidoDelMensaje->nombre);

					pthread_mutex_lock(&mutexColaCatchPokemon);
					unCatchPokemon = mensaje->contenidoDelMensaje;
					enviarCatchPokemon(unCatchPokemon,socket_suscriptor,mensaje->ID);
					printf("Mensaje enviado: %s\n",unCatchPokemon->nombre);

					list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
					printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));
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

			pthread_mutex_lock(&mutexColaCaughtPokemon);
			while(cantidadDeCaughtPokemon==0){
				pthread_cond_wait(&no_vacioCAP,&mutexColaCaughtPokemon);
				printf("Esperando a que llegue algo");

			}
			pthread_mutex_unlock(&mutexColaCaughtPokemon);

			for(int i=0;i<cantidadDeCaughtPokemon;i++){
					mensaje = list_get(Caught_Pokemon,i);
					printf("\nTe voy a enviar el mensaje: %d\n",mensaje->contenidoDelMensaje->atrapar);

					pthread_mutex_lock(&mutexColaCaughtPokemon);
					unCaughtPokemon = mensaje->contenidoDelMensaje;
					enviarCaughtPokemon(unCaughtPokemon,socket_suscriptor,mensaje->ID,mensaje->IDCorrelativo);
					printf("Mensaje enviado: %d\n",unCaughtPokemon->atrapar);

					list_add(mensaje->suscriptoresAtendidos,&(unSuscriptor->IDsuscriptor));
					printf("\nAhora el tamaño de la lista de suscriptores atendidos es de: %d",list_size(mensaje->suscriptoresAtendidos));
					puts("\nSuscriptor ok");

					pthread_mutex_unlock(&mutexColaCaughtPokemon);

					}
	}

