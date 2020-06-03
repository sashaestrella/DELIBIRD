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
	suscriptores_localized_pokemon = list_create();
	suscriptores_get_pokemon = list_create();
	suscriptores_appeared_pokemon = list_create();
	suscriptores_catch_pokemon = list_create();
	suscriptores_caught_pokemon = list_create();


		NewPokemon* unNewPokemon1 = malloc(sizeof(NewPokemon));
		MensajeNewPokemon* mensaje1;
		char* nombre = malloc(8);
		nombre = "PIKACHU";
		unNewPokemon1->nombre = nombre;
		unNewPokemon1->coordenadas.posicionX = 2;
		unNewPokemon1->coordenadas.posicionY = 3;
		unNewPokemon1->cantidad = 3;

		mensaje1 = guardarMensajeNewPokemon(unNewPokemon1);

	NewPokemon* unNewPokemon2 = malloc(sizeof(NewPokemon));
	MensajeNewPokemon* mensaje2;
		char* nombre2 = malloc(9);
		nombre2 = "ALAKAZAM";
		unNewPokemon2->nombre = nombre2;
		unNewPokemon2->coordenadas.posicionX = 2;
		unNewPokemon2->coordenadas.posicionY = 3;
		unNewPokemon2->cantidad = 3;

		mensaje2 = guardarMensajeNewPokemon(unNewPokemon2);


	LocalizedPokemon* localizedPokemon1 = malloc(sizeof(LocalizedPokemon));
	MensajeLocalizedPokemon* mensaje3;
		localizedPokemon1->nombre = "LOCALIZED1";
		localizedPokemon1->cantidadParesOrdenados = 2;
		int numero = 1;
		int idCorrelativo = 1;
		t_list* pares = list_create();
		list_add(pares,&numero);
		list_add(pares,&numero);
		list_add(pares,&numero);
		list_add(pares,&numero);
		localizedPokemon1->paresOrdenados = pares;
		int tamanioLista = list_size(localizedPokemon1->paresOrdenados);
		printf("\nEl tamaño de pares ordenados de localized es: %d\n",tamanioLista);

		mensaje3 = guardarMensajeLocalizedPokemon(localizedPokemon1,idCorrelativo);


	GetPokemon* unGetPokemon1 = malloc(sizeof(GetPokemon));
	MensajeGetPokemon* mensaje4;
		unGetPokemon1->nombre = "GET";

		mensaje4 = guardarMensajeGetPokemon(unGetPokemon1);

	AppearedPokemon* unAppearedPokemon1 = malloc(sizeof(AppearedPokemon));
	MensajeAppearedPokemon* mensaje5;
		unAppearedPokemon1->nombre = "APPEARED1";
		unAppearedPokemon1->coordenadas.posicionX = 2;
		unAppearedPokemon1->coordenadas.posicionY = 4;

		mensaje5 = guardarMensajeAppearedPokemon(unAppearedPokemon1);

	CatchPokemon* unCatchPokemon1 = malloc(sizeof(CatchPokemon));
	MensajeCatchPokemon* mensaje6;
		unCatchPokemon1->nombre = "CATCH1";
		unCatchPokemon1->coordenadas.posicionX = 5;
		unCatchPokemon1->coordenadas.posicionY = 6;
		printf("Coordenadas catch: %d %d\n",unCatchPokemon1->coordenadas.posicionX,unCatchPokemon1->coordenadas.posicionY);

		mensaje6 = guardarMensajeCatchPokemon(unCatchPokemon1);

	CaughtPokemon* unCaughtPokemon1 = malloc(sizeof(CaughtPokemon));
	MensajeCaughtPokemon* mensaje7;
		int idCorrelativoCaught = 1;
		unCaughtPokemon1->atrapar = 1;

		mensaje7 = guardarMensajeCaughtPokemon(unCaughtPokemon1,idCorrelativoCaught);


	//El sistema tira un error si no se creó correctamente el hilo
		if(pthread_create(&hiloConexion, NULL,(void*)&iniciar_servidor,NULL) != 0){
				errorExit("El hilo no pudo ser creado");
		}


	pthread_join(hiloConexion, NULL); //Puede cambiarse por Detach

	printf("Han finalizado los thread.\n");

	return EXIT_SUCCESS;
}


