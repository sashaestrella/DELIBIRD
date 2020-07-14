#include "funcionesEntrenador.h"



int calcularDistancia(Entrenador* entrenador, Pokemon* pokemon){

	int posicionX_entrenador = entrenador->posicion.posicionX;
	int posicionY_entrenador = entrenador->posicion.posicionY;
	int posicionX_pokemon = pokemon->posicion.posicionX;
	int posicionY_pokemon = pokemon->posicion.posicionY;

	int movimiento_ejeX = abs(posicionY_entrenador - posicionY_pokemon);
	int movimiento_ejeY = abs(posicionX_entrenador - posicionX_pokemon);

	return movimiento_ejeX + movimiento_ejeY;

}

void* flujoEntrenador(Entrenador* entrenador){
	Entrenador* entrenadorDeadlock = malloc(sizeof(Entrenador));
	CoordenadasXY posicion;
	int atrapados=0;
	ciclos_entrenadores[entrenador->ID -1]=0;
	//int ciclo=0;
	sem_wait(ejecutate[entrenador->ID - 1]);
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

		ciclos_totales++;
		ciclos_entrenadores[entrenador->ID -1]++;

		verificarCiclos(&ciclos_entrenadores[entrenador->ID -1], entrenador);
	}


	log_info(logger, "Soy el entrenador %d y me movi hasta la posicion (%d, %d) de un pokemon." , entrenador->ID, posicion.posicionX, posicion.posicionY);

	//enviarCatch

	CatchPokemon* catchUnPokemon = malloc(sizeof(CatchPokemon));

	catchUnPokemon->nombre = pokemon->nombre;
	catchUnPokemon->tamanioNombrePokemon = strlen(pokemon->nombre +1);
	catchUnPokemon->coordenadas.posicionX = pokemon->posicion.posicionX;
	catchUnPokemon->coordenadas.posicionY = pokemon->posicion.posicionY;

	CatchPokemonConIDs* catchPokemonConId = malloc(sizeof(CatchPokemonConIDs));

	catchPokemonConId=enviar_catchPokemon(catchUnPokemon);

	entrenador->idMensaje = catchPokemonConId->IDmensaje;
	printf("\nSoy %d y mande %d", entrenador->ID,entrenador->idMensaje);
	entrenador = list_remove(ejecutando, 0);

	list_add(blocked_caught, entrenador );

	finDeRafaga(&ciclos_entrenadores[entrenador->ID -1], entrenador);

	sem_post(finEjecucion[entrenador->ID-1]);
