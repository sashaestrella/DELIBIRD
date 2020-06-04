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

	administradorMensajesColas(colaSuscripcion, conexion);

}

void administradorMensajesColas(int op_code, int conexion){

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
					//VALIDAR SI ME SIRVE Y DELEGAR
				}
				break;

			case SUSCRIPTOR_CAUGHTPOKEMON:

				recv(conexion, &cantidadCaughtPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Caught Pokemons: %d\n", cantidadCaughtPokemon);
				CaughtPokemonConIDs* nuevoCaughtPokemonConId;
				for(int i = 0; i<cantidadCaughtPokemon; i++){
					nuevoCaughtPokemonConId = recibir_CAUGHT_POKEMON(conexion, 0, 1);
					//VALIDAR SI ME SIRVE Y DELEGAR
				}
				break;

			case SUSCRIPTOR_LOCALIZEDPOKEMON:

				recv(conexion, &cantidadLocalizedPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Localized Pokemons: %d\n", cantidadLocalizedPokemon);
				LocalizedPokemonConIDs* nuevoLocalizedPokemonConId;
				for(int i = 0; i<cantidadLocalizedPokemon; i++){
					nuevoLocalizedPokemonConId = recibir_LOCALIZED_POKEMON(conexion, 0, 1);
					//VALIDAR SI ME SIRVE Y DELEGAR
				}
				break;
			//Estas tres de abajo serian para el gamecard_______________________________________________________
			case SUSCRIPTOR_GETPOKEMON:

				recv(conexion, &cantidadGetPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Get Pokemons: %d\n", cantidadGetPokemon);
				GetPokemonConIDs* nuevoGetPokemonConId;
				for(int i = 0; i<cantidadGetPokemon; i++){
					nuevoGetPokemonConId = recibir_GET_POKEMON(conexion, 0, 1);
					//VALIDAR SI ME SIRVE Y DELEGAR
				}
				break;

			case SUSCRIPTOR_NEWPOKEMON:

				recv(conexion, &cantidadNewPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de New Pokemons: %d\n", cantidadNewPokemon);
				NewPokemonConIDs* nuevoNewPokemonConId;
				for(int i = 0; i<cantidadNewPokemon; i++){
					nuevoNewPokemonConId = recibir_NEW_POKEMON(conexion, 0, 1);
					//VALIDAR SI ME SIRVE Y DELEGAR
				}
				break;

			case SUSCRIPTOR_CATCHPOKEMON:

				recv(conexion, &cantidadCatchPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Catch Pokemons: %d\n", cantidadCatchPokemon);
				CatchPokemonConIDs* nuevoCatchPokemonConId;
				for(int i = 0; i<cantidadCatchPokemon; i++){
					nuevoCatchPokemonConId = recibir_CATCH_POKEMON(conexion, 0, 1);
					//VALIDAR SI ME SIRVE Y DELEGAR
				}
				break;
	}
}

void* adminMensajes(char* mensajeRecibido){
	printf("Me llego:");
	return EXIT_SUCCESS;
}

void* recibirMensaje(int conexion){
	char* mensajeRecibido = "Maxi";
	pthread_t admin;
	int* tamanioMaximo = 264;
	while(1){
		//mensajeRecibido = recibir_mensaje(conexion,&tamanioMaximo);

		pthread_create(&admin, NULL, adminMensajes, mensajeRecibido);
		pthread_join(admin,NULL);
	}
	return EXIT_SUCCESS;
}

void cargarDatosConexionConfig(){
	t_config* archivo_config =  config_create("team.config");
	ip = config_get_string_value(archivo_config,"IP");
	puerto = config_get_string_value(archivo_config,"PUERTO");
}

