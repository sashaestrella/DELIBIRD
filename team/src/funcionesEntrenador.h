#include "team.h"

#ifndef FUNCIONESENTRENADOR_H_
#define FUNCIONESENTRENADOR_H_



void* planificar(Entrenador*);

//void flujoEntrenador (Entrenador*);
void planificacionRR(Entrenador*);
void planificacionSJF_CD(Entrenador*);
void planificacionSJF_SD(Entrenador*);


int elegirMejorEntrenador(Pokemon*);

void moverseUnaPosicion(Entrenador*, CoordenadasXY);
bool noLlegoADestino(Entrenador*, CoordenadasXY );
bool noPuedeSeguirAtrapando(int, int);
bool hayAlgunoConMenosRafaga(double,int);

bool seDesbloqueoEntrenador();
void agregarPokemonDeRepuesto(char* nombre);
void realizarIntercambio(Entrenador*, Entrenador* );
Entrenador* buscarConQuienIntercambiar(Entrenador* entrenador);
void verificarCiclos(int*, Entrenador*,int );
void finDeRafaga(int* , Entrenador*  );

void obtener_los_que_faltan(t_list* , t_list* , t_list* );

#endif /* FUNCIONESENTRENADOR_H_ */