//---------------------------------------------------------------------------------------------rafaga

	printf("\nSoy %d y espero confirmacion", entrenador->ID);

	sem_wait(confirmacion_caught[entrenador->ID-1]);

	printf("\nSoy %d me llego la confirmacion", entrenador->ID);


	//buscar en los recibidos el id del que mande


	bool miRespuesta(CaughtPokemonConIDs* recibido){

		return recibido->IDCorrelativo == catchPokemonConId->IDmensaje;
	}


	CaughtPokemonConIDs* caughtPokemonConId = malloc(sizeof(CaughtPokemonConIDs));

	caughtPokemonConId = list_find(nuevosCaught, miRespuesta);



	//asumo que lo agarro por ahora

	bool sacarEntrenador(Entrenador* e){
		return e->ID == entrenador->ID;
	}

	entrenador = list_remove_by_condition(blocked_caught,(void*)sacarEntrenador);


	list_add(ready, entrenador);
	sem_post(&agregar_ready);

	printf("\nSoy %d pase a ready", entrenador->ID);

	sem_wait(ejecutate[entrenador->ID - 1]);

	printf("\nSoy %d segundo tramo", entrenador->ID);

	if(caughtPokemonConId->caughtPokemon->atrapar == 1){
	list_add(entrenador->pokemonesQueTiene, pokemon);
	printf("\nSoy %d y atrape a %s", entrenador->ID, pokemon->nombre);
	atrapados++;
	}else{
		puts("No pude atrapar, se me escapo");
	}
	bool buscarPokemon(Pokemon* p){
		return p->nombre == pokemon->nombre && p->IdEntrenadorQueLoVaAatrapar == pokemon->IdEntrenadorQueLoVaAatrapar;
	}


	list_remove_by_condition(pokemones_en_mapa, (void*)buscarPokemon);


	entrenador->tieneAsignadoUnPokemon = false;


	if(noPuedeSeguirAtrapando(entrenador->cantidad, atrapados)){// si atrapo todos los que tenia que atrapar
		int i=0;
		if(!cumplioSusObjetivos(entrenador)){
			list_remove(ejecutando,0);
			sem_post(finEjecucion[entrenador->ID - 1]);
		}

			while(!cumplioSusObjetivos(entrenador)){
				//intf("\nSoy %d y estoy en deadlock", entrenador->ID);
				list_add(deadlock,entrenador);

				//intf("\nSoy %d y espero resolucion deadlock", entrenador->ID);
							//sem_wait(solucionar_deadlock[entrenador->ID -1]);
							sem_wait(ejecutate[entrenador->ID - 1]);
							if(cumplioSusObjetivos(entrenador)){
								i++;
								break;
							}
							//intf("\nSoy %d y empece a resolver deadlock", entrenador->ID);
							entrenadorDeadlock = buscarConQuienIntercambiar(entrenador);//aca rompe

							posicion = entrenadorDeadlock->posicion;
							ciclos_entrenadores[entrenador->ID -1]=0;
							while(noLlegoADestino(entrenador, posicion) == true){

								moverseUnaPosicion(entrenador, posicion);
								puts("\nMe movi");
								//pthread_mutex_lock(&ciclosTotales);
								ciclos_totales++;
								ciclos_entrenadores[entrenador->ID -1]++;

								//pthread_mutex_unlock(&ciclosTotales);
								verificarCiclos(&ciclos_entrenadores[entrenador->ID -1], entrenador);
							}
							log_info(logger, "Soy el entrenador %d y me movi hasta la posicion (%d, %d) de un entrenador." , entrenador->ID, posicion.posicionX, posicion.posicionY);
							realizarIntercambio(entrenador, entrenadorDeadlock);
							//pthread_mutex_lock(&ciclosTotales);
							ciclos_totales += 5;
							ciclos_entrenadores[entrenador->ID -1]=0;
							for(int i=0;i<5;i++){
							ciclos_entrenadores[entrenador->ID -1]++;

							//pthread_mutex_unlock(&ciclosTotales);
							verificarCiclos(&ciclos_entrenadores[entrenador->ID -1], entrenador);
							}
							i++;

							if(!cumplioSusObjetivos(entrenador)){
								finDeRafaga(&ciclos_entrenadores[entrenador->ID -1], entrenador);
								sem_post(finEjecucion[entrenador->ID -1]);
							}
			}

		entrenador->estado = EXIT;
		list_add(terminados, list_remove(ejecutando,0));
		printf("\nSoy %d y termine", entrenador->ID);

		if(i==0){
		finDeRafaga(&ciclos_entrenadores[entrenador->ID -1], entrenador);
		sem_post(finEjecucion[entrenador->ID - 1]);
		}else{
		list_remove(ejecutando,0);
		finDeRafaga(&ciclos_entrenadores[entrenador->ID -1], entrenador);
		sem_post(finEjecucion[entrenador->ID -1]);
		}


	}else{

			list_add(blocked_new, list_remove(ejecutando, 0));
			sem_post(finEjecucion[entrenador->ID - 1]);
			sem_wait(ejecutate[entrenador->ID -1]);

	}


	}

}

void finDeRafaga(int* cicloEntrenador, Entrenador* entrenador ){
	if(algoritmoAUtilizar(algoritmoPlanificacion) == SJF_CD){

		entrenador->rafaga = estimarProximaRafaga(entrenador);
		*cicloEntrenador=0;

	}

}

