#include "gamecard.h"

int main(void)
{

	cargarDatosConexionConfig();
	generarConexiones(0);

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	char* primerLog = "Soy un log";

	log_info(logger, primerLog);

}


t_log* iniciar_logger(void)
{
	return log_create("gamecard.log", "Log de gamecard" , 1, LOG_LEVEL_INFO);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}

void cargarDatosConexionConfig(){
	t_config* archivo_config =  config_create("gamecard.config");
	ip = config_get_string_value(archivo_config,"IP");
	puerto = config_get_string_value(archivo_config,"PUERTO");
}

