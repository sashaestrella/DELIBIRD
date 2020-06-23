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
	pikachu1->IdEntrenadorQueLoVaAatrapar=0;

	Pokemon* pikachu2 = malloc(sizeof(Pokemon));
	pikachu2->nombre = "Pikachu";
	pikachu2->posicion.posicionX = 4;
	pikachu2->posicion.posicionY = 5;
	pikachu2->IdEntrenadorQueLoVaAatrapar=0;

	Pokemon* charmander = malloc(sizeof(Pokemon));
	charmander->nombre = "Charmander";
	charmander->posicion.posicionX = 1;
	charmander->posicion.posicionY = 8;
	charmander->IdEntrenadorQueLoVaAatrapar=0;

	Pokemon* squirtle = malloc(sizeof(Pokemon));
	squirtle->nombre = "Squirtle";
	squirtle->posicion.posicionX = 6;
	squirtle->posicion.posicionY = 2;
	squirtle->IdEntrenadorQueLoVaAatrapar=0;

	list_add(pokemones_en_mapa, pikachu1);
	list_add(pokemones_en_mapa, pikachu2);
	list_add(pokemones_en_mapa, charmander);
	list_add(pokemones_en_mapa, squirtle);
	//casos de prueba
	/*Pokemon* pikachu1 = malloc(sizeof(Pokemon));
	pikachu1->nombre = "Pikachu";
	pikachu1->posicion.posicionX = 12;
	pikachu1->posicion.posicionY = 10;
	pikachu1->IdEntrenadorQueLoVaAatrapar=0;

	Pokemon* bulbasaur = malloc(sizeof(Pokemon));
	bulbasaur->nombre = "Bulbasaur";
	bulbasaur->posicion.posicionX = 15;
	bulbasaur->posicion.posicionY = 3;
	bulbasaur->IdEntrenadorQueLoVaAatrapar=0;

	Pokemon* ratata = malloc(sizeof(Pokemon));
	ratata->nombre = "Ratata";
	ratata->posicion.posicionX = 7;
	ratata->posicion.posicionY = 5;
	ratata->IdEntrenadorQueLoVaAatrapar=0;

	Pokemon* charmander1 = malloc(sizeof(Pokemon));
	charmander1->nombre = "Charmander1";
	charmander1->posicion.posicionX = 4;
	charmander1->posicion.posicionY = 7;
	charmander1->IdEntrenadorQueLoVaAatrapar=0;

	Pokemon* charmander2 = malloc(sizeof(Pokemon));
	charmander2->nombre = "Charmander2";
	charmander2->posicion.posicionX = 12;
	charmander2->posicion.posicionY = 7;
	charmander2->IdEntrenadorQueLoVaAatrapar=0;
	list_add(pokemones_en_mapa, bulbasaur);
	list_add(pokemones_en_mapa, pikachu1);
	list_add(pokemones_en_mapa, ratata);
	list_add(pokemones_en_mapa, charmander1);*/

}


void pasar_a_ready_por_cercania(){//se ejecutaria al comienzo y cuando aparecen pokemones durante la ejecucion

	Pokemon* pokemon = malloc(sizeof(Pokemon));
	Entrenador* entrenador = malloc(sizeof(Entrenador));//ver si esto va aca o en el for

	list_add_all(blocked_new, entrenadores);

	int idEntrenador, distancia, menorDistancia;

		Entrenador* entrenadorAready = malloc(sizeof(Entrenador));
		Pokemon* pokemonAasignar = malloc(sizeof(Pokemon));

int contador=0;
	while( contador < list_size(pokemones_en_mapa)){

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
			entrenadorAready->tieneAsignadoUnPokemon = true;
			pokemonAasignar->IdEntrenadorQueLoVaAatrapar = entrenadorAready->ID;
			list_add(ready, entrenadorAready);
		}

	}

		}
		contador++;
	}

}

bool hayEntrenadorSinPokemon(){

	bool noTienePokemon(Entrenador* entrenador){
		return !entrenador->tieneAsignadoUnPokemon;
	}

	return list_any_satisfy(blocked_new, (void*)noTienePokemon);
}
