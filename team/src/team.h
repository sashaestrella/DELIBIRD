#ifndef TEAM_H_
#define TEAM_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<sys/syscall.h>
#include<src/utilsGeneral.h>

typedef enum
{
	NEW=1,
	READY=2,
	EXEC=3,
	BLOCK=4,
	EXIT=5

}Estado;

typedef struct entrenador{
	uint32_t ID;
	Estado estado;
	CoordenadasXY posicion;
	t_list* objetivos;
}Entrenador;

typedef struct pokemon{
	char* nombre;
	CoordenadasXY posicion;
}Pokemon;

bool execute;
t_list* entrenadores;

int conexionAppeared;
int conexionLocalized;
int conexionCaught;
int conexionGameBoy;
int idAppeared;
int idLocalized;
int idCaught;
int idGameBoy;
char* ip;
char* puerto;

t_log* iniciar_logger(void);
void leer_config(t_list*);
void armar_entrenadores(char**, char**, char** ,t_list*);
int cantidad(char**);
char** obtener_objetivos(char**, char**, t_list*);
void terminar_programa(int, t_log*, t_config*);
void cargarDatosConfig();


#endif
