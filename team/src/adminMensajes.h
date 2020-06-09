#ifndef ADMINMENSAJES_H_
#define ADMINMENSAJES_H_

#include "team.h"

int conexionGameBoy;
int idGameBoy;
int msgGameBoy;

char* msgAppeared;// = "8 0"; //11
char* msgLocalized;// = "9 0";
char* msgCaught;// = "13 0";

void generarConexiones();
void abrirEscuchas();
void empezarAnalizadoresDeMensajes();

// ----- Auxiliares ----- //

t_list* nuevosAppeared;
t_list* nuevosLocalized;
t_list* nuevosCaught;
t_list* mensajesRecibidos;

void noHayBroker();
void* suscribirseACola(ParametrosSuscripcion*);
void* administradorMensajesColas(int, int, int);
void enviarACK(int, int, int, int);
void cargarDatosConexionConfig();

void* recibirMensajesAppeared(int, int);
void* recibirMensajesLocalized(int, int);
void* recibirMensajesCaught(int, int);
void* adminMensajeAppeared(AppearedPokemonConIDs*);
void* adminMensajeLocalized(LocalizedPokemonConIDs*);
void* adminMensajeCaught(CaughtPokemonConIDs*);





#endif /* ADMINMENSAJES_H_ */
