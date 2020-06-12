#include "adminMensajes.h"

void generarConexiones(int tipoSuscriptor){

	// -- Hilo de suscripcion a cola appeared -- //
	ParametrosSuscripcion* appeared = malloc(sizeof(ParametrosSuscripcion));
	appeared->colaASuscribirse = SUSCRIPTOR_APPEAREDPOKEMON;
	appeared->nuevoExistente = tipoSuscriptor;

	pthread_t hiloAppeared;
	pthread_create(&hiloAppeared, NULL, suscribirseACola, appeared);


	// -- Hilo de suscripcion a cola caught -- //
	ParametrosSuscripcion* caught = malloc(sizeof(ParametrosSuscripcion));
	caught->colaASuscribirse = SUSCRIPTOR_CAUGHTPOKEMON;
	caught->nuevoExistente = tipoSuscriptor;

	pthread_t hiloCaught;
	pthread_create(&hiloCaught, NULL, suscribirseACola, caught);


	// -- Hilo de suscripcion a cola localized -- //
	ParametrosSuscripcion* localized = malloc(sizeof(ParametrosSuscripcion));
	localized->colaASuscribirse = SUSCRIPTOR_LOCALIZEDPOKEMON;
	localized->nuevoExistente = tipoSuscriptor;

	pthread_t hiloLocalized;
	pthread_create(&hiloLocalized, NULL, suscribirseACola, localized);


	//pthread_join(hiloAppeared,NULL);
	//pthread_join(hiloCaught,NULL);
	//pthread_join(hiloLocalized,NULL);
	free(appeared);
	free(localized);
	free(caught);
}

void abrirEscuchas(){
	pthread_t escuchaAppeared;
	pthread_create(&escuchaAppeared, NULL, recibirMensajesAppeared, NULL);
	pthread_detach(escuchaAppeared);

	pthread_t escuchaLocalized;
	pthread_create(&escuchaLocalized, NULL, recibirMensajesLocalized, NULL);
	pthread_detach(escuchaLocalized);

	pthread_t escuchaCaught;
	pthread_create(&escuchaCaught, NULL, recibirMensajesCaught, NULL);
	pthread_detach(escuchaCaught);
}

// ------------------------------------ AUXILIARES ------------------------------------ //

void noHayBroker(){
	//suscribirseACola(conexionGameBoy, 0, 0, msgGameBoy);
	//pthread_t hiloGameBoy;
	//pthread_create(&hiloGameBoy, NULL, recibirMensaje, conexionGameBoy);
	//pthread_join(hiloGameBoy,NULL);
}

void* suscribirseACola(ParametrosSuscripcion* datos){

	int conexion;
	op_code colaSuscripcion = datos->colaASuscribirse;
	int soyNuevo 			= datos->nuevoExistente;
	uint32_t IDsuscripcion;

	conexion = crear_conexion(ip, puerto);

	printf( "\nSe creo la suscripcion con el valor %d \n", conexion);

	send(conexion, &colaSuscripcion, sizeof(int),0);
	//send(conexion, &soyNuevo, sizeof(int),0);

	recv(conexion, &IDsuscripcion, sizeof(int), MSG_WAITALL);

	printf("Suscriptor numero: %d\n", IDsuscripcion);

	administradorMensajesColas(colaSuscripcion, conexion, IDsuscripcion);

}

