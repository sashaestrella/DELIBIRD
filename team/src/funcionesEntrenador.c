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
	Entrenador* entrenadorDeadlock = malloc(sizeof(Entrenador));
	CoordenadasXY posicion;
	int atrapados=0;//temporal
	sem_wait(&ejecutate[entrenador->ID - 1]);
	while(entrenador->estado != EXIT){

	printf("\nSoy %d y estoy esperando a empezar", entrenador->ID);


	Pokemon* pokemon = malloc(sizeof(Pokemon));

	bool buscarMiPokemon(Pokemon* pokemon){
		return pokemon->IdEntrenadorQueLoVaAatrapar == entrenador->ID;
	}
	pokemon = list_find(pokemones_en_mapa, (void*)buscarMiPokemon);
	posicion = pokemon->posicion;
	while(noLlegoADestino(entrenador, posicion)){

		moverseUnaPosicion(entrenador, posicion);

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
	printf("\nSoy %d y atrape a %s", entrenador->ID, pokemon->nombre);
	atrapados++;
	bool buscarPokemon(Pokemon* p){
		return p->nombre == pokemon->nombre && p->IdEntrenadorQueLoVaAatrapar == pokemon->IdEntrenadorQueLoVaAatrapar;
	}

	//pthread_mutex_lock(&mutex_mapa);
	list_remove_by_condition(pokemones_en_mapa, (void*)buscarPokemon);
	//pthread_mutex_unlock(&mutex_mapa);

	entrenador->tieneAsignadoUnPokemon = false;


	if(noPuedeSeguirAtrapando(entrenador->cantidad, atrapados)){// si atrapo todos los que tenia que atrapar
		int i=0;
		if(!cumplioSusObjetivos(entrenador)){
			list_remove(ejecutando,0);
			sem_post(&finEjecucion[entrenador->ID - 1]);
		}

			while(!cumplioSusObjetivos(entrenador)){
				//intf("\nSoy %d y estoy en deadlock", entrenador->ID);
				list_add(deadlock,entrenador);

				//intf("\nSoy %d y espero resolucion deadlock", entrenador->ID);
							sem_wait(&solucionar_deadlock[entrenador->ID -1]);

							if(cumplioSusObjetivos(entrenador)){
								i++;
								break;
							}
							//intf("\nSoy %d y empece a resolver deadlock", entrenador->ID);
							entrenadorDeadlock = buscarConQuienIntercambiar(entrenador);//aca rompe
							printf("\nEncontre a %d para intercambiar", entrenadorDeadlock->ID);
							posicion = entrenadorDeadlock->posicion;

							while(noLlegoADestino(entrenador, posicion) == true){

								moverseUnaPosicion(entrenador, posicion);
								puts("\nMe movi");
							}

							realizarIntercambio(entrenador, entrenadorDeadlock);
							puts("\nIntercambie, ahora tengo:");

							for(int i=0; i<list_size(entrenador->pokemonesQueTiene);i++){
								pokemon=list_get(entrenador->pokemonesQueTiene,i);
								printf("\n %s", pokemon->nombre);
							}

							i++;

							if(!cumplioSusObjetivos(entrenador)){
								sem_post(&solucione_deadlock[entrenador->ID -1]);
							}
			}

		entrenador->estado = EXIT;
		list_add(terminados, list_remove(ejecutando,0));
		printf("\nSoy %d y termine", entrenador->ID);
		if(i==0){
		sem_post(&finEjecucion[entrenador->ID - 1]);
		}else{
		list_remove(ejecutando,0);
		sem_post(&solucione_deadlock[entrenador->ID -1]);
		}






	}else{

		if(list_find(pokemones_en_mapa, (void*)buscarMiPokemon) == NULL){
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


void realizarIntercambio(Entrenador* entrenador, Entrenador* entrenadorDeadlock){

	char* pokemon1;
		Pokemon* pokemon2 = malloc(sizeof(Pokemon));
	Pokemon* pokemonParaDar = malloc(sizeof(Pokemon));
	Pokemon* pokemonArecibir = malloc(sizeof(Pokemon));
	bool resultado;
	for(int i=0; i<list_size(entrenador->objetivos);i++){
				pokemon1 = list_get(entrenador->objetivos,i);
				for(int j=0; j<list_size(entrenadorDeadlock->pokemonesQueTiene);j++){
					pokemon2 = list_get(entrenadorDeadlock->pokemonesQueTiene,j);
					resultado = strcmp(pokemon1 ,pokemon2->nombre);
					if(resultado==0){
					pokemonArecibir = pokemon2;
					break;
					}
				}
					if(resultado==0)
					break;
			}


	for(int i=0; i<list_size(entrenador->pokemonesQueTiene);i++){
				pokemon2 = list_get(entrenador->pokemonesQueTiene,i);
				for(int j=0; j<list_size(entrenador->objetivos);j++){
					pokemon1 = list_get(entrenador->objetivos,j);
					resultado = strcmp(pokemon1, pokemon2->nombre);
					if(resultado==0){
					break;
					}else{
					pokemonParaDar = pokemon2;
					}
				}
					if(resultado!=0)
					break;
			}

	list_add(entrenadorDeadlock->pokemonesQueTiene, pokemonParaDar);

	bool sacarElQueDoy(Pokemon* pokemon){
		return !strcmp(pokemon->nombre, pokemonParaDar->nombre);

	}
	list_remove_by_condition(entrenador->pokemonesQueTiene, (void*)sacarElQueDoy);



	bool sacarElQueRecibo(Pokemon* pokemon){
		return !strcmp(pokemon->nombre,pokemonArecibir->nombre);
	}
	list_remove_by_condition(entrenadorDeadlock->pokemonesQueTiene, (void*)sacarElQueRecibo);

	list_add(entrenador->pokemonesQueTiene, pokemonArecibir);


}

Entrenador* buscarConQuienIntercambiar(Entrenador* entrenador){
	Entrenador* entre= malloc(sizeof(Entrenador));
	char* pokemon1;
	Pokemon* pokemon2 = malloc(sizeof(Pokemon));
	bool resultado;

	t_list* meFaltan = list_create();

	obtener_los_que_faltan(entrenador->pokemonesQueTiene, entrenador->objetivos, meFaltan);
	char*p;
	for(int i=0; i<list_size(meFaltan);i++){
		puts("\nMe faltan");
		p=list_get(meFaltan,i);
		printf("%s",p);
	}



	bool buscarEntrenadorQueTengaElQueNecesito(Entrenador* e){

		for(int i=0; i<list_size(meFaltan);i++){

			pokemon1 = list_get(meFaltan,i);

			for(int j=0; j<list_size(e->pokemonesQueTiene);j++){
				pokemon2 = list_get(e->pokemonesQueTiene,j);
				resultado = strcmp(pokemon1 , pokemon2->nombre);
				if(resultado == 0){
					break;}
			}
			if(resultado == 0){
				break;}
		}

		return !resultado;
	}

	return  list_find(deadlock,(void*)buscarEntrenadorQueTengaElQueNecesito);
}

void obtener_los_que_faltan(t_list* yaTiene, t_list* objetivos, t_list* meFaltan){

	t_list* temporal = list_create();

	list_add_all(temporal, yaTiene);


	char* pokemon1;
	Pokemon* pokemon2 = malloc(sizeof(Pokemon));


	for (int i =0; i<list_size(objetivos);i++){
		pokemon1 = list_get(objetivos, i);

		for(int j=0; j<list_size(temporal);j++){
			pokemon2 = list_get(temporal,j);

			if(strcmp(pokemon1, pokemon2->nombre) ==0){
				list_remove(temporal,j);
				break;
			}else{
				list_add(meFaltan, pokemon1);
			}

		}

	}



}




bool cumplioSusObjetivos(Entrenador* entrenador){
	Pokemon* pokemon1 = malloc(sizeof(Pokemon));
	char* pokemon2 ;
	int contador=0;
	bool resultado;

	if(list_size(entrenador->pokemonesQueTiene) == list_size(entrenador->objetivos)){

		for(int i=0; i<list_size(entrenador->pokemonesQueTiene);i++){
					pokemon1 = list_get(entrenador->pokemonesQueTiene,i);
					for(int j=0; j<list_size(entrenador->objetivos);j++){
						pokemon2 = list_get(entrenador->objetivos,j);
						resultado = strcmp(pokemon1->nombre,pokemon2);
						if(resultado == 0){
						break;
						}
					}
					if(resultado == 0)
					contador++;
				}

	}else{
		return 0;
	}
		return contador == list_size(entrenador->objetivos);
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

bool noLlegoADestino(Entrenador* entrenador, CoordenadasXY posicion){

	return !(entrenador->posicion.posicionX ==posicion.posicionX &&
		   entrenador->posicion.posicionY == posicion.posicionY);

}

void moverseUnaPosicion(Entrenador* entrenador, CoordenadasXY posicion){


	if(entrenador->posicion.posicionX > posicion.posicionX){
		entrenador->posicion.posicionX--;

	}

	if(entrenador->posicion.posicionX < posicion.posicionX){
		entrenador->posicion.posicionX++;
	}

	if(entrenador->posicion.posicionX == posicion.posicionX){
		if(entrenador->posicion.posicionY > posicion.posicionY){
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


