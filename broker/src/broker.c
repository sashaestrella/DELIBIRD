#include "broker.h"

void errorExit(char* strerr){
	perror(strerr);
	exit(1);
}

int main(void)
{

	//iniciar_servidor();

	pthread_t hiloConexion;

	New_Pokemon = list_create();
	Localized_Pokemon = list_create();
	Get_Pokemon = list_create();
	Appeared_Pokemon = list_create();
	Catch_Pokemon = list_create();
	Caught_Pokemon = list_create();
	suscriptores_new_pokemon = list_create();

	NewPokemon* unNewPokemon = malloc(sizeof(NewPokemon));
	char* nombrePokemon = malloc(8);
	nombrePokemon = "PIKACHU";

	unNewPokemon->nombre = nombrePokemon;
	unNewPokemon->coordenadas.posicionX = 2;
	unNewPokemon->coordenadas.posicionY = 3;
	unNewPokemon->cantidad = 3;
	guardarMensajeNewPokemon(unNewPokemon);
	NewPokemon* unNewPokemon2 = malloc(sizeof(NewPokemon));
	char* nombrePokemon2 = malloc(9);
	nombrePokemon2= "ALAKAZAM";
	unNewPokemon->nombre = nombrePokemon2;
	unNewPokemon->coordenadas.posicionX = 2;
	unNewPokemon->coordenadas.posicionY = 3;
	unNewPokemon->cantidad = 3;
	guardarMensajeNewPokemon(unNewPokemon2);


	//El sistema tira un error si no se creó correctamente el hilo
		if(pthread_create(&hiloConexion, NULL,(void*)&iniciar_servidor,NULL) != 0){
				errorExit("El hilo no pudo ser creado");
		}


	pthread_join(hiloConexion, NULL); //Puede cambiarse por Detach

	printf("Han finalizado los thread.\n");

	return EXIT_SUCCESS;
}


