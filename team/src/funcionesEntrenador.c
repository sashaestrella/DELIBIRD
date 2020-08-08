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

void flujoEntrenador(Entrenador* entrenador){
	Entrenador* entrenadorDeadlock = malloc(sizeof(Entrenador));//sacar
	CoordenadasXY posicion;
	int atrapados=0;
	//ciclos_entrenadores[entrenador->ID -1]=0;
	//contadorCiclosPorEntrenador[entrenador->ID -1] =0;
	Contador* contador = list_get(contadores, entrenador->ID -1 );
	Ciclo* ciclo = list_get(ciclos, entrenador->ID -1);
	sem_wait(ejecutate[entrenador->ID - 1]);
	while(entrenador->estado != EXIT){

	//printf("\nSoy %d y estoy esperando a empezar", entrenador->ID);


	Pokemon* pokemon ;

	bool buscarMiPokemon(Pokemon* pokemon){
		return pokemon->IdEntrenadorQueLoVaAatrapar == entrenador->ID;
	}
	pokemon = list_find(pokemones_en_mapa, (void*)buscarMiPokemon);
	posicion = pokemon->posicion;

	while(noLlegoADestino(entrenador, posicion)){

		moverseUnaPosicion(entrenador, posicion);
		log_info(logger, "Soy el entrenador %d y me movi hasta la posicion (%d, %d)" , entrenador->ID, entrenador->posicion.posicionX, entrenador->posicion.posicionY);
		sleep(retardoCicloCPU);
		ciclos_totales++;
		//ciclos_entrenadores[entrenador->ID -1]++;
		ciclo->ciclo++;
		//contadorCiclosPorEntrenador[entrenador->ID -1]++;
		contador->contador++;


		verificarCiclos(&ciclo->ciclo, entrenador,0);
	}


	log_info(logger, "Soy el entrenador %d y me movi hasta la posicion (%d, %d) de un %s." , entrenador->ID, posicion.posicionX, posicion.posicionY, pokemon->nombre);

	//enviarCatch

	CatchPokemon* catchUnPokemon = malloc(sizeof(CatchPokemon));

	catchUnPokemon->nombre = pokemon->nombre;
	catchUnPokemon->tamanioNombrePokemon = strlen(pokemon->nombre +1);
	catchUnPokemon->coordenadas.posicionX = pokemon->posicion.posicionX;
	catchUnPokemon->coordenadas.posicionY = pokemon->posicion.posicionY;

	CatchPokemonConIDs* catchPokemonConId;

	catchPokemonConId=enviar_catchPokemon(catchUnPokemon);
    if(catchPokemonConId != NULL)
	entrenador->idMensaje = catchPokemonConId->IDmensaje;


	ciclos_totales++;
	//ciclos_entrenadores[entrenador->ID -1]++;
	//contadorCiclosPorEntrenador[entrenador->ID -1]++;
	ciclo->ciclo++;
	contador->contador++;

	verificarCiclos(&ciclo->ciclo, entrenador, 0);



	if(catchPokemonConId == NULL)
	log_info(logger, "Error de conexion con broker. Inicia comportamiento Default");

    if(catchPokemonConId != NULL){
	//entrenador->idMensaje = catchPokemonConId->IDmensaje;
	//printf("\nSoy %d y mande %d", entrenador->ID,entrenador->idMensaje);

	list_remove(ejecutando, 0);
    cambios_contexto++;
	list_add(blocked_caught, entrenador );

	log_info(logger, "Entrenador %d entro a cola de bloqueados (esperando respuesta de CATCH)", entrenador->ID);

	finDeRafaga(&ciclo->ciclo, entrenador);

	sem_post(finEjecucion[entrenador->ID-1]);
//---------------------------------------------------------------------------------------------rafaga

	sem_wait(confirmacion_caught[entrenador->ID-1]);

	puts("\Me llego confirmacion");

	bool miRespuesta(CaughtPokemonConIDs* recibido){

		return recibido->IDCorrelativo == catchPokemonConId->IDmensaje;
	}


	CaughtPokemonConIDs* caughtPokemonConId = list_find(nuevosCaught, (void*)miRespuesta);

	//free(catchPokemonConId->catchPokemon->nombre);
	free(catchPokemonConId->catchPokemon);
	free(catchPokemonConId);

	if(caughtPokemonConId->caughtPokemon->atrapar == 0){
		//puts("\nvoy a poner repuesto");
		agregarPokemonDeRepuesto(pokemon->nombre);
	}

	//asumo que lo agarro por ahora

	bool sacarEntrenador(Entrenador* e){
		return e->ID == entrenador->ID;
	}

	entrenador = list_remove_by_condition(blocked_caught,(void*)sacarEntrenador);


	list_add(ready, entrenador);

	log_info(logger, "Entrenador %d entro a cola ready (llego confirmacion CATCH)", entrenador->ID);

	int valor;
	sem_getvalue(&agregar_ready,&valor);
	if(valor==0 && list_size(ready)==1 && list_size(ejecutando)==0){
		sem_post(&agregar_ready);
		sem_getvalue(&agregar_ready,&valor);
		printf("\nValor de SEMAFORO %d", valor);
	}

	if(list_size(ejecutando)==1 ){
		int valor;
		sem_getvalue(&nuevoReadySJF, &valor);

		if(valor == 0)
		sem_post(&nuevoReadySJF);

		}
	//printf("\nSoy %d pase a ready", entrenador->ID);

	sem_wait(ejecutate[entrenador->ID - 1]);

	if(caughtPokemonConId->caughtPokemon->atrapar == 1 ){
	list_add(entrenador->pokemonesQueTiene, pokemon);
	log_info(logger, "Entrenador %d atrapo a %s en (%d,%d)", entrenador->ID, pokemon->nombre, posicion.posicionX, posicion.posicionY);
	atrapados++;
	}else{
		puts("No pude atrapar");
	}


	//free(catchUnPokemon);
	free(caughtPokemonConId->caughtPokemon);

	//printf("\nSoy %d segundo tramo", entrenador->ID);

    }else{
//----------------------------------------------------------------------------fin caught

    	list_add(entrenador->pokemonesQueTiene, pokemon);
    	log_info(logger, "Entrenador %d atrapo a %s en (%d,%d)", entrenador->ID, pokemon->nombre, posicion.posicionX, posicion.posicionY);
    	atrapados++;



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
			list_add(deadlock,entrenador);
			log_info(logger, "Entrenador %d entro a cola de bloqueados (espera resolucion de deadlock)", entrenador->ID);
			cambios_contexto++;
			finDeRafaga(&ciclo->ciclo, entrenador);
			sem_post(finEjecucion[entrenador->ID - 1]);
		}


			while(!cumplioSusObjetivos(entrenador)){
				//intf("\nSoy %d y estoy en deadlock", entrenador->ID);


				//intf("\nSoy %d y espero resolucion deadlock", entrenador->ID);
							//sem_wait(solucionar_deadlock[entrenador->ID -1]);
							sem_wait(ejecutate[entrenador->ID - 1]);

							if(cumplioSusObjetivos(entrenador)){
								printf("\nEntrenador %d resolvio deadlock", entrenador->ID);
								log_info(logger, "Entrenador %d resolvio deadlock", entrenador->ID);
								i++;
								break;
							}

							printf("\nSoy %d y empece a resolver deadlock", entrenador->ID);
							entrenadorDeadlock = buscarConQuienIntercambiar(entrenador);//aca rompe


							posicion = entrenadorDeadlock->posicion;


							ciclo->ciclo=0;
							while(noLlegoADestino(entrenador, posicion) == true){

								moverseUnaPosicion(entrenador, posicion);
								//puts("\nMe movi");
								//pthread_mutex_lock(&ciclosTotales);
								sleep(retardoCicloCPU);
								ciclos_totales++;
								//ciclos_entrenadores[entrenador->ID -1]++;
								//contadorCiclosPorEntrenador[entrenador->ID -1]++;
								ciclo->ciclo++;
								contador->contador++;

								//pthread_mutex_unlock(&ciclosTotales);

								verificarCiclos(&ciclo->ciclo, entrenador, 1);
							}
							log_info(logger, "Soy el entrenador %d y me movi hasta la posicion (%d, %d) de un entrenador." , entrenador->ID, posicion.posicionX, posicion.posicionY);
							realizarIntercambio(entrenador, entrenadorDeadlock);
							//pthread_mutex_lock(&ciclosTotales);
							ciclos_totales += 5;

							ciclo->ciclo=0;
							for(int j=0;j<5;j++){


							ciclo->ciclo++;
							contador->contador++;

							//pthread_mutex_unlock(&ciclosTotales);
							verificarCiclos(&ciclo->ciclo, entrenador, 1);
							}
							i++;

							if(!cumplioSusObjetivos(entrenador)){
								printf("\nSOY %d",entrenador->ID);
								finDeRafaga(&ciclo->ciclo, entrenador);
								list_add(deadlock,list_remove(ejecutando,0));
								log_info(logger, "Entrenador %d entro a cola de bloqueados (espera resolucion de deadlock)", entrenador->ID);
								cambios_contexto++;
								sem_post(finEjecucion[entrenador->ID -1]);
							}
			}

		entrenador->estado = EXIT;

		printf("\nSoy %d y termine", entrenador->ID);
		log_info(logger, "Entrenador %d entro a cola exit.", entrenador->ID);

		if(i==0){
		list_add(terminados, list_remove(ejecutando,0));
		cambios_contexto++;
		finDeRafaga(&ciclo->ciclo, entrenador);
		sem_post(finEjecucion[entrenador->ID - 1]);
		}else{
		list_add(terminados, list_remove(ejecutando,0));
		cambios_contexto++;
		printf("\nEntrenador %d resolvio deadlock", entrenador->ID);
		log_info(logger, "Entrenador %d resolvio deadlock", entrenador->ID);
		finDeRafaga(&ciclo->ciclo, entrenador);
		sem_post(finEjecucion[entrenador->ID -1]);
		}


	}else{

			list_add(blocked_new, list_remove(ejecutando, 0));
			log_info(logger, "Entrenador %d entro a cola de bloqueados (le falta atrapar pokemones)", entrenador->ID);
			cambios_contexto++;
			finDeRafaga(&ciclo->ciclo, entrenador);
			sem_post(finEjecucion[entrenador->ID - 1]);
			sem_wait(ejecutate[entrenador->ID -1]);

	}


	}

}


