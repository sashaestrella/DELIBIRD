#include "funcionesEntrenador.h"


void* planificar(Entrenador* entrenador){

	switch(algoritmoAUtilizar(algoritmoPlanificacion)){

		case FIFO:
			planificacionFifo (entrenador);
			break;
		case RR:
			planificacionRR(entrenador);
			break;
		case SJF_CD:
			planificacionSJF_CD(entrenador);
			break;
		case SJF_SD:
			planificacionSJF_SD(entrenador);
			break;
	}

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

void planificacionFifo(Entrenador* entrenador){

	printf("\nSoy el entrenador %d y se creo mi hilo.\n", entrenador->ID);

	//-----------------Espera que el hilo main le diga que se puede mover

	printf("Estoy esperando entrenador %d\n",entrenador->ID );

	sem_wait(&sem[entrenador->ID-1]);

	//-------------------------------------------------------------------


	//---------------Le dice al hilo main que ya se movio



	entrenador->posicion.posicionX = entrenador->posicion.posicionX + 1;
	entrenador->posicion.posicionY = entrenador->posicion.posicionY + 1;
	puts("\nSe movio 1 en cada eje");
	sem_post(&sem2[entrenador->ID-1]);
	//--------------------------------------------------------------------
	pthread_exit(NULL);

}

void planificacionRR(Entrenador* entrenador){

}

void planificacionSJF_CD(Entrenador* entrenador){

}

void planificacionSJF_SD(Entrenador* entrenador){

}

