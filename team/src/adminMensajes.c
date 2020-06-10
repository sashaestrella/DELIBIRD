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
	//pthread_create(&hiloLocalized, NULL, suscribirseACola, localized);


	//pthread_join(hiloAppeared,NULL);
	//pthread_join(hiloCaught,NULL);
	//pthread_join(hiloLocalized,NULL);
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
	int cantidadGetPokemon;
	int cantidadNewPokemon;
	int cantidadCatchPokemon;
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
				printf("Codigo: %d\n", codigo);
				AppearedPokemonConIDs* nuevoAppearedPokemonConId;
				for(int i = 0; i<cantidadAppearedPokemon; i++){
					nuevoAppearedPokemonConId = recibir_APPEARED_POKEMON(conexion, 0, 1, 0);
					enviarACK(IDsuscripcion, nuevoAppearedPokemonConId->IDmensaje, APPEARED_POKEMON, conexion);
					//adminMensajeAppeared(nuevoAppearedPokemonConId);
					printf("Recibi mensaje com id: %d\n", nuevoAppearedPokemonConId->IDmensaje);
				}
				break;

			case SUSCRIPTOR_CAUGHTPOKEMON:
				conexionCaught = conexion;
				IDsuscripcionCaught = IDsuscripcion;

				recv(conexion, &cantidadCaughtPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Caught Pokemons: %d\n", cantidadCaughtPokemon);
				recv(conexion, &codigo1, sizeof(op_code), MSG_WAITALL);
				printf("Codigo: %d\n", codigo1);
				CaughtPokemonConIDs* nuevoCaughtPokemonConId;
				for(int i = 0; i<cantidadCaughtPokemon; i++){
					nuevoCaughtPokemonConId = recibir_CAUGHT_POKEMON(conexion, 0, 1);
					//enviarACK(IDsuscripcion, nuevoCaughtPokemonConId->IDmensaje, CAUGHT_POKEMON, conexion);
					//adminMensajeCaught(nuevoCaughtPokemonConId);
				}
				break;

			case SUSCRIPTOR_LOCALIZEDPOKEMON:
				conexionLocalized = conexion;
				IDsuscripcionLocalized = IDsuscripcion;

				recv(conexion, &cantidadLocalizedPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Localized Pokemons: %d\n", cantidadLocalizedPokemon);
				recv(conexion, &codigo2, sizeof(op_code), MSG_WAITALL);
				printf("Codigo: %d\n", codigo2);
				LocalizedPokemonConIDs* nuevoLocalizedPokemonConId;
				for(int i = 0; i<cantidadLocalizedPokemon; i++){
					nuevoLocalizedPokemonConId = recibir_LOCALIZED_POKEMON(conexion, 0, 1);
					enviarACK(IDsuscripcion, nuevoLocalizedPokemonConId->IDmensaje, LOCALIZED_POKEMON, conexion);
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
		enviarACK(IDsuscripcionAppeared, nuevoAppeared->IDmensaje, APPEARED_POKEMON, conexionAppeared);
		pthread_create(&admin, NULL, adminMensajeAppeared, nuevoAppeared);
		pthread_detach(admin);
	}
}

void* adminMensajeAppeared(AppearedPokemonConIDs* nuevoAppeared){
	list_add(mensajesRecibidos, nuevoAppeared->IDmensaje);
	list_add(nuevosAppeared, nuevoAppeared);
	printf("Guarde un mensaje appeared");
}

void* recibirMensajesLocalized(){
	pthread_t admin;
	void* mensajeRecibido;
	LocalizedPokemonConIDs* nuevoLocalized;
	while(1){
		nuevoLocalized = recibir_LOCALIZED_POKEMON(conexionLocalized, 0, 1);
		enviarACK(IDsuscripcionLocalized, nuevoLocalized->IDmensaje, LOCALIZED_POKEMON, conexionLocalized);
		pthread_create(&admin, NULL, adminMensajeLocalized, nuevoLocalized);
		pthread_detach(admin);
	}
}

void* adminMensajeLocalized(LocalizedPokemonConIDs* nuevoLocalized){
	list_add(mensajesRecibidos, nuevoLocalized->IDmensaje);
		list_add(nuevosLocalized, nuevoLocalized);
		printf("Guarde un mensaje localized");
}

void* recibirMensajesCaught(){
	pthread_t admin;
	void* mensajeRecibido;
	CaughtPokemonConIDs* nuevoCaught;

	while(1){
		nuevoCaught = recibir_CAUGHT_POKEMON(conexionCaught, 0, 1);
		enviarACK(IDsuscripcionCaught, nuevoCaught->IDmensaje, CAUGHT_POKEMON, conexionCaught);
		pthread_create(&admin, NULL, adminMensajeCaught, nuevoCaught);
		pthread_detach(admin);
	}
}

void* adminMensajeCaught(CaughtPokemonConIDs* nuevoCaught){
	list_add(mensajesRecibidos, nuevoCaught->IDmensaje);
	list_add(nuevosCaught, nuevoCaught);
	printf("Guarde un mensaje Caught");
}





