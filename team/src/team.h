#ifndef TEAM_H_
#define TEAM_H_

#include<commons/collections/queue.h>
#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<sys/syscall.h>
#include<src/utilsGeneral.h>

sem_t sem[3];
sem_t sem2[3];


// ----------------- Estructuras ----------------- //

typedef enum
{
	NEW=1,
	READY=2,
	EXEC=3,
	BLOCK=4,
	EXIT=5

}Estado;

typedef enum
{
	FIFO=1,
	RR=2,
	SJF_CD=3,
	SJF_SD=4

}Algoritmos;

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

// ----------------- Colas ----------------- //

pthread_t *hiloEntrenador;

bool execute;
t_list* entrenadores;
t_list* objetivoGlobal;
int tiempoDeReconexion;
char* archivoLog;

t_queue* colaNEW_BLOCKED;
t_list* mensajesGetEnviados;
t_list* mensajesCatchEnviados;

t_list* nuevosCaught;
t_list* mensajesRecibidos;
t_list* nuevosPokemon;

//----------------- Variables para los algoritmos de planificacion ----------------- //

int retardoCicloCPU;
char* algoritmoPlanificacion;
int quantum;
float alpha;
int estimacionInicial;

// ----------------- Conexiones ----------------- //

char* ip;
char* puerto;

int conexionAppeared;
int IDsuscripcionAppeared;
int idAppeared;

int conexionLocalized;
int IDsuscripcionLocalized;
int idLocalized;

int conexionCaught;
int IDsuscripcionCaught;
int idCaught;

int conexionGameBoy;
int idGameBoy;
int msgGameBoy;

// ----------------- Funciones ----------------- //

t_log* iniciar_logger(void);
void leer_config();
void armar_entrenadores(char**, char**, char**);
int cantidad(char**);
char** obtener_objetivos(char**, char**, t_list*);
void terminar_programa(int, t_log*, t_config*);
void obtener_objetivo_global();
void* planificar(Entrenador*);
int obtenerCantidadObjetivo(char*);

#endif
