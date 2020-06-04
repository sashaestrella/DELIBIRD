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

struct {
	uint32_t colaSuscripcion;
	uint32_t tipoSuscriptor;
} datosSuscriptor;

int conexionGameBoy;
int idGameBoy;
int msgGameBoy;

char* ip;
char* puerto;


void noHayBroker();
void generarConexion(int, int, int, int);
void* recibirMensaje(int);
void cargarDatosConexionConfig();
void* adminMensajes(char*);


#endif /* SRC_CONEXIONBROKER_H_ */
