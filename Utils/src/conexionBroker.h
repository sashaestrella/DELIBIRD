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

	op_code colaASuscribirse;
	uint32_t nuevoExistente;
}ParametrosSuscripcion;

/*int conexionGameBoy;
int idGameBoy;
int msgGameBoy;*/

char* ip;
char* puerto;


/*void noHayBroker();
void* suscribirseACola(ParametrosSuscripcion*);
void* administradorMensajesColas(int, int, int);*/
void enviarACK(int, int, int, int);
void cargarDatosConexionConfig();

// -------- Funciones Team -------- //
/*t_list* nuevosAppeared;
t_list* nuevosLocalized;
t_list* nuevosCaught;
t_list* mensajesRecibidos;

void* recibirMensajesAppeared(int, int);
void* recibirMensajesLocalized(int, int);
void* recibirMensajesCaught(int, int);
void* adminMensajeAppeared(AppearedPokemonConIDs*);
void* adminMensajeLocalized(LocalizedPokemonConIDs*);
void* adminMensajeCaught(CaughtPokemonConIDs*);*/

// -------- Funciones Game Card -------- //


// ------------------------ //


#endif /* SRC_CONEXIONBROKER_H_ */
