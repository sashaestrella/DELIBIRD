#include "conexionBroker.h"

void noHayBroker(){
	//suscribirseACola(conexionGameBoy, 0, 0, msgGameBoy);
	//pthread_t hiloGameBoy;
	//pthread_create(&hiloGameBoy, NULL, recibirMensaje, conexionGameBoy);
	//pthread_join(hiloGameBoy,NULL);
}

void* suscribirseACola(ParametrosSuscripcion* datos){

	int conexion 			= datos->conexionCola;
	op_code colaSuscripcion = datos->colaASuscribirse;
	int soyNuevo 			= datos->nuevoExistente;
	uint32_t IDsuscripcion 	= datos->IDSuscripcion;

	conexion = crear_conexion(ip, puerto);

	printf( "\nSe creo la conexion con el valor %d \n", conexion);

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

	switch(op_code){

			case SUSCRIPTOR_APPEAREDPOKEMON:

				recv(conexion, &cantidadAppearedPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Appeared Pokemons: %d\n", cantidadAppearedPokemon);
				AppearedPokemonConIDs* nuevoAppearedPokemonConId;
				for(int i = 0; i<cantidadAppearedPokemon; i++){
					nuevoAppearedPokemonConId = recibir_APPEARED_POKEMON(conexion, 0, 1, 0);
					enviarACK(IDsuscripcion, nuevoAppearedPokemonConId->IDmensaje, APPEARED_POKEMON, conexion);
					adminMensajeAppeared(nuevoAppearedPokemonConId);
				}
				break;

			case SUSCRIPTOR_CAUGHTPOKEMON:

				recv(conexion, &cantidadCaughtPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Caught Pokemons: %d\n", cantidadCaughtPokemon);
				CaughtPokemonConIDs* nuevoCaughtPokemonConId;
				for(int i = 0; i<cantidadCaughtPokemon; i++){
					nuevoCaughtPokemonConId = recibir_CAUGHT_POKEMON(conexion, 0, 1);
					enviarACK(IDsuscripcion, nuevoCaughtPokemonConId->IDmensaje, CAUGHT_POKEMON, conexion);
					adminMensajeCaught(nuevoCaughtPokemonConId);
				}
				break;

			case SUSCRIPTOR_LOCALIZEDPOKEMON:

				recv(conexion, &cantidadLocalizedPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Localized Pokemons: %d\n", cantidadLocalizedPokemon);
				LocalizedPokemonConIDs* nuevoLocalizedPokemonConId;
				for(int i = 0; i<cantidadLocalizedPokemon; i++){
					nuevoLocalizedPokemonConId = recibir_LOCALIZED_POKEMON(conexion, 0, 1);
					enviarACK(IDsuscripcion, nuevoLocalizedPokemonConId->IDmensaje, LOCALIZED_POKEMON, conexion);
					adminMensajeLocalized(nuevoLocalizedPokemonConId);
				}
				break;
			//Estas tres de abajo serian para el gamecard_______________________________________________________
			case SUSCRIPTOR_GETPOKEMON:

				recv(conexion, &cantidadGetPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Get Pokemons: %d\n", cantidadGetPokemon);
				GetPokemonConIDs* nuevoGetPokemonConId;
				for(int i = 0; i<cantidadGetPokemon; i++){
					nuevoGetPokemonConId = recibir_GET_POKEMON(conexion, 0, 1);
					enviarACK(IDsuscripcion, nuevoGetPokemonConId->IDmensaje, GET_POKEMON, conexion);
					//VALIDAR SI ME SIRVE Y DELEGAR
				}
				break;

			case SUSCRIPTOR_NEWPOKEMON:

				recv(conexion, &cantidadNewPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de New Pokemons: %d\n", cantidadNewPokemon);
				NewPokemonConIDs* nuevoNewPokemonConId;
				for(int i = 0; i<cantidadNewPokemon; i++){
					nuevoNewPokemonConId = recibir_NEW_POKEMON(conexion, 0, 1);
					enviarACK(IDsuscripcion, nuevoNewPokemonConId->IDmensaje, NEW_POKEMON, conexion);
					//VALIDAR SI ME SIRVE Y DELEGAR
				}
				break;

			case SUSCRIPTOR_CATCHPOKEMON:

				recv(conexion, &cantidadCatchPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Catch Pokemons: %d\n", cantidadCatchPokemon);
				CatchPokemonConIDs* nuevoCatchPokemonConId;
				for(int i = 0; i<cantidadCatchPokemon; i++){
					nuevoCatchPokemonConId = recibir_CATCH_POKEMON(conexion, 0, 1);
					enviarACK(IDsuscripcion, nuevoCatchPokemonConId->IDmensaje, CATCH_POKEMON, conexion);
					//VALIDAR SI ME SIRVE Y DELEGAR
				}
				break;
			default:
				printf("Llego un mensaje invalido");
				break;
	}
}

// ------------------------------- Funciones Team ------------------------------------ //

void* recibirMensajesAppeared(int conexion, int IDsuscripcion){
	pthread_t admin;
	void* mensajeRecibido;
	AppearedPokemonConIDs* nuevoAppeared;

	while(1){
		nuevoAppeared = recibir_APPEARED_POKEMON(conexion, 0, 0, 1);
		enviarACK(IDsuscripcion, nuevoAppeared->IDmensaje, APPEARED_POKEMON, conexion);
		pthread_create(&admin, NULL, adminMensajeAppeared, nuevoAppeared);
		pthread_detach(admin);
	}
}

void* adminMensajeAppeared(AppearedPokemonConIDs* nuevoAppeared){
	list_add(mensajesRecibidos, nuevoAppeared->IDmensaje);
	list_add(nuevosAppeared, nuevoAppeared);
	printf("Guarde un mensaje appeared");
}

void* recibirMensajesLocalized(int conexion, int IDsuscripcion){
	pthread_t admin;
	void* mensajeRecibido;
	LocalizedPokemonConIDs* nuevoLocalized;
	while(1){
		nuevoLocalized = recibir_LOCALIZED_POKEMON(conexion, 0, 1);
		enviarACK(IDsuscripcion, nuevoLocalized->IDmensaje, LOCALIZED_POKEMON, conexion);
		pthread_create(&admin, NULL, adminMensajeLocalized, nuevoLocalized);
		pthread_detach(admin);
	}
}

void* adminMensajeLocalized(LocalizedPokemonConIDs* nuevoLocalized){
	list_add(mensajesRecibidos, nuevoLocalized->IDmensaje);
		list_add(nuevosLocalized, nuevoLocalized);
		printf("Guarde un mensaje localized");
}

void* recibirMensajesCaught(int conexion, int IDsuscripcion){
	pthread_t admin;
	void* mensajeRecibido;
	CaughtPokemonConIDs* nuevoCaught;

	while(1){
		nuevoCaught = recibir_CAUGHT_POKEMON(conexion, 0, 1);
		enviarACK(IDsuscripcion, nuevoCaught->IDmensaje, CAUGHT_POKEMON, conexion);
		pthread_create(&admin, NULL, adminMensajeCaught, nuevoCaught);
		pthread_detach(admin);
	}
}

void* adminMensajeCaught(CaughtPokemonConIDs* nuevoCaught){
	list_add(mensajesRecibidos, nuevoCaught->IDmensaje);
	list_add(nuevosCaught, nuevoCaught);
	printf("Guarde un mensaje Caught");
}



// ---------------------------- Funciones Game Card ---------------------------------- //


// ----------------------------------------------------------------------------------- //

void enviarACK(int IDsuscriptor, int IDmensaje, int cola, int conexion){
	ACKmensaje confirmacion;
	confirmacion.IDmensaje = IDmensaje;
	confirmacion.IDsuscriptor = IDsuscriptor;
	confirmacion.numeroDeColaALaQuePertenece = cola;
	send(conexion, &confirmacion, sizeof(ACKmensaje), 0);
}


void cargarDatosConexionConfig(){
	t_config* archivo_config =  config_create("team.config");
	ip = config_get_string_value(archivo_config,"IP");
	puerto = config_get_string_value(archivo_config,"PUERTO");
}

