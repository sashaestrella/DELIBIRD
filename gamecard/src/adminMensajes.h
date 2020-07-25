#ifndef ADMINMENSAJES_H_
#define ADMINMENSAJES_H_

#include "gamecard.h"

void generarConexiones();
void abrirEscuchas();
void empezarAnalizadoresDeMensajes();

// ----- Auxiliares ----- //

t_list* mensajesRecibidos;

void serve_client_gamecard(int*);
void process_request_gamecard(int, int);
void iniciar_servidor_gamecard(void);
void esperar_cliente_gamecard(int);

void* conexionConBroker();
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
t_list* obtenerPosiciones(char*);

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
int eliminarSiEsArchivoVacio(char*);
void eliminarLinea(int ,char*);
void eliminarSiEsCarpetaVacia(char*, char*);
void liberar_lista(char**);
int tamanioDeBloque(char*);
void actualizarTamanioPokemon(char*);

#endif /* ADMINMENSAJES_H_ */
