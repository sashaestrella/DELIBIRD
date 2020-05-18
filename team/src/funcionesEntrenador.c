#include "funcionesEntrenador.h"




void* planificar(Entrenador* entrenador){
	// Estado estado = 1;
	//uint32_t IDproceso = getpid();
	//uint32_t padre = getppid();
	pid_t tid = syscall(SYS_gettid);
	uint32_t IDentrenador = entrenador->ID;
	int x=entrenador->posicion.posicionX , y=entrenador->posicion.posicionY;

	entrenador->posicion.posicionX = entrenador->posicion.posicionX + 2; // lo muevo dos en x
	return EXIT_SUCCESS;
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

