#include "conexionBroker.h"

void noHayBroker(){
	generarConexion(conexionGameBoy, 0, 0, msgGameBoy);
	pthread_t hiloGameBoy;
	pthread_create(&hiloGameBoy, NULL, recibirMensaje, conexionGameBoy);
	pthread_join(hiloGameBoy,NULL);
}

void generarConexion(int conexion, int colaSuscripcion, int tipoSuscriptor, int IDsuscripcion){

	conexion = crear_conexion(ip, puerto);

	printf( "\nSe creo la conexion con el valor %d \n", conexion);

	send(conexion, &colaSuscripcion, sizeof(int),0);
	//send(conexion, &tipoSuscriptor, sizeof(int),0);

	recv(conexion, &IDsuscripcion, sizeof(int), MSG_WAITALL);

	printf("Suscriptor numero: %d\n", IDsuscripcion);

	int cantidadCaughtPokemon;

	recv(conexion, &cantidadCaughtPokemon, sizeof(int), MSG_WAITALL);

	printf("Cantidad de Caught Pokemons: %d\n", cantidadCaughtPokemon);

	CaughtPokemonConIDs* nuevoCaughtPokemonConId;
	for(int i = 0; i<cantidadCaughtPokemon; i++){
		nuevoCaughtPokemonConId = recibir_CAUGHT_POKEMON(conexion, 0, 1);
		//VALIDAR SI ME SIRVE Y DELEGAR
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

