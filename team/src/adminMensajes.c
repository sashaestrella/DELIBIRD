#include "adminMensajes.h"

void generarConexiones(int tipoSuscriptor){

	suscribirseAColaAppeared();

	suscribirseAColaCaught();

	suscribirseAColaLocalized();

}

void abrirEscuchas(){
	pthread_t escuchaAppeared;
	pthread_create(&escuchaAppeared, NULL, recibirMensajesAppeared, NULL);
	printf("Ya estoy escuchando la cola appeared... \n\n");
	pthread_detach(escuchaAppeared);

	pthread_t escuchaLocalized;
	pthread_create(&escuchaLocalized, NULL, recibirMensajesLocalized, NULL);
	printf("Ya estoy escuchando la cola localized... \n\n");
	pthread_detach(escuchaLocalized);

	pthread_t escuchaCaught;
	pthread_create(&escuchaCaught, NULL, recibirMensajesCaught, NULL);
	printf("Ya estoy escuchando la cola Caught... \n\n");
	pthread_detach(escuchaCaught);
}

// ------------------------------------ AUXILIARES ------------------------------------ //

void noHayBroker(){
	iniciar_servidor_team();
}

void* suscribirseAColaCaught(){

	printf("\nCola Caught:\n");
	int conexion = crear_conexion(ip, puerto);
	Suscriptor* unSuscriptor1 = malloc(sizeof(Suscriptor));
	unSuscriptor1->IDsuscriptor = 0;

	enviarSuscripcion(unSuscriptor1->IDsuscriptor,conexion,SUSCRIPTOR_CAUGHTPOKEMON);
	printf("Envie suscripcion para la cola de mensajes %d",SUSCRIPTOR_CAUGHTPOKEMON);
	recv(conexion,&IDsuscripcionCaught,sizeof(int),MSG_WAITALL);
	printf("\nRecibi mi id como suscriptor: %d\n",IDsuscripcionCaught);

	int tamanioLista;

	recv(conexion,&tamanioLista,sizeof(int),MSG_WAITALL);
	if(tamanioLista == 0){
		puts("\nNo puedo recibir la lista porque esta vacia");
		printf("Tamaño lista: %d",tamanioLista);
	}
	printf("El tamaño de la lista que voy a recibir es: %d\n",tamanioLista);

	int size;
	int variableQueNoUsoxd;
	int ack = 1;

	CaughtPokemonConIDs* CaughtConIDs;
	//CaughtPokemon* unCaughtPokemonTemporal;

	for(int i = 0; i<tamanioLista;i++){
		recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
		CaughtConIDs = recibir_CAUGHT_POKEMON(conexion,&size,1);
		//unCaughtPokemonTemporal = CaughtConIDs -> caughtPokemon;
		printf("[gameboy] Recibi un Caught, con ID: %d\n", CaughtConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,CaughtConIDs->IDmensaje);
		adminMensajeCaught(CaughtConIDs);
	}
free(CaughtConIDs);
}

void* suscribirseAColaLocalized(){

	printf("\nCola Localized:\n");
	int conexion = crear_conexion(ip, puerto);
	Suscriptor* unSuscriptor1 = malloc(sizeof(Suscriptor));
	unSuscriptor1->IDsuscriptor = 0;

	enviarSuscripcion(unSuscriptor1->IDsuscriptor,conexion,SUSCRIPTOR_LOCALIZEDPOKEMON);
	printf("Envie suscripcion para la cola de mensajes %d",SUSCRIPTOR_LOCALIZEDPOKEMON);
	recv(conexion,&IDsuscripcionLocalized,sizeof(int),MSG_WAITALL);
	printf("\nRecibi mi id como suscriptor: %d\n",IDsuscripcionLocalized);

	int tamanioLista;

	recv(conexion,&tamanioLista,sizeof(int),MSG_WAITALL);
	if(tamanioLista == 0){
		puts("\nNo puedo recibir la lista porque esta vacia");
		printf("Tamaño lista: %d",tamanioLista);
	}
	printf("El tamaño de la lista que voy a recibir es: %d\n",tamanioLista);

	int size;
	int variableQueNoUsoxd;
	int ack = 1;

	LocalizedPokemonConIDs* localizedConIDs;
	LocalizedPokemon* unLocalizedPokemonTemporal;

	for(int i = 0; i<tamanioLista;i++){
		recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
		localizedConIDs = recibir_LOCALIZED_POKEMON(conexion,&size,1);
		unLocalizedPokemonTemporal = localizedConIDs->localizedPokemon;
		printf("[gameboy] Recibi un %s,con ID: %d\n",unLocalizedPokemonTemporal->nombre,localizedConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,localizedConIDs->IDmensaje);
		//adminMensajeLocalized(localizedConIDs);
	}
free(localizedConIDs);
}

