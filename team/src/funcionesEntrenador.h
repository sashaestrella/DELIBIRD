#include "team.h"

#ifndef FUNCIONESENTRENADOR_H_
#define FUNCIONESENTRENADOR_H_


int calcularDistancia(Entrenador*, Pokemon*);
void* planificar(Entrenador*);

void* flujoEntrenador (Entrenador*);
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
void verificarCiclos(int*, int );

void obtener_los_que_faltan(t_list* , t_list* , t_list* );

#endif /* FUNCIONESENTRENADOR_H_ */
