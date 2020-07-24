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
int existePosicionPokemon(char*, CoordenadasXY);
t_list* obtenerPosiciones(int, char*, char*);

void agregarPokemon(NewPokemonConIDs*);
void eliminarPokemon(CatchPokemonConIDs*);
void obtenerCantidadYPosiciones(GetPokemonConIDs*);

void enviarMensajeAppeared(int , char*, CoordenadasXY);
void enviarMensajeCaught(int , int);
void enviarMensajeLocalized(int, char*, t_list*);

int obtenerYEscribirProximoDisponible();
void eliminarBit(int);

void eliminarBloqueDeMetadata(char*, char*);
void agregarPokemonAUnBloque(char**, NewPokemonConIDs*);
int esBloqueLleno(char*);
void agregarBloqueAMetadata(NewPokemonConIDs*);
void generarEscrituraEnBloque(NewPokemonConIDs*, char*);
int aumentarCantidadPokemon(char*, CoordenadasXY, int);
void generarBloqueNuevo(NewPokemonConIDs*, char*);

#endif /* ADMINMENSAJES_H_ */
