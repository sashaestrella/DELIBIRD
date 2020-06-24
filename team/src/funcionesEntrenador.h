#include "team.h"

#ifndef FUNCIONESENTRENADOR_H_
#define FUNCIONESENTRENADOR_H_


int calcularDistancia(Entrenador*, Pokemon*);
void* planificar(Entrenador*);

void planificacionFifo (Entrenador*);
void planificacionRR(Entrenador*);
void planificacionSJF_CD(Entrenador*);
void planificacionSJF_SD(Entrenador*);

Pokemon* elegirMejorUbicacion(LocalizedPokemonConIDs*);
int elegirMejorEntrenador(Pokemon*);

void moverseUnaPosicion(Entrenador*, Pokemon*);
bool noLlegoAlPokemon(Entrenador*, Pokemon* );
bool noPuedeSeguirAtrapando(int, int);

#endif /* FUNCIONESENTRENADOR_H_ */
