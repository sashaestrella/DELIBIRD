

#include "game-boy.h"

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

	printf( "\nSe creo la conexion con el valor %d \n", conexion);


	//enviar mensajes
	/*
	NewPokemon* unNewPokemon = malloc(sizeof(NewPokemon));
	char* nombrePokemon = malloc(8);
	int id = 0;
	nombrePokemon = "PIKACHU";
	unNewPokemon->nombre = nombrePokemon;
	unNewPokemon->coordenadas.posicionX = 2;
	unNewPokemon->coordenadas.posicionY = 3;
	unNewPokemon->cantidad = 3;
	enviarNewPokemon(unNewPokemon, conexion,id);

	printf("\nEnvie el mensaje: %s, con id: %d\n",unNewPokemon->nombre,id);

	*/
/*
	LocalizedPokemon* localizedPokemon1 = malloc(sizeof(LocalizedPokemon));
	char* nombre = malloc(9);
	nombre = "LOCALIZED";
	localizedPokemon1->nombre = nombre;
	localizedPokemon1->cantidadParesOrdenados = 2;

	int idmensaje = 0;
	int idCorrelativo;
	int numero = 1;
		t_list* pares = list_create();
		list_add(pares,&numero);
		list_add(pares,&numero);
		list_add(pares,&numero);
		list_add(pares,&numero);
	localizedPokemon1->paresOrdenados = pares;
	enviarLocalizedPokemon(localizedPokemon1,conexion,idmensaje,idCorrelativo);

	printf("\nEnvie el mensaje: %s\n",localizedPokemon1->nombre);
*/
/*
	GetPokemon* getPokemon1 = malloc(sizeof(GetPokemon));
	char* nombre = malloc(5);
	nombre = "GET";
	getPokemon1->nombre = nombre;
	int id = 0;

	enviarGetPokemon(getPokemon1,conexion,id);

	printf("\nEnvie el mensaje: %s\n",getPokemon1->nombre);
*/
/*	AppearedPokemon* appearedPokemon1 = malloc(sizeof(AppearedPokemon));
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
	CatchPokemon* catchPokemon1 = malloc(sizeof(CatchPokemon));
		char* nombre = malloc(7);
		nombre = "CATCH1";
		catchPokemon1->nombre = nombre;
		catchPokemon1->coordenadas.posicionX = 2;
		catchPokemon1->coordenadas.posicionY = 5;

		int id = 0;
		enviarCatchPokemon(catchPokemon1,conexion,id);

		printf("\nEnvie el mensaje: %s\n",catchPokemon1->nombre);

		*/
/*
	CaughtPokemon* caughtPokemon1 = malloc(sizeof(CaughtPokemon));
		caughtPokemon1->atrapar = 0;

		int id = 0;
		int idCorrelativo;
		enviarCaughtPokemon(caughtPokemon1,conexion,id,idCorrelativo);

		printf("\nEnvie el mensaje: %d\n",caughtPokemon1->atrapar);
*/
/*
	int cod_op = 13;
			send(conexion,&cod_op,sizeof(int),0);
			puts("\nSuscripcion a CaughtPokemon enviada");

			int idSuscriptor;
			recv(conexion,&idSuscriptor,sizeof(int),0);

			printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptor);

			int tamanioListaCAP;
			int IDmensajeCAP;
			recv(conexion,&tamanioListaCAP,sizeof(int),0);
			printf("\nEl tamaño de la lista que voy a recibir es: %d",tamanioListaCAP);
			int size;
			int variableQueNoUsoxd;
			CaughtPokemon* unCaughtPokemonTemporal;

			for(int i = 0; i<tamanioListaCAP;i++){

				recv(conexion,&variableQueNoUsoxd,sizeof(int),0);
				unCaughtPokemonTemporal = recibir_CAUGHT_POKEMON(conexion,&size);
				printf("\n[gameboy] Recibi un %d",unCaughtPokemonTemporal->atrapar);
			}
		free(unCaughtPokemonTemporal);
*/
/*
	int cod_op = 12;
		send(conexion,&cod_op,sizeof(int),0);
		puts("\nSuscripcion a CatchPokemon enviada");

		int idSuscriptor;
		recv(conexion,&idSuscriptor,sizeof(int),0);

		printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptor);

		int tamanioListaCP;
		int IDmensajeCP;
		recv(conexion,&tamanioListaCP,sizeof(int),0);
		printf("\nEl tamaño de la lista que voy a recibir es: %d",tamanioListaCP);
		int size;
		int variableQueNoUsoxd;
		CatchPokemon* unCatchPokemonTemporal;

		for(int i = 0; i<tamanioListaCP;i++){

			recv(conexion,&variableQueNoUsoxd,sizeof(int),0);
			unCatchPokemonTemporal = recibir_CATCH_POKEMON(conexion,&size);
			printf("\n[gameboy] Recibi un %s",unCatchPokemonTemporal->nombre);

		}
	free(unCatchPokemonTemporal);
*/
/*
	int cod_op = 11;
		send(conexion,&cod_op,sizeof(int),0);
		puts("\nSuscripcion a AppearedPokemon enviada");

		int idSuscriptor;
		recv(conexion,&idSuscriptor,sizeof(int),0);

		printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptor);

		int tamanioListaAP;
		int IDmensajeAP;
		recv(conexion,&tamanioListaAP,sizeof(int),0);
		printf("\nEl tamaño de la lista que voy a recibir es: %d",tamanioListaAP);
		int size;
		int variableQueNoUsoxd;
		AppearedPokemon* unAppearedPokemonTemporal;

		for(int i = 0; i<tamanioListaAP;i++){

			recv(conexion,&variableQueNoUsoxd,sizeof(int),0);
			unAppearedPokemonTemporal = recibir_APPEARED_POKEMON(conexion,&size);
			printf("\n[gameboy] Recibi un %s",unAppearedPokemonTemporal->nombre);

		}
	free(unAppearedPokemonTemporal);

*/
/*
	int cod_op = 10;
	send(conexion,&cod_op,sizeof(int),0);
	puts("\nSuscripcion a GetPokemon enviada");

	int idSuscriptor;
	recv(conexion,&idSuscriptor,sizeof(int),0);

	printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptor);

	int tamanioListaGP;
	int IDmensajeGP;
	recv(conexion,&tamanioListaGP,sizeof(int),0);
	printf("\nEl tamaño de la lista que voy a recibir es: %d",tamanioListaGP);
	int size;
	int variableQueNoUsoxd;
	GetPokemon* unGetPokemonTemporal;

	for(int i = 0; i<tamanioListaGP;i++){

		recv(conexion,&variableQueNoUsoxd,sizeof(int),0);
		unGetPokemonTemporal = recibir_GET_POKEMON(conexion,&size);
		printf("\n[gameboy] Recibi un %s",unGetPokemonTemporal->nombre);

	}
	free(unGetPokemonTemporal);
*/

