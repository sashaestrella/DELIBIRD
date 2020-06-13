#include "game-boy.h"
#include <src/utilsGeneral.h>

int main(int argc, char *argv[])
{


	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	//Loggear "soy un log"
	char* primerLog = "Soy game-boy";

	log_info(logger, primerLog);


	config = leer_config();

	config_set_value(config, "IP", "127.0.0.1");
	config_set_value(config, "PUERTO", "4444");
	config_save(config);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	//antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	//crear conexion

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	conexion = crear_conexion(ip, puerto);

	printf( "\nSe creo la conexion con el valor %d \n", conexion);


	//Enviar mensaje

	NewPokemon* newPokemon = malloc(sizeof(NewPokemon));
	newPokemon = parsearNewPokemon(argv[3], argv[4], argv[5], argv[6]);


	enviarNewPokemon(newPokemon, conexion);
	//Recibir mensaje
/*
	char* mensajeRecibido;

	mensajeRecibido=recibir_mensaje(conexion);
*/
	//Loguear mensaje recibido
/*
	log_info(logger, "Se recibio el mensaje: %s " ,mensajeRecibido);
*/

>>>>>>> enviarNewPokemon
	terminar_programa(conexion, logger, config);
}

//TODO
t_log* iniciar_logger(void)
{
	return log_create("game-boy.log", "Game Boy" , 1, LOG_LEVEL_INFO);

}

//TODO
t_config* leer_config(void)
{
	return config_create("game-boy.config");

}


//TODO
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);

	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
}
