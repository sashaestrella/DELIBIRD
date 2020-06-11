/*
 * Cliente.h
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#ifndef TP0_H_
#define TP0_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include<src/utilsGeneral.h>


// ----------------- Conexiones ----------------- //

int conexionGetPokemon;
int IDsuscripcionGet;
int idGetPokemon;

int conexionNewPokemon;
int IDsuscripcionNew;
int idNewPokemon;

int conexionCatch;
int IDsuscripcionCatch;
int idCatch;

int conexionGameBoy;
int idGameBoy;
int msgGameBoy;

t_log* iniciar_logger(void);
t_config* leer_config(void);
void terminar_programa(int, t_log*, t_config*);

#endif /* TP0_H_ */
