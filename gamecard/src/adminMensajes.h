#ifndef ADMINMENSAJES_H_
#define ADMINMENSAJES_H_

#include "gamecard.h"

void generarConexiones();
void abrirEscuchas();
void empezarAnalizadoresDeMensajes();

//void enviar_getPokemon(GetPokemon*);
//void enviar_catchPokemon(CatchPokemon*);
//bool descartar_localized_no_deseados(LocalizedPokemonConIDs*);
//bool descartar_caught_no_deseados(CaughtPokemonConIDs*);

// ----- Auxiliares ----- //

t_list* nuevosNewPokemon;
t_list* nuevosGetPokemon;
t_list* nuevosCatch;
t_list* mensajesRecibidos;



void noHayBroker();
void* suscribirseACola(ParametrosSuscripcion*);
void* administradorMensajesColas(int, int, int);
void enviarACK(int, int, int, int);
void cargarDatosConexionConfig();

void* recibirMensajesNew();
void* recibirMensajesGet();
void* recibirMensajesCatch();
void* adminMensajeNewPokemon(NewPokemonConIDs*);
void* adminMensajeGetPokemon(GetPokemonConIDs*);
void* adminMensajeCatch(CatchPokemonConIDs*);





#endif /* ADMINMENSAJES_H_ */
