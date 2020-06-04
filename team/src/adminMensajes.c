#include "adminMensajes.h"

void generarConexiones(){

	//Hilo de suscripcion a cola appeared
	ParametrosSuscripcion* appeared = malloc(sizeof(ParametrosSuscripcion));
	appeared->conexionCola = conexionAppeared;
	appeared->colaASuscribirse = SUSCRIPTOR_APPEAREDPOKEMON;
	appeared->nuevoExistente = 0;
	appeared->IDSuscripcion = IDsuscripcionAppeared;

	pthread_t hiloAppeared;
	pthread_create(&hiloAppeared, NULL, suscribirseACola, appeared);

	//Hilo de suscripcion a cola caught
	ParametrosSuscripcion* caught = malloc(sizeof(ParametrosSuscripcion));
	caught->conexionCola = conexionCaught;
	caught->colaASuscribirse = SUSCRIPTOR_CAUGHTPOKEMON;
	caught->nuevoExistente = 0;
	caught->IDSuscripcion = IDsuscripcionCaught;

	pthread_t hiloCaught;
	pthread_create(&hiloCaught, NULL, suscribirseACola, caught);

	//Hilo de suscripcion a cola localized
	ParametrosSuscripcion* localized = malloc(sizeof(ParametrosSuscripcion));
	localized->conexionCola = conexionLocalized;
	localized->colaASuscribirse = SUSCRIPTOR_LOCALIZEDPOKEMON;
	localized->nuevoExistente = 0;
	localized->IDSuscripcion = IDsuscripcionLocalized;

	pthread_t hiloLocalized;
	pthread_create(&hiloLocalized, NULL, suscribirseACola, localized);


	//pthread_join(hiloAppeared,NULL);
	//pthread_join(hiloCaught,NULL);
	//pthread_join(hiloLocalized,NULL);
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