void* suscribirseAColaAppeared(){

	printf("\n\nCola Appeared:\n");
	int conexion = crear_conexion(ip, puerto);
	Suscriptor* unSuscriptor1 = malloc(sizeof(Suscriptor));
	unSuscriptor1->IDsuscriptor = 0;
	enviarSuscripcion(unSuscriptor1->IDsuscriptor,conexion,SUSCRIPTOR_APPEAREDPOKEMON);

	printf("Envie suscripcion para la cola de mensajes %d",SUSCRIPTOR_APPEAREDPOKEMON);
	recv(conexion,&IDsuscripcionAppeared,sizeof(int),MSG_WAITALL);
	printf("\nRecibi mi id como suscriptor: %d\n",IDsuscripcionAppeared);

	int tamanioLista;

	recv(conexion,&tamanioLista,sizeof(int),MSG_WAITALL);
	if(tamanioLista == 0){
		puts("\nNo puedo recibir la lista porque esta vacia");
		printf("Tamaño lista: %d",tamanioLista);
	}
	printf("El tamaño de la lista que voy a recibir es: %d\n",tamanioLista);

	int size;
	int variableQueNoUsoxd;
	int ack = 1;

	AppearedPokemonConIDs* appearedConIDs;
	AppearedPokemon* unAppearedPokemonTemporal;

	for(int i = 0; i<tamanioLista;i++){
		recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
		appearedConIDs = recibir_APPEARED_POKEMON(conexion,&size,1,0);
		unAppearedPokemonTemporal = appearedConIDs->appearedPokemon;
		printf("[gameboy] Recibi un %s,con ID: %d\n",unAppearedPokemonTemporal->nombre,appearedConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,appearedConIDs->IDmensaje);
		adminMensajeAppeared(appearedConIDs);

	}

	free(appearedConIDs);
}

// ------------------------------- Funciones Team ------------------------------------ //

void* recibirMensajesAppeared(){
	pthread_t admin;
	void* mensajeRecibido;
	AppearedPokemonConIDs* nuevoAppeared;

	while(1){
		nuevoAppeared = recibir_APPEARED_POKEMON(conexionAppeared, 0, 0, 0);
		send(conexionAppeared, 1, sizeof(int), 0);
		pthread_create(&admin, NULL, (void*)adminMensajeAppeared, nuevoAppeared);
		pthread_detach(admin);
	}
	free(mensajeRecibido);
	free(nuevoAppeared);
}

void* adminMensajeAppeared(AppearedPokemonConIDs* nuevoAppeared){
	if(descartar_appeared_no_deseados(nuevoAppeared)){

		Pokemon* nuevo = malloc(sizeof(Pokemon));
		nuevo -> nombre = nuevoAppeared -> appearedPokemon -> nombre;
		nuevo -> posicion = nuevoAppeared -> appearedPokemon -> coordenadas;
		list_add(nuevosPokemon, nuevo);
		printf("Guarde un Pokemon %s de appeared con el ID de mensaje: %d\n", nuevo->nombre ,nuevoAppeared->IDmensaje);

		sem_post(&nuevosPokemons);

	} else {
		printf("Mensaje Appeared que no es para nosotros\n");
	}
	list_add(mensajesRecibidos, (void*)nuevoAppeared->IDmensaje);
}



void* recibirMensajesLocalized(){
	pthread_t admin;
	void* mensajeRecibido;
	LocalizedPokemonConIDs* nuevoLocalized;
	while(1){
		nuevoLocalized = recibir_LOCALIZED_POKEMON(conexionLocalized, 0, 0);
		send(conexionLocalized, 1, sizeof(int), 0);
		pthread_create(&admin, NULL, (void*)adminMensajeLocalized, nuevoLocalized);
		pthread_detach(admin);
	}
	free(mensajeRecibido);
	free(nuevoLocalized);
}

void* adminMensajeLocalized(LocalizedPokemonConIDs* nuevoLocalized){
	int cantidadObjetivos = 0;
	cantidadObjetivos = obtenerCantidadObjetivo(nuevoLocalized -> localizedPokemon -> nombre);
	int j = 0;
	int cantidadPokemonLocalizados = list_size(nuevoLocalized -> localizedPokemon -> paresOrdenados);
	Pokemon* nuevo = malloc(sizeof(Pokemon));

	if(descartar_localized_no_deseados(nuevoLocalized)){

		for(int i=0; i<cantidadObjetivos && j < cantidadPokemonLocalizados; i++){
		nuevo = elegirMejorUbicacion(nuevoLocalized);
		list_add(nuevosPokemon, nuevo);
		printf("Guarde un Pokemon %s de localized con el ID de mensaje %d\n", nuevo->nombre, nuevoLocalized->IDmensaje);

		bool compararCoordenadas(CoordenadasXY* coordenadas){
			return (nuevo -> posicion.posicionX == coordenadas->posicionX) && (nuevo -> posicion.posicionY == coordenadas->posicionY);
		}
		list_remove_by_condition(nuevoLocalized -> localizedPokemon -> paresOrdenados, (void*)compararCoordenadas);

		sem_post(&nuevosPokemons);

		j++;
		}

	} else {
		//printf("Mensaje Localized que no es para nosotros\n");
	}
	list_add(mensajesRecibidos, nuevoLocalized->IDmensaje); //por si se cae conexion
	free(nuevo);
}




