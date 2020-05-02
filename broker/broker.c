#include "broker.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "estructuras.h"
#include <commons/collections/list.h>


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

	pthread_create(&hiloConexion, NULL,(void*)&iniciar_servidor,NULL);





	pthread_join(hiloConexion, NULL); //Puede cambiarse por Detach
/*
	int mensaje;

while(0){
	switch(mensaje){
		case NEW_POKEMON:
			list_add(suscriptoresNewPokemon,&suscriptor);
			break;
		case LOCALIZED_POKEMON:
			list_add(suscriptoresLocalizedPokemon,&suscriptor);
			break;
		case GET_POKEMON:
			list_add(suscriptoresGetPokemon,&suscriptor);
			break;
		case APPEARED_POKEMON:
			list_add(suscriptoresAppearedPokemon,&suscriptor);
			break;
		case CATCH_POKEMON:
			list_add(suscriptoresCatchPokemon,&suscriptor);
			break;
		case CAUGHT_POKEMON:
			list_add(suscriptoresCaughtPokemon,&suscriptor);
			break;
	}
}




	pthread_join(hiloColaNewPokemon, NULL);
	pthread_join(hiloColaLocalizedPokemon, NULL);
	pthread_join(hiloColaGetPokemon, NULL);
	pthread_join(hiloColaAppearedPokemon, NULL);
	pthread_join(hiloColaCatchPokemon, NULL);
	pthread_join(hiloColaCaughtPokemon, NULL);
*/
	printf("Han finalizado los thread.\n");

	//mostrar como quedaron todas las listas de suscriptores

	return EXIT_SUCCESS;
}