void agregarPokemonDeRepuesto(char* nombre){

	bool tengoRepuesto(Pokemon* pokemon){
		return !strcmp(pokemon->nombre,nombre);
	}
	//puts("\nAntes de entrar al if");
	if(list_any_satisfy(mapa_auxiliar, (void*)tengoRepuesto)){

		list_add(pokemones_en_mapa,list_remove_by_condition(mapa_auxiliar, (void*)tengoRepuesto));
	//	puts("\nAgregue repuesto");

	}

}


void finDeRafaga(int* cicloEntrenador, Entrenador* entrenador ){

	if(algoritmoAUtilizar(algoritmoPlanificacion) == SJF_SD || algoritmoAUtilizar(algoritmoPlanificacion) == SJF_CD ){

		entrenador->rafaga = estimarProximaRafaga(entrenador);
		printf("\nrafaga ahora %f", entrenador->rafaga);
		*cicloEntrenador=0;

	}

}

void verificarCiclos(int* cicloEntrenador, Entrenador* entrenador, int enDeadlock ){

	if(algoritmoAUtilizar(algoritmoPlanificacion) == RR){

		if(*cicloEntrenador >= quantum){
			*cicloEntrenador=0;

			if(enDeadlock == 1){
				list_add(readyDeadlock,list_remove(ejecutando,0));
				cambios_contexto++;
			}else{
			list_add(ready,list_remove(ejecutando,0));
			cambios_contexto++;
			}
			puts("\nCorte por quantum");
			sem_post(finEjecucion[entrenador->ID -1]);
			sem_wait(ejecutate[entrenador->ID -1]);
		}
	}

	if (algoritmoAUtilizar(algoritmoPlanificacion) == SJF_CD){


		if(seDesbloqueoEntrenador()){ // aca poner las dos condiciones con ||

			//printf("\nSoy %d y proxima rafaga %f", entrenador->ID,entrenador->rafaga);
			entrenador->rafaga = estimarProximaRafaga(entrenador);
			//puts("\nEntro uno");

			if(hayAlgunoConMenosRafaga(entrenador->rafaga, enDeadlock)){//esto puede volar
				if(enDeadlock == 1){
							list_add(readyDeadlock,list_remove(ejecutando,0));
						}else{
						list_add(ready,list_remove(ejecutando,0));
						}
			puts("\nSali por sjfCD");
			*cicloEntrenador=0;
			sem_post(finEjecucion[entrenador->ID -1]);
			sem_wait(ejecutate[entrenador->ID -1]);

		}

		}

	}
}



