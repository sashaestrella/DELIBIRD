#include "gamecard.h"



int main(void)
{
	sem_init(&suscripciones, 0, 1);
	leerConfig();
	cantidadArchivos = 0;
	crearDirectorioTG();
	/*generarConexiones(0);
	t_log* logger;

	logger = iniciar_logger();

	char* primerLog = "Log de Gamecard";

	log_info(logger, primerLog);
	*/

}

t_log* iniciar_logger(void)
{
	return log_create("team.log", "Log de team" , 1, LOG_LEVEL_INFO);
}

void leerConfig(){
	t_config* config = config_create("gamecard.config");
	ip = config_get_string_value(config,"IP");
	puerto = config_get_string_value(config,"PUERTO");
	puntoMontaje = config_get_string_value(config,"PUNTO_MONTAJE_TALLGRASS");
	char* tiempoRein;
	char* tiempoReco;
	char* tiempoReta;
	tiempoRein = config_get_string_value(config,"TIEMPO_DE_REINTENTO_OPERACION");
	tiempoReco = config_get_string_value(config,"TIEMPO_DE_REINTENTO_CONEXION");
	tiempoReta = config_get_string_value(config,"TIEMPO_DE_RETARDO_OPERACION");
	tiempoReintento = atoi(tiempoRein);
	tiempoReconexion = atoi(tiempoReco);
	tiempoRetardo = atoi(tiempoReta);
	//close(config);
}
