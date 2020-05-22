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
	localizedPokemon1->nombre = "PIKACHU";
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




	//El sistema tira un error si no se creó correctamente el hilo
		if(pthread_create(&hiloConexion, NULL,(void*)&iniciar_servidor,NULL) != 0){
				errorExit("El hilo no pudo ser creado");
		}


	pthread_join(hiloConexion, NULL); //Puede cambiarse por Detach

	printf("Han finalizado los thread.\n");

	return EXIT_SUCCESS;
}