void verificarCiclos(int* cicloEntrenador, Entrenador* entrenador ){

	if(algoritmoAUtilizar(algoritmoPlanificacion) == RR){

		if(*cicloEntrenador >= quantum){
			*cicloEntrenador=0;

			list_add(ready,list_remove(ejecutando,0));
			puts("\nCorte por quantum");
			sem_post(finEjecucion[entrenador->ID -1]);
			sem_wait(ejecutate[entrenador->ID -1]);
		}
	}

	if (algoritmoAUtilizar(algoritmoPlanificacion) == SJF_CD){


		if(seDesbloqueoEntrenador()){ // aca poner las dos condiciones con ||

			printf("\nSoy %d y proxima rafaga %f", entrenador->ID,entrenador->rafaga);
			entrenador->rafaga = estimarProximaRafaga(entrenador);
			puts("\nEntro uno");
		if(hayAlgunoEnReadyConMenosRafaga(entrenador->rafaga)){//este puede volar

			list_add(ready,list_remove(ejecutando,0));
			puts("\nSali por sjfCD");
			*cicloEntrenador=0;
			sem_post(finEjecucion[entrenador->ID -1]);
			sem_wait(ejecutate[entrenador->ID -1]);

		}

		}

	}
}

bool entroNuevoAReady(){ //esta faltaria probar, debrias er cada vez que entre un entrenador nuevo al sistema.

	Entrenador* entrenador1 = malloc(sizeof(Entrenador));
	Entrenador* entrenador2 = malloc(sizeof(Entrenador));
    int comp;
    printf("\nTamaño anterior %d", list_size(readyAnterior));
    printf("\nTamaño actual %d", list_size(ready));

		for( int i=0; i<list_size(ready); i++){
			entrenador1 = list_get(ready, i);
			printf("\n %d))))", entrenador1->ID);
			for(int j=0; j < list_size(readyAnterior);j++){

				entrenador2=list_get(readyAnterior,j);
				printf("\n %d)", entrenador2->ID);
				if(entrenador1->ID == entrenador2->ID){
					comp=0;
					printf("\ncomp %d", comp);
					break;
				}else{
					comp=1;
					printf("\ncomp %d", comp);
				}

			}

		}

		//if(comp){
		list_clean(readyAnterior);
		list_add_all(readyAnterior, ready);
		//}


		return comp;

}

bool seDesbloqueoEntrenador(){//esta verifica que se haya llegago desde entrada salida a ready.

	int tamanioAnterior = list_size(readyAnterior);

	int tamanioActual=list_size(ready);

	if(tamanioAnterior < tamanioActual){
		tamanioAnterior = tamanioActual;
		return true;
	}else{
		return false;
	}

}

bool hayAlgunoEnReadyConMenosRafaga(float rafaga){

	bool tieneMenorRafaga(Entrenador* entrenadorLista){
		return estimarProximaRafaga(entrenadorLista) < rafaga;
	}

	return list_any_satisfy(ready, tieneMenorRafaga);
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

	log_info(logger, "Soy el entrenador %d, intercambio con entrenador %d. Le doy a %s y me da a %s." , entrenador->ID, entrenadorDeadlock->ID, pokemonParaDar->nombre, pokemonArecibir->nombre);

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

	entre = list_find(deadlock,(void*)buscarEntrenadorQueTengaElQueNecesito);
	log_info(logger, "Se detecto deadlock entre entrenador %d y %d", entrenador->ID, entre->ID);
	return  entre;
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

bool noLlegoADestino(Entrenador* entrenador, CoordenadasXY posicion){

	return !(entrenador->posicion.posicionX ==posicion.posicionX &&
		   entrenador->posicion.posicionY == posicion.posicionY);

}

void moverseUnaPosicion(Entrenador* entrenador, CoordenadasXY posicion){


	if(entrenador->posicion.posicionX == posicion.posicionX){
		if(entrenador->posicion.posicionY > posicion.posicionY){
			entrenador->posicion.posicionY--;

		}else{
			entrenador->posicion.posicionY++;

		}
	}

	if(entrenador->posicion.posicionX > posicion.posicionX){
		entrenador->posicion.posicionX--;

	}

	if(entrenador->posicion.posicionX < posicion.posicionX){
		entrenador->posicion.posicionX++;
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


