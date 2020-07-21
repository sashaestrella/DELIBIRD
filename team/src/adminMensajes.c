#include "adminMensajes.h"

void generarConexiones(){


	// -- Hilo de suscripcion a cola caught -- //

	pthread_t hiloCaught;
	pthread_create(&hiloCaught, NULL, (void*)suscribirseAColaCaught, NULL);


	// -- Hilo de suscripcion a cola appeared -- //

	pthread_t hiloAppeared;
	pthread_create(&hiloAppeared, NULL, (void*)suscribirseAColaAppeared, NULL);


	// -- Hilo de suscripcion a localized -- //

	pthread_t hiloLocalized;
	pthread_create(&hiloLocalized, NULL,(void*) suscribirseAColaLocalized, NULL);


	pthread_detach(hiloAppeared);
	pthread_detach(hiloCaught);
	pthread_detach(hiloLocalized);
	//pthread_join(hiloAppeared,NULL);
	//pthread_join(hiloCaught,NULL);
	//pthread_join(hiloLocalized,NULL);

}

void abrirEscuchaAppeared(){
	pthread_t escuchaAppeared;
	pthread_create(&escuchaAppeared, NULL, (void*)recibirMensajesAppeared, NULL);
	printf("Ya estoy escuchando la cola appeared... \n\n");
	pthread_detach(escuchaAppeared);
}

void abrirEscuchaLocalized(){
	pthread_t escuchaLocalized;
	pthread_create(&escuchaLocalized, NULL, (void*)recibirMensajesLocalized, NULL);
	printf("Ya estoy escuchando la cola localized... \n\n");
	pthread_detach(escuchaLocalized);
}

void abrirEscuchaCaught(){
	pthread_t escuchaCaught;
	pthread_create(&escuchaCaught, NULL, (void*)recibirMensajesCaught, NULL);
	printf("Ya estoy escuchando la cola Caught... \n\n");
	pthread_detach(escuchaCaught);
}

// ------------------------------------ AUXILIARES ------------------------------------ //

void noHayBroker(){
	iniciar_servidor_team();
}

void suscribirseAColaCaught(){

	int IDsuscripcion;
	sem_wait(&suscripciones);
	int conexion = crear_conexion(ip, puerto);
	if(conexion != -1){
	printf( "\nSe creo la suscripcion a cola Caught con el valor %d \n", conexion);

	enviarSuscripcion(IDsuscripcionCaught, conexion, SUSCRIPTOR_CAUGHTPOKEMON);
	recv(conexion, &IDsuscripcion, sizeof(int), MSG_WAITALL);
	sem_post(&suscripciones);
	printf("Suscriptor numero: %d\n", IDsuscripcion);

	char id[2];
	sprintf(id, "%d", IDsuscripcion);
	config_set_value(archivo_config, "ID_CAUGHT", id );
	config_save(archivo_config);
	administradorMensajesColas(SUSCRIPTOR_CAUGHTPOKEMON, conexion, IDsuscripcion);
	}else{
		sem_post(&reintento_caught);
		sem_post(&suscripciones);
		}
}

void suscribirseAColaLocalized(){

	int IDsuscripcion;
	sem_wait(&suscripciones);
	int conexion = crear_conexion(ip, puerto);
	if(conexion != -1){
	printf( "\nSe creo la suscripcion a cola Localized con el valor %d \n", conexion);

	enviarSuscripcion(IDsuscripcionLocalized, conexion, SUSCRIPTOR_LOCALIZEDPOKEMON);
	recv(conexion, &IDsuscripcion, sizeof(int), MSG_WAITALL);
	sem_post(&suscripciones);
	printf("Suscriptor numero: %d\n", IDsuscripcion);

	char id[2];
	sprintf(id, "%d", IDsuscripcion);
	config_set_value(archivo_config, "ID_LOCALIZED", id );
	config_save(archivo_config);
	administradorMensajesColas(SUSCRIPTOR_LOCALIZEDPOKEMON, conexion, IDsuscripcion);
	}else{
		sem_post(&reintento_localized);
		sem_post(&suscripciones);
		}
}

void suscribirseAColaAppeared(){

	int IDsuscripcion;
	sem_wait(&suscripciones);
	int conexion = crear_conexion(ip, puerto);
	if(conexion != -1){
	printf( "\nSe creo la suscripcion a cola Appeared con el valor %d \n", conexion);

	enviarSuscripcion(IDsuscripcionAppeared, conexion, SUSCRIPTOR_APPEAREDPOKEMON);
	recv(conexion, &IDsuscripcion, sizeof(int), MSG_WAITALL);
	sem_post(&suscripciones);
	printf("Suscriptor numero: %d\n", IDsuscripcion);

	char id[2];
	sprintf(id, "%d", IDsuscripcion);
	config_set_value(archivo_config, "ID_APPEARED", id );
	config_save(archivo_config);
	administradorMensajesColas(SUSCRIPTOR_APPEAREDPOKEMON, conexion, IDsuscripcion);
	}else{
	sem_post(&reintento_appeared);
	sem_post(&suscripciones);
	}
}

