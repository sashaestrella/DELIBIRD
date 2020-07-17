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
void* suscribirseAColaNew();
void* suscribirseAColaGet();
void* suscribirseAColaCatch();
void* administradorMensajesColas(int, int, int);
void enviarACK(int, int, int, int);
void cargarDatosConexionConfig();

void* recibirMensajesNew();
void* recibirMensajesGet();
void* recibirMensajesCatch();
void* adminMensajeNewPokemon(NewPokemonConIDs*);
void* adminMensajeGetPokemon(GetPokemonConIDs*);
void* adminMensajeCatch(CatchPokemonConIDs*);

void* crearDirectorioTG();
void* crearMetadata(char*);
void* crearFiles(char*);
void* crearBlocks(char*);
void* generarBitmap(char*);

void* armarFolderPara(char*);
void* crearMetadataPara(char*);
int archivoAbierto(char*);
int existePokemon(char*);
int existePosicion(char**, CoordenadasXY);
t_list* obtenerPosiciones(int, char*, char*);

void agregarPokemon(NewPokemonConIDs*);
void eliminarPokemon(CatchPokemonConIDs*);
LocalizedPokemon* obtenerCantidadYPosiciones(GetPokemonConIDs*);

int enviarMensajeAppeared(int idMensaje, char* pokemon, CoordenadasXY coordenadas);
int enviarMensajeCaught(int idMensaje, int resultado);
int enviarMensajeLocalized(int idMensaje, char* pokemon, CoordenadasXY coordenadas, int cantidad);

#endif /* ADMINMENSAJES_H_ */
