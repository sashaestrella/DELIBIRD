#include "gamecard.h"



int main(void)
{
	sem_init(&colaNew, 0, 0);
	sem_init(&colaNew, 0, 0);
	sem_init(&colaNew, 0, 0);
	sem_init(&suscripciones, 0, 1);
	mensajesCatch = list_create();
	mensajesNew = list_create();
	mensajesGet = list_create();
	mensajesRecibidos = list_create();

	leerConfig();
	cantidadArchivos = 0;
	crearDirectorioTG();

	abrirAtenciones();
	generarConexiones(0);


	logger = iniciar_logger();

	char* primerLog = "Log de Gamecard";

	log_info(logger, primerLog);

}

t_log* iniciar_logger(void)
{
	return log_create("gamecard.log", "Log de gamecard" , 1, LOG_LEVEL_INFO);
}

void leerConfig(){
	t_config* config = config_create("gamecard.config");
	ip = config_get_string_value(config,"IP_BROKER");
	puerto = config_get_string_value(config,"PUERTO_BROKER");
	puntoMontaje = config_get_string_value(config,"PUNTO_MONTAJE_TALLGRASS");
	tiempoReintento = config_get_int_value(config,"TIEMPO_DE_REINTENTO_OPERACION");
	tiempoReconexion = config_get_int_value(config,"TIEMPO_DE_REINTENTO_CONEXION");
	tiempoRetardo = config_get_int_value(config,"TIEMPO_DE_RETARDO_OPERACION");
	//close(config);
}
