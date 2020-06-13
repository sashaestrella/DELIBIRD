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
