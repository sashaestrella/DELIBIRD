#include "gamecard.h"



int main(void)
{

	t_config* config = config_create("gamecard.config");
	ip = config_get_string_value(config,"IP");
	puerto = config_get_string_value(config,"PUERTO");
	generarConexiones(0);
	t_log* logger;

	logger = iniciar_logger();

	char* primerLog = "Soy un log";

	log_info(logger, primerLog);

}

t_log* iniciar_logger(void)
{
	return log_create("team.log", "Log de team" , 1, LOG_LEVEL_INFO);

}
