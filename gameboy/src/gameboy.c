

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
	int idNewPokemon = 0;
	nombrePokemon = "PIKACHU";
	unNewPokemon->nombre = nombrePokemon;
	unNewPokemon->coordenadas.posicionX = 2;
	unNewPokemon->coordenadas.posicionY = 3;
	unNewPokemon->cantidad = 3;
	enviarNewPokemon(unNewPokemon, conexion,idNewPokemon);

	printf("\nEnvie el mensaje: %s, con id: %d\n",unNewPokemon->nombre,idNewPokemon);
*/


	LocalizedPokemon* localizedPokemon1 = malloc(sizeof(LocalizedPokemon));
	char* nombre = malloc(9);
	nombre = "LOCALIZED";
	localizedPokemon1->nombre = nombre;
	localizedPokemon1->cantidadParesOrdenados = 2;

	int idmensaje = 0;
	int idCorrelativo = 2;
	int numero = 1;
	int numero2 = 2;
	CoordenadasXY* unasCoordenadas = malloc(sizeof(CoordenadasXY));
	unasCoordenadas->posicionX = numero;
	unasCoordenadas->posicionY = numero2;
		t_list* pares = list_create();
		list_add(pares,unasCoordenadas);
		list_add(pares,unasCoordenadas);
	localizedPokemon1->paresOrdenados = pares;
	enviarLocalizedPokemon(localizedPokemon1,conexion,idmensaje,idCorrelativo);

	printf("\nEnvie el mensaje: %s\n",localizedPokemon1->nombre);

/*
	GetPokemon* getPokemon1 = malloc(sizeof(GetPokemon));
	char* nombre = malloc(5);
	nombre = "GET";
	getPokemon1->nombre = nombre;
	int id = 0;

	enviarGetPokemon(getPokemon1,conexion,id);

	printf("\nEnvie el mensaje: %s\n",getPokemon1->nombre);
*/
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

/*	CatchPokemon* catchPokemon1 = malloc(sizeof(CatchPokemon));
		char* nombre = malloc(7);
		nombre = "CATCH1";
		catchPokemon1->nombre = nombre;
		catchPokemon1->coordenadas.posicionX = 2;
		catchPokemon1->coordenadas.posicionY = 5;
		int id = 0;
		enviarCatchPokemon(catchPokemon1,conexion,id);

		printf("\nEnvie el mensaje: %s\n",catchPokemon1->nombre);

*/
/*	CaughtPokemon* caughtPokemon1 = malloc(sizeof(CaughtPokemon));
		caughtPokemon1->atrapar = 0;

		int id = 0;
		int idCorrelativo = 3;
		enviarCaughtPokemon(caughtPokemon1,conexion,id,idCorrelativo);

		printf("\nEnvie el mensaje: %d\n",caughtPokemon1->atrapar);
*/

/*	int idSuscriptorPosta;
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

	free(caughtConIDs);
*/
/*
	int cod_op = 12;
	int idSuscriptorPosta;

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

	CatchPokemonConIDs* catchConIDs;
	CatchPokemon* unCatchPokemonTemporal;

	for(int i = 0; i<tamanioLista;i++){
		recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
		catchConIDs = recibir_CATCH_POKEMON(conexion,&size,1);
		unCatchPokemonTemporal = catchConIDs->catchPokemon;
		printf("[gameboy] Recibi un %s,con ID: %d\n",unCatchPokemonTemporal->nombre,catchConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,catchConIDs->IDmensaje);
	}

	free(catchConIDs);
*/
/*
	int idSuscriptorPosta;
	int cod_op = 11;

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

	AppearedPokemonConIDs* appearedConIDs;
	AppearedPokemon* unAppearedPokemonTemporal;

	for(int i = 0; i<tamanioLista;i++){
		recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
		appearedConIDs = recibir_APPEARED_POKEMON(conexion,&size,1,0);
		unAppearedPokemonTemporal = appearedConIDs->appearedPokemon;
		printf("[gameboy] Recibi un %s,con ID: %d\n",unAppearedPokemonTemporal->nombre,appearedConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,appearedConIDs->IDmensaje);
	}

	free(appearedConIDs);

*/
/*
	int idSuscriptorPosta;
	int cod_op = 10;

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

	GetPokemonConIDs* getConIDs;
	GetPokemon* unGetPokemonTemporal;

	for(int i = 0; i<tamanioLista;i++){
		recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
		getConIDs = recibir_GET_POKEMON(conexion,&size,1);
		unGetPokemonTemporal = getConIDs->getPokemon;
		printf("[gameboy] Recibi un %s,con ID: %d\n",unGetPokemonTemporal->nombre,getConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,getConIDs->IDmensaje);
	}

	free(getConIDs);
*/
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

	free(localizedConIDs);
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
