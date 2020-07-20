#ifndef ADMINMENSAJES_H_
#define ADMINMENSAJES_H_

#include "gamecard.h"

void generarConexiones();
void abrirEscuchas();
void empezarAnalizadoresDeMensajes();

// ----- Auxiliares ----- //

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

void abrirAtenciones();
void* atenderMensajesNew();
void* atenderMensajesCatch();
void* atenderMensajesGet();

void* crearDirectorioTG();
void* crearMetadata(char*);
void* crearFiles(char*);
void* crearBlocks(char*);
void* generarBitmap(char*, t_config*);

void* armarFolderPara(char*);
void* crearMetadataPara(char*);
int archivoAbierto(char*);
int existePokemon(char*);
int existePosicion(char**, CoordenadasXY);
t_list* obtenerPosiciones(int, char*, char*);

void agregarPokemon(NewPokemonConIDs*);
void eliminarPokemon(CatchPokemonConIDs*);
void obtenerCantidadYPosiciones(GetPokemonConIDs*);

int enviarMensajeAppeared(int , char*, CoordenadasXY);
int enviarMensajeCaught(int , int);
int enviarMensajeLocalized(int, char*, t_list*);

#endif /* ADMINMENSAJES_H_ */
