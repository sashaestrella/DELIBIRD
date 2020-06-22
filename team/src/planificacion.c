#include "planificacion.h"

void* esperarCaught(CaughtPokemonConIDs* unCaught){
	CatchPokemonConIDs* catchEnviado;

	while(1){
		sem_wait(&mensajesCaught);

		bool encontrarMensajeEnviado(CatchPokemonConIDs* mensajeCatch){
			return mensajeCatch->IDmensaje == unCaught -> IDCorrelativo;
		}
		catchEnviado = (CatchPokemonConIDs*)list_find(mensajesCatchEnviados, (void*)encontrarMensajeEnviado);

		//Enviar señal al entrenador con la respuesta
	}
	free(catchEnviado);
}


void* entrenadoresReadyLocalizedAppeared(){
	Pokemon* nuevoPokemon;
	nuevoPokemon = malloc(sizeof(Pokemon));
	int id;

	while(1){
		sem_wait(&nuevosPokemons);

		nuevoPokemon = (Pokemon*)list_get(nuevosPokemon, 0);
		list_remove(nuevosPokemon, 0);

		id = elegirMejorEntrenador(nuevoPokemon);

		//Enviar señal al entrenador con el Pokemon

	}
	free(nuevoPokemon);
}

void pokemonesParaPrueba(){

	Pokemon* pikachu1 = malloc(sizeof(Pokemon));
	pikachu1->nombre = "Pikachu";
	pikachu1->posicion.posicionX = 9;
	pikachu1->posicion.posicionY = 2;

	Pokemon* pikachu2 = malloc(sizeof(Pokemon));
	pikachu2->nombre = "Pikachu";
	pikachu2->posicion.posicionX = 4;
	pikachu2->posicion.posicionY = 5;

	Pokemon* charmander = malloc(sizeof(Pokemon));
	charmander->nombre = "Charmander";
	charmander->posicion.posicionX = 1;
	charmander->posicion.posicionY = 8;

	Pokemon* squirtle = malloc(sizeof(Pokemon));
	squirtle->nombre = "Squirtle";
	squirtle->posicion.posicionX = 6;
	squirtle->posicion.posicionY = 2;

	list_add(pokemones_en_mapa, pikachu1);
	list_add(pokemones_en_mapa, pikachu2);
	list_add(pokemones_en_mapa, charmander);
	list_add(pokemones_en_mapa, squirtle);
}

void pasar_a_ready_por_cercania(){//se ejecutaria al comienzo y cuando aparecen pokemones durante la ejecucion

	Pokemon* pokemon = malloc(sizeof(Pokemon));
	Entrenador* entrenador = malloc(sizeof(Entrenador));//ver si esto va aca o en el for
	list_add_all(blocked_new, entrenadores);
	int idEntrenador;
	for(int i = 0; list_size(pokemones_en_mapa); i++){

		pokemon = list_get(pokemones_en_mapa, i);

		idEntrenador = elegirMejorEntrenador(pokemon);

		pokemon->IdEntrenadorQueLoVaAatrapar = idEntrenador;

		bool buscarEntrenador(Entrenador* entrenador){
			return entrenador->ID == idEntrenador;
		}

		entrenador = list_find(blocked_new, (void*)buscarEntrenador);

		list_add(ready, entrenador);
	}

}
