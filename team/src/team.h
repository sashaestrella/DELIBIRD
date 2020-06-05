#ifndef TEAM_H_
#define TEAM_H_

#include<src/conexionBroker.h>
#include<semaphore.h>

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

pthread_t *hiloEntrenador;


bool execute;
t_list* entrenadores;
t_list* objetivoGlobal;
int tiempoDeReconexion;
char* archivoLog;

//------------Variables para los algoritmos de planificacion

int retardoCicloCPU;
char* algoritmoPlanificacion;
int quantum;
float alpha;
int estimacionInicial;
//----------------------------------------------------------

int conexionAppeared;
int IDsuscripcionAppeared;
int idAppeared;

int conexionLocalized;
int IDsuscripcionLocalized;
int idLocalized;

int conexionCaught;
int IDsuscripcionCaught;
int idCaught;


t_log* iniciar_logger(void);
void leer_config();
void armar_entrenadores(char**, char**, char**);
int cantidad(char**);
char** obtener_objetivos(char**, char**, t_list*);
void terminar_programa(int, t_log*, t_config*);
void obtener_objetivo_global();
void* planificar(Entrenador*);
#endif