bool seDesbloqueoEntrenador(){

	int valor;
	sem_getvalue(&nuevoReadySJF, &valor);
	//printf("\nValor semaforo %d", valor);
	if(valor == 1){
		sem_wait(&nuevoReadySJF);
		return true;
	}else{
		return false;
	}

}

bool hayAlgunoConMenosRafaga(double rafaga, int enDeadlock){

	bool tieneMenorRafaga(Entrenador* entrenadorLista){
		return estimarProximaRafaga(entrenadorLista) < rafaga;
	}

	if(enDeadlock == 1){
	return list_any_satisfy(readyDeadlock, (void*)tieneMenorRafaga);
	}else{
	return list_any_satisfy(ready, (void*)tieneMenorRafaga);
	}
}

void realizarIntercambio(Entrenador* entrenador, Entrenador* entrenadorDeadlock){


	Pokemon* pokemonParaDar ;
	Pokemon* pokemonArecibir;


	bool loNecesito(Pokemon* poke){

		bool esta(char* pokemon){
			return !strcmp(poke->nombre, pokemon);
		}

		return list_any_satisfy(entrenador->objetivos, (void*)esta);
	}

	pokemonArecibir = list_remove_by_condition(entrenadorDeadlock->pokemonesQueTiene, (void*)loNecesito);

	list_add(entrenador->pokemonesQueTiene, pokemonArecibir);


	bool noLoNecesito(Pokemon* poke){

		bool esta(char* pokemon){
			return !strcmp(poke->nombre, pokemon);
		}

		if(list_any_satisfy(entrenador->objetivos, (void*)esta)){
			return false;
		}else{
			return true;
		}
	}


	pokemonParaDar = list_remove_by_condition(entrenador->pokemonesQueTiene, (void*)noLoNecesito);

	list_add(entrenadorDeadlock->pokemonesQueTiene, pokemonParaDar);


	log_info(logger, "Soy el entrenador %d, intercambio con entrenador %d. Le doy a %s y me da a %s." , entrenador->ID, entrenadorDeadlock->ID, pokemonParaDar->nombre, pokemonArecibir->nombre);

}

