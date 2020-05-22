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
		unNewPokemon1->nombre = "PIKACHU";
		unNewPokemon1->coordenadas.posicionX = 2;
		unNewPokemon1->coordenadas.posicionY = 3;
		unNewPokemon1->cantidad = 3;
		guardarMensajeNewPokemon(unNewPokemon1);

	NewPokemon* unNewPokemon2 = malloc(sizeof(NewPokemon));
		unNewPokemon2->nombre = "ALAKAZAM";
		unNewPokemon2->coordenadas.posicionX = 2;
		unNewPokemon2->coordenadas.posicionY = 3;
		unNewPokemon2->cantidad = 3;
		guardarMensajeNewPokemon(unNewPokemon2);

	LocalizedPokemon* localizedPokemon1 = malloc(sizeof(LocalizedPokemon));
		localizedPokemon1->nombre = "LOCALIZED1";
		localizedPokemon1->cantidadParesOrdenados = 2;
		int numero = 1;
		t_list* pares = list_create();
		list_add(pares,&numero);
		list_add(pares,&numero);
		list_add(pares,&numero);
		list_add(pares,&numero);
		localizedPokemon1->paresOrdenados = pares;
		int tamanioLista = list_size(localizedPokemon1->paresOrdenados);
		printf("\nEl tamaño de pares ordenados es: %d\n",tamanioLista);
		guardarMensajeLocalizedPokemon(localizedPokemon1);

	LocalizedPokemon* localizedPokemon2 = malloc(sizeof(LocalizedPokemon));
		localizedPokemon2->nombre = "LOCALIZED2";
		localizedPokemon2->cantidadParesOrdenados = 3;
		int numero1 = 1;
		int numero2 = 4;
		t_list* pares2 = list_create();
		list_add(pares2,&numero1);
		list_add(pares2,&numero2);
		list_add(pares2,&numero1);
		list_add(pares2,&numero2);
		list_add(pares2,&numero1);
		list_add(pares2,&numero2);
		localizedPokemon2->paresOrdenados = pares2;
		int tamanioLista2 = list_size(localizedPokemon2->paresOrdenados);
		printf("\nEl tamaño de pares ordenados es: %d\n",tamanioLista2);
		guardarMensajeLocalizedPokemon(localizedPokemon2);


	LocalizedPokemon* localizedPokemon3 = malloc(sizeof(LocalizedPokemon));
		localizedPokemon3->nombre = "LOCALIZED3";
		localizedPokemon3->cantidadParesOrdenados = 2;
		int numero3 = 1;
		t_list* pares3 = list_create();
		list_add(pares3,&numero3);
		list_add(pares3,&numero3);
		list_add(pares3,&numero3);
		list_add(pares3,&numero3);
		localizedPokemon3->paresOrdenados = pares3;
		int tamanioLista3 = list_size(localizedPokemon3->paresOrdenados);
		printf("\nEl tamaño de pares ordenados es: %d\n",tamanioLista3);
		guardarMensajeLocalizedPokemon(localizedPokemon3);

	GetPokemon* unGetPokemon1 = malloc(sizeof(GetPokemon));
		unGetPokemon1->nombre = "GET";

		guardarMensajeGetPokemon(unGetPokemon1);

	AppearedPokemon* unAppearedPokemon1 = malloc(sizeof(AppearedPokemon));
		unAppearedPokemon1->nombre = "APPEARED1";
		unAppearedPokemon1->coordenadas.posicionX = 2;
		unAppearedPokemon1->coordenadas.posicionY = 4;
		guardarMensajeAppearedPokemon(unAppearedPokemon1);

	CatchPokemon* unCatchPokemon1 = malloc(sizeof(CatchPokemon));
		unCatchPokemon1->nombre = "CATCH1";
		unCatchPokemon1->coordenadas.posicionX = 5;
		unCatchPokemon1->coordenadas.posicionY = 6;
		printf("Coordenadas catch: %d %d\n",unCatchPokemon1->coordenadas.posicionX,unCatchPokemon1->coordenadas.posicionY);
		guardarMensajeCatchPokemon(unCatchPokemon1);

	CaughtPokemon* unCaughtPokemon1 = malloc(sizeof(CaughtPokemon));
		unCaughtPokemon1->atrapar = 1;
		guardarMensajeCaughtPokemon(unCaughtPokemon1);

	//El sistema tira un error si no se creó correctamente el hilo
		if(pthread_create(&hiloConexion, NULL,(void*)&iniciar_servidor,NULL) != 0){
				errorExit("El hilo no pudo ser creado");
		}


	pthread_join(hiloConexion, NULL); //Puede cambiarse por Detach

	printf("Han finalizado los thread.\n");

	return EXIT_SUCCESS;
}


