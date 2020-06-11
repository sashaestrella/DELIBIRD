/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "gamecard.h"

int main(void)
{

	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;


	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	char* primerLog = "Soy un log";

	log_info(logger, primerLog);

	//Loggear "soy un log"

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

	NewPokemon* unNewPokemon = malloc(sizeof(NewPokemon));
		char* nombrePokemon = malloc(8);
		int idNewPokemon = 0;
		nombrePokemon = "PIKACHU";
		unNewPokemon->nombre = nombrePokemon;
		unNewPokemon->coordenadas.posicionX = 2;
		unNewPokemon->coordenadas.posicionY = 3;
		unNewPokemon->cantidad = 3;
		enviarNewPokemon(unNewPokemon, conexion,idNewPokemon);

		printf("\nEnvie el mensaje: %s, con id: %d\n",unNewPokemon->nombre,idNewPokemon);

/*
	//enviar mensaje

	int tamanio_maximo_mensaje = 256;

	char* mensajeAEnviar = malloc(tamanio_maximo_mensaje);

	printf("\nIngrese el mensaje:");

	fgets(mensajeAEnviar,tamanio_maximo_mensaje,stdin);

	printf("Se va a enviar el mensaje %s", mensajeAEnviar);

	enviar_mensaje(mensajeAEnviar, conexion); //Envia el mensaje

	//recibir mensaje

	char* mensajeRecibido;

	mensajeRecibido=recibir_mensaje(conexion);


	//loguear mensaje recibido

	log_info(logger, "Se recibio el mensaje: %s " ,mensajeRecibido);

	terminar_programa(conexion, logger, config);*/
}

//TODO
t_log* iniciar_logger(void)
{
	return log_create("gamecard.log", "Log de Game Card" , 1, LOG_LEVEL_INFO);

}

//TODO
t_config* leer_config(void)
{
	return config_create("gamecard.config");

}

//TODO
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);

	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
}
