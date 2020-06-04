#include "adminMensajes.h"

void generarConexiones(){

	//generarConexion es un hilo que genera los otros hilos

	generarConexion(conexionAppeared, 13, 0, IDsuscripcionAppeared);
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