Entrenador* buscarConQuienIntercambiar(Entrenador* entrenador){
	Entrenador* entre= malloc(sizeof(Entrenador));

	t_list* meFaltan = list_create();



	obtener_los_que_faltan(entrenador->pokemonesQueTiene, entrenador->objetivos, meFaltan);

	/*char* p;
	for(int i=0; i<list_size(meFaltan);i++){
		puts("\nMe faltan");

		p=(char*)list_get(meFaltan,i);
		printf("%s",p);
	}*/

	bool buscarEntrenadorQueTengaElQueNecesito(Entrenador* e){

		bool loNecesito(Pokemon* poke){

			bool esta(char* pokemon){
				return !strcmp(poke->nombre, pokemon);
			}

			return list_any_satisfy(meFaltan, (void*)esta);
		}

		return list_any_satisfy(e->pokemonesQueTiene, (void*)loNecesito);
	}



	entre = (Entrenador*)list_find(deadlock,(void*)buscarEntrenadorQueTengaElQueNecesito);

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
				//printf("\nMeti a %s", pokemon1);
				list_add(meFaltan, pokemon1);
			}

		}

	}



}




bool cumplioSusObjetivos(Entrenador* entrenador){

	t_list* auxiliar = list_create();
	list_add_all(auxiliar, entrenador->objetivos);


	if(list_size(entrenador->pokemonesQueTiene) == list_size(auxiliar)){


		bool sonLosQueNecesita(Pokemon* p){

			bool esta(char* poke){
				return !strcmp(poke, p->nombre);
			}

			if( list_any_satisfy(auxiliar, (void*)esta)){
				list_remove_by_condition(auxiliar, (void*)esta);
				return true;
			}else{
				return false;
			}
		}

		if(list_all_satisfy(entrenador->pokemonesQueTiene, (void*)sonLosQueNecesita)){
			list_destroy(auxiliar);
			return true;
		}

	}else{
		list_destroy(auxiliar);
		return false;;
	}


	//bool destructor(char* e){
	//	return true;
	//}

	//list_destroy_and_destroy_elements(auxiliar, destructor);

	/*if(list_size(entrenador->pokemonesQueTiene) == list_size(entrenador->objetivos)){

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
	}*/

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
	Entrenador* paraProbar ;
	Entrenador* mejorEntrenador ;


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


