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

	int atrapados=0;//temporal
	sem_wait(&ejecutate[entrenador->ID - 1]);
	while(entrenador->estado != EXIT){

	printf("\nSoy %d y estoy esperando a empezar", entrenador->ID);


	Pokemon* pokemon = malloc(sizeof(Pokemon));

	bool buscarMiPokemon(Pokemon* pokemon){
		return pokemon->IdEntrenadorQueLoVaAatrapar == entrenador->ID;
	}
	pokemon = list_find(pokemones_en_mapa, (void*)buscarMiPokemon);

	while(noLlegoAlPokemon(entrenador, pokemon) == true){

		moverseUnaPosicion(entrenador, pokemon);

	}

	//enviarCatch

	entrenador = list_remove(ejecutando, 0);

	list_add(blocked_caught, entrenador );

	sem_post(&finEjecucion[entrenador->ID-1]);

	//sem_wait(&confirmacion_caught[entrenador->ID-1]);

	//asumo que lo agarro por ahora

	bool sacarEntrenador(Entrenador* e){
		return e->ID == entrenador->ID;
	}

	entrenador = list_remove_by_condition(blocked_caught,(void*)sacarEntrenador);

	//pthread_mutex_lock(&colaReady);
	list_add(ready, entrenador);
	//pthread_mutex_unlock(&colaReady);

	sem_wait(&ejecutate[entrenador->ID - 1]);
	printf("\nSoy %d segundo tramo", entrenador->ID);
	list_add(entrenador->pokemonesQueTiene, pokemon);
	atrapados++;
	bool buscarPokemon(Pokemon* p){
		return p->nombre == pokemon->nombre && p->IdEntrenadorQueLoVaAatrapar == pokemon->IdEntrenadorQueLoVaAatrapar;
	}

	//pthread_mutex_lock(&mutex_mapa);
	list_remove_by_condition(pokemones_en_mapa, (void*)buscarPokemon);
	//pthread_mutex_unlock(&mutex_mapa);

	entrenador->tieneAsignadoUnPokemon = false;


	if(noPuedeSeguirAtrapando(entrenador->cantidad, atrapados)){// si atrapo todos los que tenia que atrapar

		entrenador->estado = EXIT;
		puts("1");
		list_add(terminados, list_remove(ejecutando,0));
		printf("\nSoy %d y termine", entrenador->ID);
		sem_post(&finEjecucion[entrenador->ID - 1]);
	}else{

		if(list_find(pokemones_en_mapa, (void*)buscarMiPokemon) == NULL){
			puts("\nEntre aca");
			list_add(blocked_new, list_remove(ejecutando, 0));
			sem_post(&finEjecucion[entrenador->ID - 1]);
			sem_wait(&ejecutate[entrenador->ID -1]);
		}else{

				//pthread_mutex_lock(&colaBlocked_new);
				list_add(blocked_new, list_remove(ejecutando, 0));
				//pthread_mutex_lock(&colaBlocked_new);
				sem_post(&finEjecucion[entrenador->ID - 1]);

			}
	}


	}

}

bool noPuedeSeguirAtrapando(int necesita, int losQueYaAtrapo){
	return necesita == losQueYaAtrapo;

}
void planificacionRR(Entrenador* entrenador){

}

void planificacionSJF_CD(Entrenador* entrenador){

}

void planificacionSJF_SD(Entrenador* entrenador){

}

bool noLlegoAlPokemon(Entrenador* entrenador, Pokemon* pokemon){

	return !(entrenador->posicion.posicionX == pokemon->posicion.posicionX &&
		   entrenador->posicion.posicionY == pokemon->posicion.posicionY);

}

