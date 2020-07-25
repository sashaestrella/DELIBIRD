#include "planificacion.h"



void pokemonesParaPrueba(){

	Pokemon* pikachu1 = malloc(sizeof(Pokemon));
	pikachu1->nombre = "Pikachu";
	pikachu1->posicion.posicionX = 3;
	pikachu1->posicion.posicionY = 8;
	pikachu1->IdEntrenadorQueLoVaAatrapar=0;

	Pokemon* pikachu2 = malloc(sizeof(Pokemon));
	pikachu2->nombre = "Pikachu";
	pikachu2->posicion.posicionX = 3;
	pikachu2->posicion.posicionY = 9;
	pikachu2->IdEntrenadorQueLoVaAatrapar=0;

	Pokemon* charmander = malloc(sizeof(Pokemon));
	charmander->nombre = "Charmander";
	charmander->posicion.posicionX = 6;
	charmander->posicion.posicionY = 5;
	charmander->IdEntrenadorQueLoVaAatrapar=0;

	Pokemon* squirtle = malloc(sizeof(Pokemon));
	squirtle->nombre = "Squirtle";
	squirtle->posicion.posicionX = 2;
	squirtle->posicion.posicionY = 2;
	squirtle->IdEntrenadorQueLoVaAatrapar=0;

	Pokemon* squirtle2 = malloc(sizeof(Pokemon));
		squirtle2->nombre = "Squirtle";
		squirtle2->posicion.posicionX = 3;
		squirtle2->posicion.posicionY = 5;
		squirtle2->IdEntrenadorQueLoVaAatrapar=0;

	Pokemon* onix = malloc(sizeof(Pokemon));
		onix->nombre = "Onix";
		onix->posicion.posicionX = 2;
		onix->posicion.posicionY = 2;
		onix->IdEntrenadorQueLoVaAatrapar=0;

	Pokemon* gengar = malloc(sizeof(Pokemon));
	gengar->nombre = "Gengar";
	gengar->posicion.posicionX = 7;
	gengar->posicion.posicionY = 5;
	gengar->IdEntrenadorQueLoVaAatrapar=0;
	list_add(pokemones_en_mapa, pikachu1);
	list_add(pokemones_en_mapa, pikachu2);
	list_add(pokemones_en_mapa, charmander);
	list_add(pokemones_en_mapa, squirtle);
	//list_add(pokemones_en_mapa, squirtle2);
	//list_add(pokemones_en_mapa, onix);
	//list_add(pokemones_en_mapa, gengar);
}


int pasar_a_ready_por_cercania(){

	Pokemon* pokemon;
	Entrenador* entrenador;


	int idEntrenador, distancia, menorDistancia;

		Entrenador* entrenadorAready ;
		Pokemon* pokemonAasignar ;

int contador=0;
	while( contador < list_size(pokemones_en_mapa) ){

		if(hayEntrenadorSinPokemon()){

menorDistancia = 88888;

for(int i = 0; i<list_size(pokemones_en_mapa); i++){

		pokemon = list_get(pokemones_en_mapa, i);

		if(pokemon->IdEntrenadorQueLoVaAatrapar ==0){

				idEntrenador = elegirMejorEntrenador(pokemon);

				bool buscarEntrenador(Entrenador* entrenador){
					return entrenador->ID == idEntrenador;
				}

				entrenador = list_find(blocked_new, (void*)buscarEntrenador);

				distancia= calcularDistancia(entrenador, pokemon);

		if(distancia < menorDistancia){
			pokemonAasignar = pokemon;
			entrenadorAready = entrenador;
			menorDistancia = distancia;
		}
		}
		if(i == list_size(pokemones_en_mapa)-1){
			bool buscar(Entrenador* e){
				return e->ID == entrenador->ID;
			}
			if(list_any_satisfy(blocked_new,(void*)buscar)){
			entrenadorAready->tieneAsignadoUnPokemon = true;
			pokemonAasignar->IdEntrenadorQueLoVaAatrapar = entrenadorAready->ID;
			quitar_de_bloqueados_new(entrenadorAready);

			entrePorPrimeraVez(entrenadorAready);
			list_add(ready, entrenadorAready);
			return 1;
			}
			//sem_post(&agregar_ready);
		}

	}

		}

		contador++;

	}
	return 0;
}

void entrePorPrimeraVez(Entrenador* entrenador){
	if(algoritmoAUtilizar(algoritmoPlanificacion) == SJF_CD){

		bool esta(int id){
			return id == entrenador->ID;
		}

		if(!list_any_satisfy(entraronPorPrimeraVez, (void*)esta)){

			entrenador->rafaga = estimarProximaRafaga(entrenador);
			printf("\nAgregue a %d", entrenador->ID);
			list_add(entraronPorPrimeraVez, entrenador->ID);
		}

	}
}


void quitar_de_bloqueados_new(Entrenador* entrenador){
	bool buscarEntrenador(Entrenador* e){
		return e->ID == entrenador->ID;
	}
	list_remove_by_condition(blocked_new, (void*)buscarEntrenador);
}

bool hayEntrenadorSinPokemon(){

	bool noTienePokemon(Entrenador* entrenador){
		return !entrenador->tieneAsignadoUnPokemon;
	}

	return list_any_satisfy(blocked_new, (void*)noTienePokemon);
}
