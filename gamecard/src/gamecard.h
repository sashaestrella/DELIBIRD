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
#include<commons/bitarray.h>
#include<readline/readline.h>
#include<src/utilsGeneral.h>
#include<semaphore.h>
#include<sys/mman.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <fcntl.h>

char* ip;
char* puerto;
int cantidadArchivos;
char* puntoMontaje;
int tiempoReintento;
int tiempoReconexion;
int tiempoRetardo;
sem_t suscripciones;
t_log* logger;

void leerConfig();

// ----------------- Conexiones ----------------- //

int IDsuscripcionGet;
int idGetPokemon;

int IDsuscripcionNew;
int idNewPokemon;

int IDsuscripcionCatch;
int idCatch;

int conexionGameBoy;
int idGameBoy;
int msgGameBoy;

int cantidadBloques;
int tamanioBloque;

sem_t colaGet;
t_list* mensajesGet;
sem_t colaCatch;
t_list* mensajesCatch;
sem_t colaNew;
t_list* mensajesNew;

t_list* mensajesRecibidos;

t_log* iniciar_logger(void);
t_config* leer_config(void);
void terminar_programa(int, t_log*, t_config*);

#endif /* TP0_H_ */
