#ifndef ADMINMENSAJES_H_
#define ADMINMENSAJES_H_

#include "team.h"

char* msgAppeared;// = "8 0"; //11
char* msgLocalized;// = "9 0";
char* msgCaught;// = "13 0";

void generarConexiones();
void abrirEscuchas();
void empezarAnalizadoresDeMensajes();

void enviar_getPokemon(GetPokemon*);
void enviar_catchPokemon(CatchPokemon*);
bool descartar_localized_no_deseados(LocalizedPokemonConIDs*);
bool descartar_caught_no_deseados(CaughtPokemonConIDs*);
bool descartar_appeared_no_deseados(AppearedPokemonConIDs*);

// ----- Auxiliares ----- //

t_list* nuevosLocalized;
t_list* nuevosCaught;
t_list* mensajesRecibidos;



void noHayBroker();
void* suscribirseAColaAppeared();
void* suscribirseAColaLocalized();
void* suscribirseAColaCaught();
void* administradorMensajesColas(int, int, int);
void enviarACK(int, int, int, int);

void* recibirMensajesAppeared();
void* recibirMensajesLocalized();
void* recibirMensajesCaught();
void* adminMensajeAppeared(AppearedPokemonConIDs*);
void* adminMensajeLocalized(LocalizedPokemonConIDs*);
void* adminMensajeCaught(CaughtPokemonConIDs*);





#endif /* ADMINMENSAJES_H_ */
