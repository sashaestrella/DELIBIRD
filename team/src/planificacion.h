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

void* esperarCaught();

void* entrenadoresReadyLocalizedAppeared();



#endif /* PLANIFICACION_H_ */
