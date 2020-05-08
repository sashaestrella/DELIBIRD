#include "game-boy.h"
#include <src/utilsGeneral.h>

int main(void)
{
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
	puts("aca anda");

	config_set_value(config, "IP", "127.0.0.1");
	config_set_value(config, "PUERTO", "4444");
	config_save(config);

	//Crear conexion
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	conexion = crear_conexion(ip, puerto);

	printf( "\nSe creo la conexion con el valor %d \n", conexion);

	//Enviar mensaje

	int tamanio_maximo_mensaje = 256;

	char* mensajeAEnviar = malloc(tamanio_maximo_mensaje);

	NewPokemon* newPokemon = malloc(sizeof(NewPokemon));

	printf("\nIngrese el mensaje:");

	//fgets(mensajeAEnviar,tamanio_maximo_mensaje,stdin);

	//newPokemon = parsearNewPokemon(mensajeAEnviar);

	//	BROKER NEWPOKEMON PIKACHU 3 2 1

	//printf("Se va a enviar el pokemon %s", newPokemon->nombre);

	enviarNewPokemon(newPokemon, conexion);
	//enviar_mensaje(mensajeAEnviar, conexion);

	//Recibir mensaje
/*
	char* mensajeRecibido;

	mensajeRecibido=recibir_mensaje(conexion);
*/
	//Loguear mensaje recibido
/*
	log_info(logger, "Se recibio el mensaje: %s " ,mensajeRecibido);
*/
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
