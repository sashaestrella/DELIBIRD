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

void moverseUnaPosicion(Entrenador*, CoordenadasXY);
bool noLlegoADestino(Entrenador*, CoordenadasXY );
bool noPuedeSeguirAtrapando(int, int);
void realizarIntercambio(Entrenador*, Entrenador* );
Entrenador* buscarConQuienIntercambiar(Entrenador* entrenador);
bool cumplioSusObjetivos(Entrenador* entrenador);

#endif /* FUNCIONESENTRENADOR_H_ */
