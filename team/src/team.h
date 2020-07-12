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
#include "adminMensajes.h"



t_log* logger;

sem_t **ejecutate;
sem_t **finEjecucion;
sem_t **confirmacion_caught;

//estos deberian declararse en main porque su tamaño depende de la cantidad de entrenadores


sem_t mensajesCaught;
sem_t nuevosPokemons;
sem_t suscripciones;
sem_t aparicion_pokemon;
sem_t agregar_ready;

pthread_mutex_t colaReady;
pthread_mutex_t colaBlocked_new;
pthread_mutex_t mutex_mapa;
pthread_mutex_t ciclosTotales;

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
	bool tieneAsignadoUnPokemon;
	int cantidad; //temporal para probar fifo
	CoordenadasXY posicion;
	t_list* objetivos;
	t_list* pokemonesQueTiene;
	float rafaga;
	int idMensaje;
}Entrenador;

typedef struct pokemon{
	char* nombre;
	CoordenadasXY posicion;
	int IdEntrenadorQueLoVaAatrapar;
}Pokemon;

// ----------------- Colas ----------------- //

pthread_t *hiloEntrenador;

bool execute;
t_list* entrenadores;
t_list* objetivoGlobal;
int tiempoDeReconexion;
char* archivoLog;


t_list* mensajesGetEnviados;
t_list* mensajesCatchEnviados;

t_list* nuevosCaught;
t_list* mensajesRecibidos;
t_list* nuevosPokemon;

t_list* blocked_new;
t_list* blocked_caught;
t_list* ready;
t_list* ejecutando;
t_list* terminados;
t_list* deadlock;
t_list* pokemones_en_mapa;


t_list* readyAnterior;

//----------------- Variables para los algoritmos de planificacion ----------------- //

int retardoCicloCPU;
char* algoritmoPlanificacion;
int quantum;
float alpha;
int estimacionInicial;

int ciclos_totales;
int *ciclos_entrenadores;
int *contadorCiclosPorEntrenador;

int* valorAnteriorReady;

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
void inicializarSemaforos();
void armar_entrenadores(char**, char**, char**);
int cantidad(char**);
char** obtener_objetivos(char**, char**, t_list*);
void terminar_programa(int, t_log*, t_config*);
void obtener_objetivo_global();
void* flujoEntrenador(Entrenador*);
int obtenerCantidadObjetivo(char*);
bool cumplioSusObjetivos(Entrenador* entrenador);
void reordenarSJF_SD(int);
float estimarProximaRafaga(Entrenador*);

void planificadorFIFO_RR();
void planificadorSJF_SD();
#endif
