/*
 * planificacion.h
 *
 *  Created on: 12 jun. 2020
 *      Author: utnso
 */

#ifndef PLANIFICACION_H_
#define PLANIFICACION_H_

#include "team.h"

//_______________Funciones de hilos______________________

void* algoritmoPlanificador();
void* planicadorEntrenador(Entrenador*);


void quitar_de_bloqueados_new(Entrenador* );


void pokemonesParaPrueba();
bool hayEntrenadorSinPokemon();

void entrePorPrimeraVez();

#endif /* PLANIFICACION_H_ */
