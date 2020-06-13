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
/*
	int idSuscriptorPosta;
		int cod_op = 9;

		Suscriptor* unSuscriptor1 = malloc(sizeof(Suscriptor));
		unSuscriptor1->socketSuscriptor = 4;
		unSuscriptor1->IDsuscriptor = 0;

		enviarSuscripcion(unSuscriptor1->IDsuscriptor,conexion,cod_op);
		printf("Envie suscripcion para la cola de mensajes %d",cod_op);
		recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
		printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptorPosta);

		//printf("Ya fui incluido en la cola de suscriptores de New Pokemon, lo que mi ID es el mismo: %d\n",idSuscriptor);

		int tamanioLista;

		recv(conexion,&tamanioLista,sizeof(int),MSG_WAITALL);
		if(tamanioLista == 0){
			puts("\nNo puedo recibir la lista porque esta vacia");
			printf("Tamaño lista: %d",tamanioLista);
		}
		printf("El tamaño de la lista que voy a recibir es: %d\n",tamanioLista);

		int size;
		int variableQueNoUsoxd;
		int ack = 1;

		LocalizedPokemonConIDs* localizedConIDs;
		LocalizedPokemon* unLocalizedPokemonTemporal;

		for(int i = 0; i<tamanioLista;i++){
			recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
			localizedConIDs = recibir_LOCALIZED_POKEMON(conexion,&size,1);
			unLocalizedPokemonTemporal = localizedConIDs->localizedPokemon;
			printf("[gameboy] Recibi un %s,con ID: %d\n",unLocalizedPokemonTemporal->nombre,localizedConIDs->IDmensaje);
			send(conexion,&ack,sizeof(int),0);
			printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,localizedConIDs->IDmensaje);
		}

		free(localizedConIDs)*/;
	/*int idSuscriptorPosta;
		int cod_op = 13;

		Suscriptor* unSuscriptor1 = malloc(sizeof(Suscriptor));
		unSuscriptor1->socketSuscriptor = 4;
		unSuscriptor1->IDsuscriptor = 0;

		enviarSuscripcion(unSuscriptor1->IDsuscriptor,conexion,cod_op);
		printf("Envie suscripcion para la cola de mensajes %d",cod_op);
		recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
		printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptorPosta);

		//printf("Ya fui incluido en la cola de suscriptores de New Pokemon, lo que mi ID es el mismo: %d\n",idSuscriptor);

		int tamanioLista;

		recv(conexion,&tamanioLista,sizeof(int),MSG_WAITALL);
		if(tamanioLista == 0){
			puts("\nNo puedo recibir la lista porque esta vacia");
			printf("Tamaño lista: %d",tamanioLista);
		}
		printf("El tamaño de la lista que voy a recibir es: %d\n",tamanioLista);

		int size;
		int variableQueNoUsoxd;
		int ack = 1;

		CaughtPokemonConIDs* caughtConIDs;
		CaughtPokemon* unCaughtPokemonTemporal;

		for(int i = 0; i<tamanioLista;i++){
			recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
			caughtConIDs = recibir_CAUGHT_POKEMON(conexion,&size,1);
			unCaughtPokemonTemporal = caughtConIDs->caughtPokemon;
			printf("[gameboy] Recibi un %d[MensajeCaughtPokemon],con ID: %d\n",unCaughtPokemonTemporal->atrapar,caughtConIDs->IDmensaje);
			send(conexion,&ack,sizeof(int),0);
			printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,caughtConIDs->IDmensaje);
		}

		free(caughtConIDs);*/
	/*AppearedPokemon* appearedPokemon1 = malloc(sizeof(AppearedPokemon));
	char* nombre = malloc(9);
	nombre = "APPEARED1";
	appearedPokemon1->nombre = nombre;
	appearedPokemon1->coordenadas.posicionX = 7;
	appearedPokemon1->coordenadas.posicionY = 3;
	printf("Coordenadas de appeared: %d %d",appearedPokemon1->coordenadas.posicionX,appearedPokemon1->coordenadas.posicionY);
	int id = 0;
	int idCorrelativo = 1;

	enviarAppearedPokemon(appearedPokemon1,conexion,id,idCorrelativo);
	printf("\nEnvie el mensaje: %s\n",appearedPokemon1->nombre);
*/
/*
		int idSuscriptorPosta;
		int cod_op = 8;

		Suscriptor* unSuscriptor1 = malloc(sizeof(Suscriptor));
		unSuscriptor1->socketSuscriptor = 4;
		unSuscriptor1->IDsuscriptor = 0;

		enviarSuscripcion(unSuscriptor1->IDsuscriptor,conexion,cod_op);
		printf("Envie suscripcion para la cola de mensajes %d",cod_op);
		recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
		printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptorPosta);

		//printf("Ya fui incluido en la cola de suscriptores de New Pokemon, lo que mi ID es el mismo: %d\n",idSuscriptor);

		int tamanioListaNP;

		recv(conexion,&tamanioListaNP,sizeof(int),MSG_WAITALL);
		if(tamanioListaNP == 0){
			puts("\nNo puedo recibir la lista porque esta vacia");
			printf("Tamaño lista: %d",tamanioListaNP);
		}
		printf("El tamaño de la lista que voy a recibir es: %d\n",tamanioListaNP);

		int size;
		int variableQueNoUsoxd;
		int ack = 1;

		NewPokemonConIDs* newConIDs;
		NewPokemon* unNewPokemonTemporal;

		for(int i = 0; i<tamanioListaNP;i++){
			recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
			newConIDs = recibir_NEW_POKEMON(conexion,&size,1);
			unNewPokemonTemporal = newConIDs->newPokemon;
			printf("[gameboy] Recibi un %s,con ID: %d\n",unNewPokemonTemporal->nombre,newConIDs->IDmensaje);
			send(conexion,&ack,sizeof(int),0);
			printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,newConIDs->IDmensaje);
		}

		free(newConIDs);
*/
		int idSuscriptorPosta;
		int cod_op = 8;

		Suscriptor* unSuscriptor1 = malloc(sizeof(Suscriptor));
		unSuscriptor1->socketSuscriptor = 4;
		unSuscriptor1->IDsuscriptor = 0;

		enviarSuscripcion(unSuscriptor1->IDsuscriptor,conexion,cod_op);
		printf("Envie suscripcion para la cola de mensajes %d",cod_op);
		recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
		printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptorPosta);

		//printf("Ya fui incluido en la cola de suscriptores de New Pokemon, lo que mi ID es el mismo: %d\n",idSuscriptor);

		int tamanioListaNP;

		recv(conexion,&tamanioListaNP,sizeof(int),MSG_WAITALL);
		if(tamanioListaNP == 0){
			puts("\nNo puedo recibir la lista porque esta vacia");
			printf("Tamaño lista: %d",tamanioListaNP);
		}
		printf("El tamaño de la lista que voy a recibir es: %d\n",tamanioListaNP);

		int size;
		int variableQueNoUsoxd;
		int ack = 1;

		NewPokemonConIDs* newConIDs;
		NewPokemon* unNewPokemonTemporal;

		for(int i = 0; i<tamanioListaNP;i++){
			recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
			newConIDs = recibir_NEW_POKEMON(conexion,&size,1);
			unNewPokemonTemporal = newConIDs->newPokemon;
			printf("[gameboy] Recibi un %s,con ID: %d\n",unNewPokemonTemporal->nombre,newConIDs->IDmensaje);
			send(conexion,&ack,sizeof(int),0);
			printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,newConIDs->IDmensaje);
		}

		free(newConIDs);


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
