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
	//suscribirseACola(conexionGameBoy, 0, 0, msgGameBoy);
	//pthread_t hiloGameBoy;
	//pthread_create(&hiloGameBoy, NULL, recibirMensaje, conexionGameBoy);
	//pthread_join(hiloGameBoy,NULL);
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
		nuevoAppeared = recibir_APPEARED_POKEMON(conexionAppeared, 0, 0, 1);
		send(conexionAppeared, 1, sizeof(int), 0);
		pthread_create(&admin, NULL, (void*)adminMensajeAppeared, nuevoAppeared);
		pthread_detach(admin);
	}
	free(mensajeRecibido);
	free(nuevoAppeared);
}

void* adminMensajeAppeared(AppearedPokemonConIDs* nuevoAppeared){
	if(descartar_appeared_no_deseados(nuevoAppeared)){
		list_add(mensajesRecibidos, (void*)nuevoAppeared->IDmensaje);

		Pokemon* nuevo = malloc(sizeof(Pokemon));
		nuevo -> nombre = nuevoAppeared -> appearedPokemon -> nombre;
		nuevo -> posicion = nuevoAppeared -> appearedPokemon -> coordenadas;
		list_add(nuevosPokemon, nuevo);
		printf("Guarde un Pokemon %s de appeared con el ID de mensaje: %d\n", nuevo->nombre ,nuevoAppeared->IDmensaje);

	} else {
		printf("Mensaje Appeared que no es para nosotros\n");
	}
}



void* recibirMensajesLocalized(){
	pthread_t admin;
	void* mensajeRecibido;
	LocalizedPokemonConIDs* nuevoLocalized;
	while(1){
		nuevoLocalized = recibir_LOCALIZED_POKEMON(conexionLocalized, 0, 1);
		send(conexionLocalized, 1, sizeof(int), 0);
		pthread_create(&admin, NULL, (void*)adminMensajeLocalized, nuevoLocalized);
		pthread_detach(admin);
	}
	free(mensajeRecibido);
	free(nuevoLocalized);
}

void* adminMensajeLocalized(LocalizedPokemonConIDs* nuevoLocalized){

	if(descartar_localized_no_deseados(nuevoLocalized)){
		list_add(mensajesRecibidos, nuevoLocalized->IDmensaje); //por si se cae conexion

		Pokemon* nuevo = malloc(sizeof(Pokemon));
		nuevo = elegirMejorUbicacion(nuevoLocalized);
		list_add(nuevosPokemon, nuevo);
		printf("Guarde un Pokemon %s de localized con el ID de mensaje %d\n", nuevo->nombre, nuevoLocalized->IDmensaje);

	} else {
		printf("Mensaje Localized que no es para nosotros\n");
	}
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

		list_add(mensajesRecibidos, nuevoCaught->IDmensaje);
		list_add(nuevosCaught, nuevoCaught);
		printf("Guarde un mensaje Caught");

	} else {
		printf("Mensaje Caught que no es para nosotros\n");
	}
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

bool descartar_appeared_no_deseados(AppearedPokemonConIDs* appearedPokemonRecibido){ //REVISAR SI FUNCIONA
	bool compararNombre(char* nombreObjetivo){
		return nombreObjetivo == appearedPokemonRecibido -> appearedPokemon -> nombre;
	}
	return list_any_satisfy(objetivoGlobal, (void*)compararNombre);
}

bool descartar_localized_no_deseados(LocalizedPokemonConIDs* localizedPokemonRecibido){

	bool compararIDcorrelativo (GetPokemonConIDs* mensajeGet){

		return mensajeGet->IDmensaje == localizedPokemonRecibido->IDcorrelativo;
	}

	return list_any_satisfy(mensajesGetEnviados, (void*)compararIDcorrelativo);

}


bool descartar_caught_no_deseados(CaughtPokemonConIDs* caughtPokemonRecibido){

	bool compararIDcorrelativo (CatchPokemonConIDs* mensajeCatch){

		return mensajeCatch->IDmensaje == caughtPokemonRecibido->IDCorrelativo;
	}

	return list_any_satisfy(mensajesCatchEnviados, (void*)compararIDcorrelativo);

}

