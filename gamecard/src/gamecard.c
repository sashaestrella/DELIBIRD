#include "gamecard.h"
#include "adminMensajes.h"


int main(void)
{
	sem_init(&colaNew, 0, 0);
	sem_init(&colaCatch, 0, 0);
	sem_init(&colaGet, 0, 0);
	sem_init(&suscripciones, 0, 1);
	sem_init(&reintentoNew, 0, 0);
	sem_init(&reintentoGet, 0, 0);
	sem_init(&reintentoCatch, 0, 0);
	mensajesCatch = list_create();
	mensajesNew = list_create();
	mensajesGet = list_create();

	leerConfig();
	logger = iniciar_logger();
	cantidadArchivos = 0;
	crearDirectorioTG();
	abrirAtenciones();

	NewPokemonConIDs* newPokemon = malloc(sizeof(NewPokemonConIDs));
	newPokemon->newPokemon = malloc(sizeof(NewPokemon));
	newPokemon->IDmensaje = 1;
	newPokemon->newPokemon->cantidad = 3;
	CoordenadasXY coordenadas;
	coordenadas.posicionX = 3;
	coordenadas.posicionY = 4;
	newPokemon->newPokemon->coordenadas = coordenadas;
	newPokemon->newPokemon->nombre = string_duplicate("Pikachu");
	newPokemon->newPokemon->tamanioNombrePokemon = strlen("Pikachu") + 1;

	agregarPokemon(newPokemon);

	/*pthread_t hiloConexionGameboy;
	pthread_create(&hiloConexionGameboy, NULL, (void*)noHayBroker, NULL);

	//pthread_detach(hiloConexionGameboy);


	pthread_t hiloConexionBroker;
	pthread_create(&hiloConexionBroker, NULL, (void*)conexionConBroker, NULL);
	//pthread_detach(hiloConexionBroker);


	//pthread_join(hiloConexionGameboy, NULL);
	pthread_join(hiloConexionBroker, NULL);
*/
	//char* primerLog = "Log de Gamecard";

	//log_info(logger, primerLog);

}

t_log* iniciar_logger(void)
{
	return log_create(rutaLog, " [GAMECARD] " , 1, LOG_LEVEL_INFO);
}

void leerConfig(){
	config = config_create("gamecard.config");
	ip = config_get_string_value(config,"IP_BROKER");
	puerto = config_get_string_value(config,"PUERTO_BROKER");
	puntoMontaje = config_get_string_value(config,"PUNTO_MONTAJE_TALLGRASS");
	tiempoReintento = config_get_int_value(config,"TIEMPO_DE_REINTENTO_OPERACION");
	tiempoReconexion = config_get_int_value(config,"TIEMPO_DE_REINTENTO_CONEXION");
	tiempoRetardo = config_get_int_value(config,"TIEMPO_DE_RETARDO_OPERACION");
	miPUERTO = config_get_string_value(config, "MI_PUERTO");
	miIP = config_get_string_value(config, "MI_IP");
	IDsuscripcionNew = config_get_int_value(config, "ID_SUSCRIPCION_NEW");
	IDsuscripcionGet = config_get_int_value(config, "ID_SUSCRIPCION_GET");
	IDsuscripcionCatch = config_get_int_value(config, "ID_SUSCRIPCION_CATCH");
	rutaLog = config_get_string_value(config, "RUTA_LOG");
	tamanioBloque = config_get_int_value(config, "BLOCKS_SIZE");
	cantidadBloques = config_get_int_value(config, "BLOCKS");
	magicNumber = config_get_string_value(config, "MAGIC_NUMBER");
}