void* administradorMensajesColas(int op_code, int conexion, int IDsuscripcion){

	int cantidadAppearedPokemon;
	int cantidadCaughtPokemon;
	int cantidadLocalizedPokemon;

	uint32_t codigo;
	uint32_t codigo1;
	uint32_t codigo2;
	switch(op_code){

			case SUSCRIPTOR_APPEAREDPOKEMON:
				conexionAppeared = conexion;
				IDsuscripcionAppeared = IDsuscripcion;

				recv(conexion, &cantidadAppearedPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Appeared Pokemons: %d\n", cantidadAppearedPokemon);
				recv(conexion, &codigo, sizeof(op_code), MSG_WAITALL);
				printf("Codigo de cola: %d\n", codigo);
				AppearedPokemonConIDs* nuevoAppearedPokemonConId;
				for(int i = 0; i<cantidadAppearedPokemon; i++){
					nuevoAppearedPokemonConId = recibir_APPEARED_POKEMON(conexion, 0, 1, 0);
					send(conexion, 1, sizeof(int), 0); // No se si pasar el 1 con un void*
					adminMensajeAppeared(nuevoAppearedPokemonConId);
					printf("Recibi mensaje com id: %d\n", nuevoAppearedPokemonConId->IDmensaje);
				}
				break;

			case SUSCRIPTOR_CAUGHTPOKEMON:
				conexionCaught = conexion;
				IDsuscripcionCaught = IDsuscripcion;

				recv(conexion, &cantidadCaughtPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Caught Pokemons: %d\n", cantidadCaughtPokemon);
				recv(conexion, &codigo1, sizeof(op_code), MSG_WAITALL);
				printf("Codigo de cola: %d\n", codigo1);
				CaughtPokemonConIDs* nuevoCaughtPokemonConId;
				for(int i = 0; i<cantidadCaughtPokemon; i++){
					nuevoCaughtPokemonConId = recibir_CAUGHT_POKEMON(conexion, 0, 1);
					send(conexion, 1, sizeof(int), 0);
					adminMensajeCaught(nuevoCaughtPokemonConId);
					printf("Recibi mensaje com id: %d\n", nuevoCaughtPokemonConId->IDmensaje);
				}
				break;

			case SUSCRIPTOR_LOCALIZEDPOKEMON:
				conexionLocalized = conexion;
				IDsuscripcionLocalized = IDsuscripcion;

				recv(conexion, &cantidadLocalizedPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Localized Pokemons: %d\n", cantidadLocalizedPokemon);
				recv(conexion, &codigo2, sizeof(op_code), MSG_WAITALL);
				printf("Codigo de cola: %d\n", codigo2);
				LocalizedPokemonConIDs* nuevoLocalizedPokemonConId;
				for(int i = 0; i<cantidadLocalizedPokemon; i++){
					nuevoLocalizedPokemonConId = recibir_LOCALIZED_POKEMON(conexion, 0, 1);
					send(conexion, 1, sizeof(int), 0);
					adminMensajeLocalized(nuevoLocalizedPokemonConId);
				}
				break;
			default:
				printf("Llego un mensaje invalido");
				break;
	}
}

// ------------------------------- Funciones Team ------------------------------------ //

void* recibirMensajesAppeared(){
	pthread_t admin;
	void* mensajeRecibido;
	AppearedPokemonConIDs* nuevoAppeared;

	while(1){
		nuevoAppeared = recibir_APPEARED_POKEMON(conexionAppeared, 0, 0, 1);
		send(conexionAppeared, 1, sizeof(int), 0);
		pthread_create(&admin, NULL, adminMensajeAppeared, nuevoAppeared);
		pthread_detach(admin);
	}
	free(mensajeRecibido);
	free(nuevoAppeared);
}

void* adminMensajeAppeared(AppearedPokemonConIDs* nuevoAppeared){
	if(descartar_appeared_no_deseados(nuevoAppeared)){

		list_add(mensajesRecibidos, nuevoAppeared->IDmensaje);

		char* nombre = nuevoAppeared -> appearedPokemon -> nombre;
		GetPokemon* mensajeGet = malloc(sizeof(GetPokemon));
		mensajeGet -> nombre = nombre;
		mensajeGet -> tamanioNombrePokemon = strlen(nombre)+1;
		enviar_getPokemon(mensajeGet);
	}
	printf("Guarde un mensaje appeared con el ID %d\n:" ,nuevoAppeared->IDmensaje);
}



void* recibirMensajesLocalized(){
	pthread_t admin;
	void* mensajeRecibido;
	LocalizedPokemonConIDs* nuevoLocalized;
	while(1){
		nuevoLocalized = recibir_LOCALIZED_POKEMON(conexionLocalized, 0, 1);
		send(conexionLocalized, 1, sizeof(int), 0);
		pthread_create(&admin, NULL, adminMensajeLocalized, nuevoLocalized);
		pthread_detach(admin);
	}
	free(mensajeRecibido);
	free(nuevoLocalized);
}

void* adminMensajeLocalized(LocalizedPokemonConIDs* nuevoLocalized){
	if(descartar_localized_no_deseados(nuevoLocalized)){

		list_add(mensajesRecibidos, nuevoLocalized->IDmensaje);
		list_add(nuevosLocalized, nuevoLocalized);
		printf("Guarde un mensaje localized");

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

