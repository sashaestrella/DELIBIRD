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
void* recibirMensaje(int);
void cargarDatosConexionConfig();
void* adminMensajes(char*);
void enviarACK(int, int, int);


#endif /* SRC_CONEXIONBROKER_H_ */
