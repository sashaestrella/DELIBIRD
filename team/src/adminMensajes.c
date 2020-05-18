#include "adminMensajes.h"

void noHayBroker(){
	generarConexion(conexionGameBoy, msgGameBoy);
	pthread_t hiloGameBoy;
	pthread_create(&hiloGameBoy, NULL, recibirMensaje, conexionGameBoy);
	pthread_join(hiloGameBoy,NULL);
}

void generarConexiones(){

	generarConexion(conexionAppeared, msgAppeared);
	//pthread_t hiloAppeared;
	//pthread_create(&hiloAppeared, NULL, recibirMensaje, conexionAppeared);
	//pthread_join(hiloAppeared,NULL);


	/*generarConexion(conexionLocalized, msgLocalized);
	pthread_t hiloLocalized;
	pthread_create(&hiloLocalized, NULL, recibirMensaje, conexionLocalized);
	pthread_detach(hiloLocalized);


	generarConexion(conexionCaught, msgCaught);
	pthread_t hiloCaught;
	pthread_create(&hiloCaught, NULL, recibirMensaje, conexionCaught);
	pthread_detach(hiloCaught);*/

}



void generarConexion(int conexion, char* mensajeAEnviar){

	conexion = crear_conexion(ip, puerto);

	printf( "\nSe creo la conexion con el valor %d \n", conexion);

	int aEnviar = 8;
	int soyNuevo = 0;
	//enviar_mensaje(mensajeAEnviar, conexion);
	send(conexion, &aEnviar, sizeof(int),0);
	send(conexion, &soyNuevo, sizeof(int),0);

	char* mensajeRecibido = malloc(sizeof(int));

	//mensajeRecibido = recibir_mensaje(conexion,sizeof(int); //recibirSmensaje(conexion);

	//printf("Mensaje: %s\n", mensajeRecibido);
}

void* recibirMensaje(int conexion){
	char* mensajeRecibido;
	pthread_t admin;
	int* tamanioMaximo = 264;
	while(1){
		mensajeRecibido = recibir_mensaje(conexion,&tamanioMaximo);
		pthread_create(&admin, NULL, adminMensajes, mensajeRecibido);
		pthread_join(admin,NULL);
	}
	return EXIT_SUCCESS;
}

void* adminMensajes(char* mensaje){
	char* tipo;
	char* pokemon;
	tipo = strtok(mensaje, " ");
	pokemon = strtok(NULL, " ");
	if(!strcmp(tipo,"APPEARED")){
		puts("A");
	}
	if(!strcmp(tipo,"LOCALIZED")){
		puts("L");
	}
	if(!strcmp(tipo,"CAUGHT")){
		puts("C");
	}
	return EXIT_SUCCESS;
}
