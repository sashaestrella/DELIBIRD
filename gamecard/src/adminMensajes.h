#ifndef ADMINMENSAJES_H_
#define ADMINMENSAJES_H_

#include "gamecard.h"

void generarConexiones();
void abrirEscuchas();

// --------------------- GAMEBOY --------------------- //

void serve_client_gamecard(int*);
void process_request_gamecard(int, int);
void iniciar_servidor_gamecard(void);
void esperar_cliente_gamecard(int);

// --------------------- BROKER --------------------- //

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

// --------------------- INICIALIZAR --------------------- //

void crearDirectorioTG();
void crearMetadata(char*);
void crearFiles(char*);
void crearBlocks(char*);
void generarBitmap(char*, t_config*);

// --------------------- ENVIAR MENSAJES --------------------- //

void enviarMensajeAppeared(int , char*, CoordenadasXY);
void enviarMensajeCaught(int , int);
void enviarMensajeLocalized(int, char*, t_list*);

// --------------------- BITMAP --------------------- //

int obtenerYEscribirProximoDisponible();
void eliminarBit(int);

// --------------------- FS --------------------- //

void armarFolderPara(char*);
void crearMetadataPara(char*);
int archivoAbierto(char*);
int existePokemon(char*);
int tamanioDeBloque(char*);
void actualizarTamanioPokemon(char*);

// --------------------- AGREGAR --------------------- //

void agregarPokemon(NewPokemonConIDs*);
void agregarPokemonAUnBloque(char**, NewPokemonConIDs*);
void agregarBloqueAMetadata(char*, char*);
void generarBloqueNuevo(char*, char*);
void generarEscrituraEnBloque(char*, char*);
void pisarBloque(char*, char*);
char** lecturaBloques(char**);
char* lecturaBloque(char*);
int esBloqueLleno(char*, char*);
void escribirBloques(char**, char**, int, char*);

// --------------------- ELIMINAR --------------------- //

void eliminarPokemon(CatchPokemonConIDs*);
void eliminarBloqueDeMetadata(char*, char*);
int eliminarSiEsArchivoVacio(char*);
int recorrerParaEliminarArchivo(char**, char*);
void eliminarSiEsCarpetaVacia(char*, char*);

// --------------------- BUSCAR --------------------- //

t_list* obtenerPosiciones(char*);
void obtenerCantidadYPosiciones(GetPokemonConIDs*);

// --------------------- AUX --------------------- //

int cantidadDeBloques(char**);
void liberar_lista(char**);
#endif /* ADMINMENSAJES_H_ */
