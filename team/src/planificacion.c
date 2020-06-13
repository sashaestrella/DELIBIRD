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