/*	int cod_op = 9;

	send(conexion,&cod_op,sizeof(int),0);
	puts("\nSuscripcion a LocalizedPokemon enviada");

	int idSuscriptor;
	recv(conexion,&idSuscriptor,sizeof(int),0);

	printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptor);

		int tamanioListaLP;
		int IDmensajeLP;
		recv(conexion,&tamanioListaLP,sizeof(int),0);
		printf("\nEl tamaño de la lista que voy a recibir es: %d",tamanioListaLP);
		int size;
		int variableQueNoUsoxd;

		LocalizedPokemonConIDs* localizedConIDs;
		LocalizedPokemon* unLocalizedPokemon;

		for(int i = 0; i<tamanioListaLP;i++){
			recv(conexion,&variableQueNoUsoxd,sizeof(int),0);
			localizedConIDs = recibir_LOCALIZED_POKEMON(conexion,&size,IDmensajeLP);
			unLocalizedPokemon = localizedConIDs->localizedPokemon;
			printf("\n[gameboy] Recibi un %s\n",localizedConIDs->localizedPokemon->nombre);
		}
		free(localizedConIDs);
*/


	int cod_op = 8;

	send(conexion,&cod_op,sizeof(int),0);
	puts("\nSuscripcion a NewPokemon enviada");


	int idSuscriptor;
	recv(conexion,&idSuscriptor,sizeof(int),0);

	printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptor);
	int tamanioListaNP;

	recv(conexion,&tamanioListaNP,sizeof(int),MSG_WAITALL);
	if(tamanioListaNP == 0){
		puts("\nNo puedo recibir la lista porque esta vacia");
		printf("Tamaño lista: %d",tamanioListaNP);
	}
	printf("\nEl tamaño de la lista que voy a recibir es: %d\n",tamanioListaNP);

	int size;
	int id;
	int variableQueNoUsoxd;
	NewPokemon* unNewPokemonTemporal;

	for(int i = 0; i<tamanioListaNP;i++){
		recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
		unNewPokemonTemporal = recibir_NEW_POKEMON(conexion,&size,id);
		printf("\n[gameboy] Recibi un %s\n",unNewPokemonTemporal->nombre);

	}
	free(unNewPokemonTemporal);


/*
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
*/

	terminar_programa(conexion, logger, config);
}

//TODO
t_log* iniciar_logger(void)
{
	return log_create("gameboy.log", "Log de GameBoy" , 1, LOG_LEVEL_INFO);

}

//TODO
t_config* leer_config(void)
{
	return config_create("gameboy.config");

}

//TODO
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);

	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
}