void* recibirMensajesCaught(){
	pthread_t admin;
	void* mensajeRecibido;
	CaughtPokemonConIDs* nuevoCaught;

	while(1){
		nuevoCaught = recibir_CAUGHT_POKEMON(conexionCaught, 0, 1);
		send(conexionCaught, 1, sizeof(int), 0);
		pthread_create(&admin, NULL, adminMensajeCaught, nuevoCaught);
		pthread_detach(admin);
	}
	free(mensajeRecibido);
	free(nuevoCaught);
}

void* adminMensajeCaught(CaughtPokemonConIDs* nuevoCaught){
	if(descartar_caught_no_deseados(nuevoCaught)){

		list_add(nuevosCaught, nuevoCaught);
		printf("Guarde un mensaje Caught");
		sem_post(&mensajesCaught);

	} else {
		printf("Mensaje Caught que no es para nosotros\n");
	}
	list_add(mensajesRecibidos, nuevoCaught->IDmensaje);
}



void enviar_getPokemon(GetPokemon* get_pokemon){

	int id_mensaje;
	GetPokemonConIDs* getPokemonConId = malloc(sizeof(GetPokemonConIDs));

	int conexion = crear_conexion(ip, puerto);

	enviarGetPokemon(get_pokemon, conexion ,0);

	recv(conexion, &id_mensaje, sizeof(int), MSG_WAITALL);

	getPokemonConId->IDmensaje = id_mensaje;

	list_add(mensajesGetEnviados, getPokemonConId);

	free(getPokemonConId);
	//cerrar conexion
}

void enviar_catchPokemon(CatchPokemon* catch_pokemon){

	int id_mensaje;
	CatchPokemonConIDs* catchPokemonConId = malloc(sizeof(CatchPokemonConIDs));

	int conexion = crear_conexion(ip, puerto);

	enviarGetPokemon(catch_pokemon, conexion ,0);

	recv(conexion, &id_mensaje, sizeof(int), MSG_WAITALL);

	catchPokemonConId->IDmensaje = id_mensaje;

	list_add(mensajesCatchEnviados, catchPokemonConId);

	free(catchPokemonConId);
	//cerrar conexion
}

bool descartar_appeared_no_deseados(AppearedPokemonConIDs* appearedPokemonRecibido){
	bool compararNombre(char* nombreObjetivo){
		return strcmp(nombreObjetivo, appearedPokemonRecibido -> appearedPokemon -> nombre);
	}
	bool yaRecibi(int idMensaje){
		return idMensaje != appearedPokemonRecibido -> IDmensaje;
	}
	return list_any_satisfy(objetivoGlobal, (void*)compararNombre) && list_all_satisfy(mensajesRecibidos, (void*)yaRecibi);
}

bool descartar_localized_no_deseados(LocalizedPokemonConIDs* localizedPokemonRecibido){

	bool compararIDcorrelativo (GetPokemonConIDs* mensajeGet){
		return mensajeGet->IDmensaje == localizedPokemonRecibido->IDcorrelativo;
	}

	bool yaRecibi(int idMensaje){
			return idMensaje != localizedPokemonRecibido -> IDmensaje;
	}

	return list_any_satisfy(mensajesGetEnviados, (void*)compararIDcorrelativo) && list_all_satisfy(mensajesRecibidos, (void*)yaRecibi);

}


bool descartar_caught_no_deseados(CaughtPokemonConIDs* caughtPokemonRecibido){

	bool compararIDcorrelativo (CatchPokemonConIDs* mensajeCatch){

		return mensajeCatch->IDmensaje == caughtPokemonRecibido->IDCorrelativo;
	}

	bool yaRecibi(int idMensaje){
			return idMensaje != caughtPokemonRecibido -> IDmensaje;
	}

	return list_any_satisfy(mensajesCatchEnviados, (void*)compararIDcorrelativo) && list_all_satisfy(mensajesRecibidos, (void*)yaRecibi);

}


//-----------Servidor para gameBoy--------


void iniciar_servidor_team(void){
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

	case LOCALIZED_POKEMON:
		pthread_create(&hiloLocalized, NULL, recibirMensajesLocalized, NULL);
		break;

	case APPEARED_POKEMON:
		//pthread_create(&hiloAppeared, NULL, recibirMensajesAppeared, NULL);
		//pthread_detach(hiloAppeared);
		//con la funcion recibirMensajesAppeared me tira segmentation fault

		appeared = recibir_APPEARED_POKEMON(cliente,0,0,0);

		pokemon->nombre = appeared->appearedPokemon->nombre;
		pokemon->posicion.posicionX = appeared->appearedPokemon->coordenadas.posicionX;
		pokemon->posicion.posicionY = appeared->appearedPokemon->coordenadas.posicionY;
		list_add(nuevosPokemon, pokemon);
		puts("\nAgregue un pokemon a la lista");
		break;

	case CAUGHT_POKEMON:
		pthread_create(&hiloCaught, NULL, recibirMensajesCaught, NULL);
		break;
	default:
		puts("\nError en codigo de operacion");
		break;

	}
}
