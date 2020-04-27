#include "broker.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void crearColaNewPokemon(){
	t_list* listaNew = list_create();
}

int main(void)
{

	//iniciar_servidor();

	pthread_t hilo1, hilo2;
	char *message1 = "Hilo 1";
	char *message2 = "Hilo 2";

	int iret1, iret2;

	iret1 = pthread_create(&hilo1, NULL,(void*)&iniciar_servidor,NULL);
	if(iret1 != 0){
		printf("Fallo la creacion del hilo: %d",iret1);
	}
	iret2 = pthread_create( &hilo2, NULL, crearColaNewPokemon ,(void*) message2);

	pthread_join( hilo1, NULL);
	//pthread_join( hilo2, NULL);



	printf("Hilo 1 devuelve: %d\n",iret1);
	//printf("Hilo 2 devuelve: %d\n",iret2);
	exit(0);

	return EXIT_SUCCESS;
}
