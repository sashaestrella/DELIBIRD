#ifndef SRC_CONEXIONBROKER_H_
#define SRC_CONEXIONBROKER_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<sys/syscall.h>
#include"utilsGeneral.h"

typedef struct parametrosSuscripcion{

	uint32_t conexionCola;
	op_code colaASuscribirse;
	uint32_t nuevoExistente;
	uint32_t IDSuscripcion;
}ParametrosSuscripcion;

int conexionGameBoy;
int idGameBoy;
int msgGameBoy;

char* ip;
char* puerto;


void noHayBroker();
void* suscribirseACola(ParametrosSuscripcion*);

// -------- Funciones Team -------- //
void* recibirMensajesAppeared(int, int);
void* recibirMensajesLocalized(int, int);
void* recibirMensajesCaught(int, int);
void* adminMensajeAppeared(AppearedPokemonConIDs);
void* adminMensajeLocalized(LocalizedPokemonConIDs);
void* adminMensajeCaught(CaugthPokemonConIDs);

// -------- Funciones Game Card -------- //


// ------------------------ //
void enviarACK(int, int, int, int);
void cargarDatosConexionConfig();


#endif /* SRC_CONEXIONBROKER_H_ */
