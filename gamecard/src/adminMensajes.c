#include "adminMensajes.h"

void generarConexiones(int tipoSuscriptor){

	// -- Hilo de suscripcion a cola new -- //
	ParametrosSuscripcion* new = malloc(sizeof(ParametrosSuscripcion));
	new->colaASuscribirse = SUSCRIPTOR_NEWPOKEMON;
	new->nuevoExistente = tipoSuscriptor;

	pthread_t hiloNew;
	pthread_create(&hiloNew, NULL, suscribirseACola, new);


	// -- Hilo de suscripcion a catch -- //
	ParametrosSuscripcion* catch = malloc(sizeof(ParametrosSuscripcion));
	catch->colaASuscribirse = SUSCRIPTOR_CATCHPOKEMON;
	catch->nuevoExistente = tipoSuscriptor;

	pthread_t hiloCatch;
	pthread_create(&hiloCatch, NULL, suscribirseACola, catch);


	// -- Hilo de suscripcion a cola get -- //
	ParametrosSuscripcion* get = malloc(sizeof(ParametrosSuscripcion));
	get->colaASuscribirse = SUSCRIPTOR_GETPOKEMON;
	get->nuevoExistente = tipoSuscriptor;

	pthread_t hiloGet;
	//pthread_create(&hiloGet, NULL, suscribirseACola, get);


	//pthread_join(hiloAppeared,NULL);
	//pthread_join(hiloCaught,NULL);
	//pthread_join(hiloLocalized,NULL);
	free(new);
	free(get);
	free(catch);
}

void abrirEscuchas(){
	pthread_t escuchaNew;
	pthread_create(&escuchaNew, NULL, recibirMensajesNew, NULL);
	pthread_detach(escuchaNew);

	pthread_t escuchaGet;
	pthread_create(&escuchaGet, NULL, recibirMensajesGet, NULL);
	pthread_detach(escuchaGet);

	pthread_t escuchaCatch;
	pthread_create(&escuchaCatch, NULL, recibirMensajesCatch, NULL);
	pthread_detach(escuchaCatch);
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

	int cantidadGetPokemon;
	int cantidadNewPokemon;
	int cantidadCatchPokemon;
	uint32_t codigo;
	uint32_t codigo1;
	uint32_t codigo2;
	switch(op_code){

			case SUSCRIPTOR_NEWPOKEMON:
				conexionNewPokemon = conexion;
				IDsuscripcionNew= IDsuscripcion;

				recv(conexion, &cantidadNewPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de New Pokemons: %d\n", cantidadNewPokemon);
				recv(conexion, &codigo, sizeof(op_code), MSG_WAITALL);
				printf("Codigo de cola: %d\n", codigo);
				NewPokemonConIDs* nuevoNewPokemonConId;
				for(int i = 0; i<cantidadNewPokemon; i++){
					nuevoNewPokemonConId = recibir_NEW_POKEMON(conexion, 0, 1);
					send(conexion, 1, sizeof(int), 0); // No se si pasar el 1 con un void*
					adminMensajeNew(nuevoNewPokemonConId);
					printf("Recibi mensaje com id: %d\n", nuevoNewPokemonConId->IDmensaje);
				}
				break;

			case SUSCRIPTOR_GETPOKEMON:
				conexionGetPokemon = conexion;
				IDsuscripcionGet= IDsuscripcion;

				recv(conexion, &cantidadGetPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Get Pokemons: %d\n", cantidadGetPokemon);
				recv(conexion, &codigo1, sizeof(op_code), MSG_WAITALL);
				printf("Codigo de cola: %d\n", codigo1);
				GetPokemonConIDs* nuevoGetPokemonConId;
				for(int i = 0; i<cantidadGetPokemon; i++){
					nuevoGetPokemonConId = recibir_GET_POKEMON(conexion, 0, 1);
					send(conexion, 1, sizeof(int), 0);
					adminMensajeGet(nuevoGetPokemonConId);
				}
				break;

			case SUSCRIPTOR_CATCHPOKEMON:
				conexionCatch = conexion;
				IDsuscripcionCatch= IDsuscripcion;

				recv(conexion, &cantidadCatchPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Catch Pokemons: %d\n", cantidadCatchPokemon);
				recv(conexion, &codigo2, sizeof(op_code), MSG_WAITALL);
				printf("Codigo de cola: %d\n", codigo2);
				CatchPokemonConIDs* nuevoCatchPokemonConId;
				for(int i = 0; i<cantidadCatchPokemon; i++){
					nuevoCatchPokemonConId = recibir_LOCALIZED_POKEMON(conexion, 0, 1);
					send(conexion, 1, sizeof(int), 0);
					adminMensajeCatch(nuevoCatchPokemonConId);
				}
				break;
			default:
				printf("Llego un mensaje invalido");
				break;
	}
}

// ------------------------------- Funciones Game Card ------------------------------------ //

void* recibirMensajesNew(){
	pthread_t admin;
	void* mensajeRecibido;
	NewPokemonConIDs* nuevoNewPokemon;

	while(1){
		nuevoNewPokemon = recibir_NEW_POKEMON(conexionNewPokemon, 0, 0);
		send(conexionNewPokemon, 1, sizeof(int), 0);
		pthread_create(&admin, NULL, adminMensajeNewPokemon, nuevoNewPokemon);
		pthread_detach(admin);
	}
}

void* adminMensajeNewPokemon(NewPokemonConIDs* nuevoNewPokemon){
	list_add(mensajesRecibidos, nuevoNewPokemon->IDmensaje);
	list_add(nuevosNewPokemon, nuevoNewPokemon);
	printf("Guarde un mensaje New Pokemon con el ID %d\n:" ,nuevoNewPokemon->IDmensaje);
}

void* recibirMensajesGet(){
	pthread_t admin;
	void* mensajeRecibido;
	GetPokemonConIDs* nuevoGetPokemon;
	while(1){

		if(descartar_localized_no_deseados(nuevoGetPokemon)){
			nuevoGetPokemon = recibir_LOCALIZED_POKEMON(conexionGetPokemon, 0, 1);
			send(conexionGetPokemon, 1, sizeof(int), 0);
			pthread_create(&admin, NULL, adminMensajeGetPokemon, nuevoGetPokemon);
			pthread_detach(admin);
		}
	}
}

void* adminMensajeGetPokemon(GetPokemonConIDs* nuevoGetPokemon){
	list_add(mensajesRecibidos, nuevoGetPokemon->IDmensaje);
		list_add(nuevosGetPokemon, nuevoGetPokemon);
		printf("Guarde un mensaje Get");
}

void* recibirMensajesCatch(){
	pthread_t admin;
	void* mensajeRecibido;
	CaughtPokemonConIDs* nuevoCatch;

	while(1){
		if(descartar_caught_no_deseados(nuevoCatch)){
			nuevoCatch = recibir_CAUGHT_POKEMON(conexionCatch, 0, 1);
			send(conexionCatch, 1, sizeof(int), 0);
			pthread_create(&admin, NULL, adminMensajeCatch, nuevoCatch);
			pthread_detach(admin);
		}
	}
}

void* adminMensajeCatch(CatchPokemonConIDs* nuevoCatch){
	list_add(mensajesRecibidos, nuevoCatch->IDmensaje);
	list_add(nuevosCatch, nuevoCatch);
	printf("Guarde un mensaje Catch");
}