void administradorMensajesColas(int op_code, int conexion, int IDsuscripcion){

	int cantidadAppearedPokemon;
	int cantidadLocalizedPokemon;
	int cantidadCaughtPokemon;
	uint32_t codigo;
	uint32_t codigo1;
	uint32_t codigo2;
	switch(op_code){

			case SUSCRIPTOR_APPEAREDPOKEMON:
				conexionAppeared = conexion;
				IDsuscripcionAppeared= IDsuscripcion;
				recv(conexion, &cantidadAppearedPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Appeared Pokemons: %d\n", cantidadAppearedPokemon);
				AppearedPokemonConIDs* nuevoAppearedPokemonConId;
				for(int i = 0; i<cantidadAppearedPokemon; i++){
					recv(conexion, &codigo, sizeof(op_code), MSG_WAITALL);
					printf("Codigo de cola: %d\n", codigo);
					nuevoAppearedPokemonConId = recibir_APPEARED_POKEMON(conexion, 0,1,0);
					int ack = 1;
					send(conexion, &ack, sizeof(int), 0); // No se si pasar el 1 con un void*
					adminMensajeAppeared(nuevoAppearedPokemonConId);
					printf("Recibi mensaje com id: %d\n", nuevoAppearedPokemonConId->IDmensaje);
				}
				abrirEscuchaAppeared();
				break;

			case SUSCRIPTOR_LOCALIZEDPOKEMON:
				conexionLocalized = conexion;
				IDsuscripcionLocalized= IDsuscripcion;

				recv(conexion, &cantidadLocalizedPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Localized Pokemons: %d\n", cantidadLocalizedPokemon);
				LocalizedPokemonConIDs* nuevoLocalizedPokemonConId;
				for(int i = 0; i<cantidadLocalizedPokemon; i++){
					recv(conexion, &codigo1, sizeof(op_code), MSG_WAITALL);
					printf("Codigo de cola: %d\n", codigo1);
					nuevoLocalizedPokemonConId = recibir_LOCALIZED_POKEMON(conexion, 1, 1);
					int ack = 1;
					send(conexion, &ack, sizeof(int), 0);
					adminMensajeLocalized(nuevoLocalizedPokemonConId);
				}
				abrirEscuchaLocalized();
				break;

			case SUSCRIPTOR_CAUGHTPOKEMON:
				conexionCaught = conexion;
				IDsuscripcionCaught= IDsuscripcion;

				recv(conexion, &cantidadCaughtPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Caught Pokemons: %d\n", cantidadCaughtPokemon);

				CaughtPokemonConIDs* nuevoCaughtPokemonConId;
				for(int i = 0; i<cantidadCaughtPokemon; i++){
					recv(conexion, &codigo2, sizeof(op_code), MSG_WAITALL);
					printf("Codigo de cola: %d\n", codigo2);
					nuevoCaughtPokemonConId = recibir_CAUGHT_POKEMON(conexion, 1, 1);
					send(conexion, 1, sizeof(int), 0);
					adminMensajeCaught(nuevoCaughtPokemonConId);
				}
				abrirEscuchaCaught();
				break;
			default:
				printf("Llego un mensaje invalido");
				break;
	}
}
// ------------------------------- Funciones Team ------------------------------------ //

void recibirMensajesAppeared(){
	pthread_t admin;
	AppearedPokemonConIDs* nuevoAppeared;

	while(1){
		int cod_op;
		int error = recv(conexionAppeared, &cod_op, sizeof(int), 0);
		if(error == 0){
			close(conexionAppeared);
			sem_post(&reintento_appeared);
			pthread_exit(NULL);
		}

		nuevoAppeared = recibir_APPEARED_POKEMON(conexionAppeared, 0, 1, 0); // aca no iria id correlativo
		int ack = 1;
		send(conexionAppeared, &ack, sizeof(int), 0);
		pthread_create(&admin, NULL, (void*)adminMensajeAppeared, nuevoAppeared);
		pthread_detach(admin);

	}

	//free(nuevoAppeared);
}

void adminMensajeAppeared(AppearedPokemonConIDs* nuevoAppeared){

	if(descartar_appeared_no_deseados(nuevoAppeared)){

		Pokemon* nuevo = malloc(sizeof(Pokemon));
		nuevo -> nombre = nuevoAppeared -> appearedPokemon -> nombre;
		nuevo -> posicion.posicionX = nuevoAppeared -> appearedPokemon -> coordenadas.posicionX;
		nuevo -> posicion.posicionY = nuevoAppeared -> appearedPokemon -> coordenadas.posicionY;
		nuevo->IdEntrenadorQueLoVaAatrapar = 0;
		//list_add(nuevosPokemon, nuevo);
		list_add(pokemones_en_mapa, nuevo);
		//sem_post(&aparicion_pokemon);
		sem_post(&agregar_ready);
		log_info(logger, "Llego un mensaje APPEARED con el ID de mensaje: %d. Contenido %s %d %d\n",nuevoAppeared->IDmensaje, nuevo->nombre, nuevo->posicion.posicionX, nuevo->posicion.posicionY);

		puts("\n");


	} else {
		printf("Mensaje Appeared que no es para nosotros\n");
	}
	list_add(mensajesRecibidos, (void*)nuevoAppeared->IDmensaje);

}



void recibirMensajesLocalized(){
	pthread_t admin;
	LocalizedPokemonConIDs* nuevoLocalized;
	while(1){
		int cod_op;
		int error = recv(conexionLocalized, &cod_op, sizeof(int), 0);

		if(error == 0){
			close(conexionLocalized);
			sem_post(&reintento_localized);
			pthread_exit(NULL);
		}

		nuevoLocalized = recibir_LOCALIZED_POKEMON(conexionLocalized, 0, 1);
		int ack = 1;
		send(conexionLocalized, &ack, sizeof(int), 0);
		pthread_create(&admin, NULL, (void*)adminMensajeLocalized, nuevoLocalized);
		pthread_detach(admin);
	}

	//free(nuevoLocalized);
}

void adminMensajeLocalized(LocalizedPokemonConIDs* nuevoLocalized){
	int cantidadObjetivos = 0;
	cantidadObjetivos = obtenerCantidadObjetivo(nuevoLocalized -> localizedPokemon -> nombre);//necesito n pokemon de estos
	int j = 0;
	int cantidadPokemonLocalizados = list_size(nuevoLocalized -> localizedPokemon -> paresOrdenados);// se encontraron n
	Pokemon* nuevo = malloc(sizeof(Pokemon));
	CoordenadasXY* coor=malloc(sizeof(CoordenadasXY));
	//printf("\nLLego localized con ID %d", nuevoLocalized->IDcorrelativo);

	if(descartar_localized_no_deseados(nuevoLocalized)){
		log_info(logger, "Llego un mensaje LOCALIZED con el ID de mensaje %d e ID correlativo. \n",nuevoLocalized->IDmensaje, nuevoLocalized->IDcorrelativo);

		for(int i=0; i<cantidadObjetivos && j < cantidadPokemonLocalizados; i++){
		nuevo = elegirMejorUbicacion(nuevoLocalized);
		nuevo->IdEntrenadorQueLoVaAatrapar = 0;

		list_add(pokemones_en_mapa, nuevo);
		list_add(especies_localizadas,nuevo->nombre);
		printf("Guarde un Pokemon %s de localized con el ID de mensaje %d\n", nuevo->nombre, nuevoLocalized->IDmensaje);
		log_info(logger, "Contenido LOCALIZED %s %d %d ", nuevo->nombre, nuevo->posicion.posicionX, nuevo->posicion.posicionY);
		bool compararCoordenadas(CoordenadasXY* coordenadas){
			return (nuevo -> posicion.posicionX == coordenadas->posicionX) && (nuevo -> posicion.posicionY == coordenadas->posicionY);
		}
		list_remove_by_condition(nuevoLocalized -> localizedPokemon -> paresOrdenados, (void*)compararCoordenadas);
		//lo saca para que no lo vuelva a comparar por esas coordenadas

		//sem_post(&nuevosPokemons);
		sem_post(&pruebaLocalized);

		j++;
		}


		for(int i=0; i<list_size(nuevoLocalized->localizedPokemon->paresOrdenados);i++){
		Pokemon* poke = malloc(sizeof(Pokemon));
		poke->nombre = nuevoLocalized->localizedPokemon->nombre;
		printf("\nCantidad de pares %d", list_size(nuevoLocalized->localizedPokemon->paresOrdenados));
		coor = list_get(nuevoLocalized->localizedPokemon->paresOrdenados,i);
		poke->posicion.posicionX = coor->posicionX;

		poke->posicion.posicionY = coor->posicionY;

		list_add(mapa_auxiliar, poke);
		}

	} else {
		printf("Mensaje Localized que no es para nosotros\n");
	}
	list_add(mensajesRecibidos, nuevoLocalized->IDmensaje); //por si se cae conexion
	//free(nuevo);
}




void recibirMensajesCaught(){
	pthread_t admin;
	CaughtPokemonConIDs* nuevoCaught;

	while(1){
		int cod_op;
		int error = recv(conexionCaught, &cod_op, sizeof(int), 0);

		if(error == 0){
			close(conexionCaught);
			sem_post(&reintento_caught);
			pthread_exit(NULL);
		}

		nuevoCaught = recibir_CAUGHT_POKEMON(conexionCaught, 0, 1);
		int ack = 1;
		send(conexionCaught, &ack, sizeof(int), 0);
		pthread_create(&admin, NULL, (void*)adminMensajeCaught, nuevoCaught);
		pthread_detach(admin);

	}
	//free(nuevoCaught);
}

void adminMensajeCaught(CaughtPokemonConIDs* nuevoCaught){
	if(descartar_caught_no_deseados(nuevoCaught)){

		list_add(nuevosCaught, nuevoCaught);
		printf("Guarde un mensaje Caught");
		log_info(logger, "Llego un mensaje CAUGHT con el ID de mensaje %d, ID correlativo %d y resultado: %d. \n",nuevoCaught->IDmensaje,nuevoCaught->IDCorrelativo,nuevoCaught->caughtPokemon->atrapar);
		Entrenador* entrenadorParaAvisar = malloc(sizeof(Entrenador));

		bool esperaRespuesta(Entrenador* entrenador){
			return entrenador->idMensaje == nuevoCaught->IDCorrelativo;
		}

		entrenadorParaAvisar = list_find(entrenadores, esperaRespuesta);

		printf("\nLe avise a %d que llego su mensaje", entrenadorParaAvisar->ID);

		sem_post(confirmacion_caught[entrenadorParaAvisar->ID-1]);


	} else {
		printf("Mensaje Caught que no es para nosotros\n");
	}
	list_add(mensajesRecibidos, nuevoCaught->IDmensaje);
}



void enviar_getPokemon(GetPokemon* get_pokemon){

	int id_mensaje;
	GetPokemonConIDs* getPokemonConId = malloc(sizeof(GetPokemonConIDs));

	int conexion = crear_conexion(ip, puerto);
	if(conexion != -1){
	printf("\nConexionnn %d", conexion);
	enviarGetPokemon(get_pokemon, conexion ,0);

	recv(conexion, &id_mensaje, sizeof(int), MSG_WAITALL);
	printf("El id del mi mensaje es %d", id_mensaje);
	getPokemonConId->IDmensaje = id_mensaje;

	list_add(mensajesGetEnviados, getPokemonConId);

	//free(getPokemonConId);
	close(conexion);
	}
}

CatchPokemonConIDs* enviar_catchPokemon(CatchPokemon* catch_pokemon){

	int id_mensaje;
	CatchPokemonConIDs* catchPokemonConId = malloc(sizeof(CatchPokemonConIDs));

	int conexion = crear_conexion(ip, puerto);
	if(conexion == -1){
		//puts("\nDevuelvo NULL");
		return NULL;
	}
	enviarCatchPokemon(catch_pokemon, conexion ,0);

	recv(conexion, &id_mensaje, sizeof(int), MSG_WAITALL);

	catchPokemonConId->IDmensaje = id_mensaje;
	catchPokemonConId->catchPokemon = catch_pokemon;
	list_add(mensajesCatchEnviados, catchPokemonConId);
	puts("\nGuarde un enviado");

	return catchPokemonConId;
	//free(catchPokemonConId);
	close(conexion);
	//cerrar conexion
}




bool mePasoDeLosQueNecesito(char* nombre){

	bool contar(char* pokemon){
		return !strcmp(pokemon, nombre);
	}

	int cantidadDeUnaEspecieQueNecesito = list_count_satisfying(objetivoGlobal, contar);

	printf("\nTamaño %d", list_count_satisfying(especies_localizadas, contar));

	if(list_count_satisfying(especies_localizadas, contar) > cantidadDeUnaEspecieQueNecesito){
		list_add(especies_localizadas, nombre);
		return true;
	}else{
		return false;

	}

	return list_count_satisfying(especies_localizadas, contar) > cantidadDeUnaEspecieQueNecesito;
}

bool descartar_appeared_no_deseados(AppearedPokemonConIDs* appearedPokemonRecibido){
	bool compararNombre(char* nombreObjetivo){
		return !strcmp(nombreObjetivo, appearedPokemonRecibido -> appearedPokemon -> nombre);
	}

	if(list_any_satisfy(objetivoGlobal, compararNombre)){

		if(mePasoDeLosQueNecesito( appearedPokemonRecibido -> appearedPokemon -> nombre)){
			Pokemon* pokemon = malloc(sizeof(Pokemon));
			pokemon->nombre = appearedPokemonRecibido -> appearedPokemon -> nombre;
			pokemon->posicion.posicionX = appearedPokemonRecibido->appearedPokemon->coordenadas.posicionX;
			pokemon->posicion.posicionY = appearedPokemonRecibido->appearedPokemon->coordenadas.posicionY;
			pokemon->IdEntrenadorQueLoVaAatrapar = 0;
			list_add(mapa_auxiliar, pokemon);
		}

			return !mePasoDeLosQueNecesito( appearedPokemonRecibido -> appearedPokemon -> nombre);

	}else{
		return false;
	}
}


bool yaRecibiEsaEspecie(char* especie){

	bool compararNombre(char* localizada){
			return !strcmp(localizada, especie);
		}


		return !list_any_satisfy(especies_localizadas, compararNombre) && list_any_satisfy(objetivoGlobal, compararNombre) ;

}



bool descartar_localized_no_deseados(LocalizedPokemonConIDs* localizedPokemonRecibido){

	bool compararIDcorrelativo (GetPokemonConIDs* mensajeGet){
		return mensajeGet->IDmensaje == localizedPokemonRecibido->IDcorrelativo;
	}

	bool yaRecibi(int idMensaje){
			return idMensaje != localizedPokemonRecibido -> IDmensaje;
	}

	if( list_any_satisfy(mensajesGetEnviados, (void*)compararIDcorrelativo)){// falta comparar los que ya recibi
		//puts("\nDescarte por repeticion");
		return yaRecibiEsaEspecie(localizedPokemonRecibido->localizedPokemon->nombre);

	}else{
		//puts("\nDescarte por id");
		return false;
	}
}


bool descartar_caught_no_deseados(CaughtPokemonConIDs* caughtPokemonRecibido){

	bool compararIDcorrelativo (CatchPokemonConIDs* mensajeCatch){

	//	printf("\nID del que esta %d, correlativo del que llega %d",  mensajeCatch->IDmensaje, caughtPokemonRecibido->IDCorrelativo );
		return mensajeCatch->IDmensaje == caughtPokemonRecibido->IDCorrelativo;
	}

	//printf("\nTAmaño catch enviados %d", list_size(mensajesCatchEnviados));



	return list_any_satisfy(mensajesCatchEnviados, (void*)compararIDcorrelativo);

}


//-----------Servidor para gameBoy--------


void iniciar_servidor_team(void){
	int socket_servidor;

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP_TEAM, PUERTO_TEAM, &hints, &servinfo);

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
    	esperar_cliente_team(socket_servidor);
}

void esperar_cliente_team(int socket_servidor)
{
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&hiloServidorTeam,NULL,(void*)serve_client_team,&socket_cliente);
	pthread_detach(hiloServidorTeam);

}

void serve_client_team(int* socket)
{
	int cod_op;

	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
		cod_op = -1;
	process_request_team(cod_op, *socket);
}

void process_request_team(int cod_op, int cliente){

	pthread_t hiloLocalized;
	pthread_t hiloAppeared;
	pthread_t hiloCaught;

	AppearedPokemonConIDs* appeared;
	Pokemon* pokemon = malloc(sizeof(Pokemon));

	switch(cod_op){

	case APPEARED_POKEMON:


		appeared = recibir_APPEARED_POKEMON(cliente,0,0,0);

		pokemon->nombre = appeared->appearedPokemon->nombre;
		pokemon->posicion.posicionX = appeared->appearedPokemon->coordenadas.posicionX;
		pokemon->posicion.posicionY = appeared->appearedPokemon->coordenadas.posicionY;
		pokemon->IdEntrenadorQueLoVaAatrapar = 0;
		list_add(pokemones_en_mapa, pokemon);
		//sem_post(&agregar_ready);
		sem_post(&aparicion_pokemon);
		log_info(logger, "Llego un mensaje APPEARED. Contenido %s %d %d\n",	 pokemon->nombre, pokemon->posicion.posicionX, pokemon->posicion.posicionY);

		break;

	default:
		puts("\nError en codigo de operacion");
		break;

	}
}
