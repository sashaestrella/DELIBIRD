#include "funcionesEntrenador.h"


void* planificar(Entrenador* entrenador){

	printf("\nSoy el entrenador %d y se creo mi hilo.\n", entrenador->ID);

	//-----------------Espera que el hilo main le diga que se puede mover
	pthread_mutex_lock(&mutexTeam);
	if(condicionTeam[entrenador->ID-1] == 0){
	printf("Estoy esperando entrenador %d\n",entrenador->ID );
	pthread_cond_wait(&cond[entrenador->ID -1], &mutexTeam);
	}
	pthread_mutex_unlock(&mutexTeam);
	//-------------------------------------------------------------------


	//---------------Le dice al hilo main que ya se movio
	pthread_mutex_lock(&mutexTeam);

	entrenador->posicion.posicionX = entrenador->posicion.posicionX + 1;
	entrenador->posicion.posicionY = entrenador->posicion.posicionY + 1;
	puts("\nSe movio 1 en cada eje");
	condicionTeam[entrenador->ID-1]=2;
	pthread_cond_signal(&cond[entrenador->ID-1]);
	pthread_mutex_unlock(&mutexTeam);
	//--------------------------------------------------------------------
	pthread_exit(NULL);
}


int calcularDistancia(Entrenador* entrenador, Pokemon* pokemon){

	int posicionX_entrenador = entrenador->posicion.posicionX;
	int posicionY_entrenador = entrenador->posicion.posicionY;
	int posicionX_pokemon = pokemon->posicion.posicionX;
	int posicionY_pokemon = pokemon->posicion.posicionY;

	int movimiento_ejeX = abs(posicionY_entrenador - posicionY_pokemon);
	int movimiento_ejeY = abs(posicionX_entrenador - posicionX_pokemon);

	return movimiento_ejeX + movimiento_ejeY;

}