void moverseUnaPosicion(Entrenador* entrenador, Pokemon* pokemon){


	if(entrenador->posicion.posicionX > pokemon->posicion.posicionX){
		entrenador->posicion.posicionX--;

	}

	if(entrenador->posicion.posicionX < pokemon->posicion.posicionX){
		entrenador->posicion.posicionX++;
	}

	if(entrenador->posicion.posicionX == pokemon->posicion.posicionX){
		if(entrenador->posicion.posicionY > pokemon->posicion.posicionY){
			entrenador->posicion.posicionY--;

		}else{
			entrenador->posicion.posicionY++;

		}
	}

}

Pokemon* elegirMejorUbicacion(LocalizedPokemonConIDs* localizedPokemon){
	Pokemon* mejor = malloc(sizeof(Pokemon));
	mejor -> nombre = localizedPokemon -> localizedPokemon -> nombre;

	int menorDistancia = 9999999;
	int distancia = 0;

	int cursorEntrenadores = 0;
	int cursorUbicaciones = 0;

	CoordenadasXY mejoresCoordenadas;
	CoordenadasXY* coordenadas = malloc(sizeof(CoordenadasXY));
	Entrenador* paraProbar = malloc(sizeof(Entrenador));

	paraProbar = (Entrenador*)list_get(entrenadores, cursorEntrenadores);
	coordenadas = (CoordenadasXY*)list_get(localizedPokemon -> localizedPokemon -> paresOrdenados, cursorUbicaciones);

	while(coordenadas != NULL){
		mejor -> posicion = *coordenadas;

		while(paraProbar != NULL){
			distancia = calcularDistancia(paraProbar, mejor);

			if(menorDistancia > distancia){
				menorDistancia = distancia;
				mejoresCoordenadas = *coordenadas;
			}

			cursorEntrenadores++;
			paraProbar = (Entrenador*)list_get(entrenadores, cursorEntrenadores);
		}

		cursorEntrenadores = 0;
		paraProbar = (Entrenador*)list_get(entrenadores, cursorEntrenadores);
		cursorUbicaciones++;
		coordenadas = (CoordenadasXY*)list_get(localizedPokemon -> localizedPokemon -> paresOrdenados, cursorUbicaciones);
	}

	mejor -> posicion = mejoresCoordenadas;
	printf("La mejor posicion es ( %d , %d  )\n", mejoresCoordenadas.posicionX, mejoresCoordenadas.posicionY);
	return mejor;
}

int elegirMejorEntrenador(Pokemon* nuevoPokemon){
	Entrenador* paraProbar = malloc(sizeof(Entrenador));
	Entrenador* mejorEntrenador = malloc(sizeof(Entrenador));

	int mejor;

	int menorDistancia=9999;
	int distancia = 0;

	int cursorEntrenadores = 0;
	int cantidadEntrenadores = list_size(blocked_new);
	printf("\nCantidadDeEntrenadores %d", cantidadEntrenadores);
	/*paraProbar = (Entrenador*)list_get(entrenadores, cursorEntrenadores);

	if(paraProbar->tieneAsignadoUnPokemon == false){
	distancia = calcularDistancia(paraProbar, nuevoPokemon);
	menorDistancia = distancia;
	mejorEntrenador = paraProbar;
	}*/
	while(cursorEntrenadores < cantidadEntrenadores){

		paraProbar = (Entrenador*)list_get(blocked_new, cursorEntrenadores);

		if(paraProbar->tieneAsignadoUnPokemon == false){

		distancia = calcularDistancia(paraProbar, nuevoPokemon);


		if(menorDistancia > distancia){
			menorDistancia = distancia;
			mejorEntrenador = paraProbar;
		}

		}
		cursorEntrenadores++;
	}

	//printf("\nEl mejor entrenador es ID %d en la posicion (%d , %d) para el pokemon %s en (%d,%d) \n", mejorEntrenador -> ID, mejorEntrenador -> posicion.posicionX, mejorEntrenador -> posicion.posicionY, nuevoPokemon->nombre, nuevoPokemon->posicion.posicionX, nuevoPokemon->posicion.posicionY);

	return mejorEntrenador -> ID;
}


