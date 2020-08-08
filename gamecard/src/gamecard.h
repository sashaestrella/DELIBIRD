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
char* miPUERTO;
char* miIP;
char* rutaLog;
char* magicNumber;
int cantidadArchivos;
char* puntoMontaje;
int tiempoReintento;
int tiempoReconexion;
int tiempoRetardo;
sem_t suscripciones;
sem_t mensajeABroker;
t_log* logger;
t_config* config;

void leerConfig();

// ----------------- Conexiones ----------------- //

int IDsuscripcionGet;

int IDsuscripcionNew;

int IDsuscripcionCatch;

int cantidadBloques;
int tamanioBloque;

sem_t reintentoNew;
sem_t reintentoGet;
sem_t reintentoCatch;

t_log* iniciar_logger(void);
t_config* leer_config(void);
void terminar_programa(int, t_log*, t_config*);

#endif /* TP0_H_ */
