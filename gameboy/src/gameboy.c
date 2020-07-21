#include "game-boy.h"
#include<src/utilsGeneral.h>
#include<src/utilsBroker.h>

t_log* logger;
t_config* config;

int esUnNumero(char* algo){
	for(int i = 0;i < strlen(algo);i++){
		if(!isdigit(algo[i])){
			return 0;
		}
	}
	return 1;
}

void suscribirNewPokemon(){
	int conexion;
	char* ip;
	char* puerto;
	int tamanioListaNP;
	int idSuscriptorPosta;
	int cod_op = 8;
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	NewPokemonConIDs* newConIDs;
	NewPokemon* unNewPokemonTemporal;

	ip = config_get_string_value(config, "IP_BROKER");
	puerto = config_get_string_value(config, "PUERTO_BROKER");
	printf("El ip y el puerto son: %s %s\n", ip, puerto);

	conexion = crear_conexion(ip, puerto);
	char* loQueVoyALoguear = "[GAMEBOY]Se creó la conexión con BROKER, con el valor %d.";
	log_info(logger,loQueVoyALoguear,conexion);

	enviarSuscripcion(0,conexion,cod_op);
	loQueVoyALoguear = "[GAMEBOY]Me suscribí a la cola NEW_POKEMON.";
	log_info(logger,loQueVoyALoguear);
	recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
	printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptorPosta);
	recv(conexion,&tamanioListaNP,sizeof(int),MSG_WAITALL);
	if(tamanioListaNP == 0){
		puts("\nNo puedo recibir la lista porque esta vacia");
		printf("Tamaño lista: %d",tamanioListaNP);
	}
	printf("El tamaño de la lista que voy a recibir es: %d\n",tamanioListaNP);

	for(int i = 0; i<tamanioListaNP;i++){
		recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
		newConIDs = recibir_NEW_POKEMON(conexion,&size,1);
		unNewPokemonTemporal = newConIDs->newPokemon;
		printf("[GAMEBOY]Recibí un %s,%i,%i,%i, con ID: %d\n",unNewPokemonTemporal->nombre,unNewPokemonTemporal->coordenadas.posicionX, unNewPokemonTemporal->coordenadas.posicionY, unNewPokemonTemporal->cantidad, newConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[GAMEBOY]ACK=%d del mensaje %d fue enviado\n",ack,newConIDs->IDmensaje);
	}
	/*if(tamanioListaNP != 0){
		free(newConIDs);
	}*/
	while(1){
		int cod_op;
		recv(conexion, &cod_op, sizeof(int), 0);
		newConIDs = recibir_NEW_POKEMON(conexion, 0, 1);
		unNewPokemonTemporal = newConIDs->newPokemon;
		loQueVoyALoguear = "[GAMEBOY]Llegó un nuevo mensaje a la cola NEW_POKEMON.Recibí un %s,%i,%i,%i, con ID: %d.";
		log_info(logger,loQueVoyALoguear,unNewPokemonTemporal->nombre,unNewPokemonTemporal->coordenadas.posicionX, unNewPokemonTemporal->coordenadas.posicionY, unNewPokemonTemporal->cantidad, newConIDs->IDmensaje);
		int ack = 1;
		send(conexion, &ack, sizeof(int), 0);
	}
}

void suscribirLocalizedPokemon(){
	int conexion;
	char* ip;
	char* puerto;
	ip = config_get_string_value(config, "IP_BROKER");
	puerto = config_get_string_value(config, "PUERTO_BROKER");
	printf("el ip y el puerto son: %s %s", ip, puerto);

	int idSuscriptorPosta;
	int cod_op = 9;
	int tamanioLista;
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	LocalizedPokemonConIDs* localizedConIDs;
	LocalizedPokemon* unLocalizedPokemonTemporal;


	conexion = crear_conexion(ip, puerto);
	char* loQueVoyALoguear = "[GAMEBOY]Se creó la conexión con BROKER, con el valor %d.";
	log_info(logger,loQueVoyALoguear,conexion);

	enviarSuscripcion(0,conexion,cod_op);
	loQueVoyALoguear = "[GAMEBOY]Me suscribí a la cola LOCALIZED_POKEMON.";
	log_info(logger,loQueVoyALoguear);
	recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
	printf("\nRecibí mi id como suscriptor: %d\n",idSuscriptorPosta);

	recv(conexion,&tamanioLista,sizeof(int),MSG_WAITALL);
	if(tamanioLista == 0){
		puts("\nNo puedo recibir la lista porque esta vacia");
		printf("Tamaño lista: %d",tamanioLista);
	}
	printf("El tamaño de la lista que voy a recibir es: %d\n",tamanioLista);

	for(int i = 0; i<tamanioLista;i++){
		recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
		localizedConIDs = recibir_LOCALIZED_POKEMON(conexion,&size,1);
		unLocalizedPokemonTemporal = localizedConIDs->localizedPokemon;
		printf("[GAMEBOY]Recibí un %s,con %i pares ordenados y con ID: %d\n",unLocalizedPokemonTemporal->nombre,unLocalizedPokemonTemporal->cantidadParesOrdenados,localizedConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[GAMEBOY]ACK=%d del mensaje %d fue enviado\n",ack,localizedConIDs->IDmensaje);
	}
	//free(localizedConIDs);
	while(1){
		int cod_op;
		recv(conexion, &cod_op, sizeof(int), 0);
		localizedConIDs = recibir_LOCALIZED_POKEMON(conexion, 0, 1);
		unLocalizedPokemonTemporal = localizedConIDs->localizedPokemon;
		loQueVoyALoguear = "[GAMEBOY]Llegó un nuevo mensaje a la cola LOCALIZED_POKEMON.Recibí un %s,con %i pares ordenados y con ID: %d.";
		log_info(logger,loQueVoyALoguear,unLocalizedPokemonTemporal->nombre,unLocalizedPokemonTemporal->cantidadParesOrdenados,localizedConIDs->IDmensaje);
		int ack = 1;
		send(conexion, &ack, sizeof(int), 0);
	}
}

void suscribirGetPokemon(){
	int conexion;
	char* ip;
	char* puerto;
	ip = config_get_string_value(config, "IP_BROKER");
	puerto = config_get_string_value(config, "PUERTO_BROKER");
	puts("estoy en suscribir get");
	printf("el ip y el puerto son: %s %s", ip, puerto);

	int idSuscriptorPosta;
	int cod_op = 10;
	int tamanioLista;
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	GetPokemonConIDs* getConIDs;
	GetPokemon* unGetPokemonTemporal;


	conexion = crear_conexion(ip, puerto);
	char* loQueVoyALoguear = "[GAMEBOY]Se creó la conexión con BROKER, con el valor %d.";
	log_info(logger,loQueVoyALoguear,conexion);

	enviarSuscripcion(0,conexion,cod_op);
	loQueVoyALoguear = "[GAMEBOY]Me suscribí a la cola GET_POKEMON.";
	log_info(logger,loQueVoyALoguear);
	recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
	printf("\nRecibí mi id como suscriptor: %d\n",idSuscriptorPosta);

	recv(conexion,&tamanioLista,sizeof(int),MSG_WAITALL);
	if(tamanioLista == 0){
		puts("\nNo puedo recibir la lista porque esta vacia");
		printf("Tamaño lista: %d",tamanioLista);
	}
	printf("El tamaño de la lista que voy a recibir es: %d\n",tamanioLista);

	for(int i = 0; i<tamanioLista;i++){
		recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
		getConIDs = recibir_GET_POKEMON(conexion,&size,1);
		unGetPokemonTemporal = getConIDs->getPokemon;
		printf("[GAMEBOY]Recibí un %s,con ID: %d\n",unGetPokemonTemporal->nombre,getConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[GAMEBOY]ACK=%d del mensaje %d fue enviado\n",ack,getConIDs->IDmensaje);
	}
	//free(getConIDs);
	while(1){
		int cod_op;
		recv(conexion, &cod_op, sizeof(int), 0);
		getConIDs = recibir_GET_POKEMON(conexion, 0, 1);
		unGetPokemonTemporal = getConIDs->getPokemon;
		loQueVoyALoguear = "[GAMEBOY]Llegó un nuevo mensaje a la cola GET_POKEMON.Recibí un %s,con ID: %d.";
		log_info(logger,loQueVoyALoguear,unGetPokemonTemporal->nombre,getConIDs->IDmensaje);
		int ack = 1;
		send(conexion, &ack, sizeof(int), 0);
	}
}

void suscribirAppearedPokemon(){
	int conexion;
	char* ip;
	char* puerto;

	ip = config_get_string_value(config, "IP_BROKER");
	puerto = config_get_string_value(config, "PUERTO_BROKER");
	printf("el ip y el puerto son: %s %s", ip, puerto);

	int idSuscriptorPosta;
	int cod_op = 11;
	int tamanioLista;
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	AppearedPokemonConIDs* appearedConIDs;
	AppearedPokemon* unAppearedPokemonTemporal;


	conexion = crear_conexion(ip, puerto);
	char* loQueVoyALoguear = "[GAMEBOY]Se creó la conexión con BROKER, con el valor %d.";
	log_info(logger,loQueVoyALoguear,conexion);

	enviarSuscripcion(0,conexion,cod_op);
	loQueVoyALoguear = "[GAMEBOY]Me suscribi a la cola APPEARED_POKEMON.";
	log_info(logger,loQueVoyALoguear);
	recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
	printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptorPosta);

	recv(conexion,&tamanioLista,sizeof(int),MSG_WAITALL);
	if(tamanioLista == 0){
		puts("\nNo puedo recibir la lista porque esta vacia");
		printf("Tamaño lista: %d",tamanioLista);
	}
	printf("El tamaño de la lista que voy a recibir es: %d\n",tamanioLista);

	for(int i = 0; i<tamanioLista;i++){
		recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
		appearedConIDs = recibir_APPEARED_POKEMON(conexion,&size,1,0);
		unAppearedPokemonTemporal = appearedConIDs->appearedPokemon;
		printf("[GAMEBOY]Recibí un %s,%i,%i, con ID: %d\n",unAppearedPokemonTemporal->nombre,unAppearedPokemonTemporal->coordenadas.posicionX,unAppearedPokemonTemporal->coordenadas.posicionY,appearedConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[GAMEBOY]ACK=%d del mensaje %d fue enviado\n",ack,appearedConIDs->IDmensaje);
	}
	//free(appearedConIDs);
	while(1){
		int cod_op;
		recv(conexion, &cod_op, sizeof(int), 0);
		appearedConIDs = recibir_APPEARED_POKEMON(conexion, 0, 1, 0); // aca no iria id correlativo
		unAppearedPokemonTemporal = appearedConIDs->appearedPokemon;
		loQueVoyALoguear = "[GAMEBOY]Llegó un nuevo mensaje a la cola APPEARED_POKEMON.Recibí un %s,%i,%i, con ID: %d.";
		log_info(logger,loQueVoyALoguear,unAppearedPokemonTemporal->nombre,unAppearedPokemonTemporal->coordenadas.posicionX,unAppearedPokemonTemporal->coordenadas.posicionY,appearedConIDs->IDmensaje);
		int ack = 1;
		send(conexion, &ack, sizeof(int), 0);
	}
}

void suscribirCatchPokemon(){
	int conexion;
	char* ip;
	char* puerto;

	ip = config_get_string_value(config, "IP_BROKER");
	puerto = config_get_string_value(config, "PUERTO_BROKER");
	printf("el ip y el puerto son: %s %s", ip, puerto);

	int cod_op = 12;
	int idSuscriptorPosta;
	int tamanioLista;
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	CatchPokemonConIDs* catchConIDs;
	CatchPokemon* unCatchPokemonTemporal;


	conexion = crear_conexion(ip, puerto);
	char* loQueVoyALoguear = "[GAMEBOY]Se creó la conexión con BROKER, con el valor %d.";
	log_info(logger,loQueVoyALoguear,conexion);

	enviarSuscripcion(0,conexion,cod_op);
	loQueVoyALoguear = "[GAMEBOY]Me suscribí a la cola CATCH_POKEMON.";
	log_info(logger,loQueVoyALoguear);
	recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
	printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptorPosta);

	recv(conexion,&tamanioLista,sizeof(int),MSG_WAITALL);
	if(tamanioLista == 0){
		puts("\nNo puedo recibir la lista porque esta vacia");
		printf("Tamaño lista: %d",tamanioLista);
	}
	printf("El tamaño de la lista que voy a recibir es: %d\n",tamanioLista);

	for(int i = 0; i<tamanioLista;i++){
		recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
		catchConIDs = recibir_CATCH_POKEMON(conexion,&size,1);
		unCatchPokemonTemporal = catchConIDs->catchPokemon;
		printf("[GAMEBOY]Recibí un %s,%i,%i, con ID: %d\n",unCatchPokemonTemporal->nombre,unCatchPokemonTemporal->coordenadas.posicionX,unCatchPokemonTemporal->coordenadas.posicionY,catchConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[GAMEBOY]ACK=%d del mensaje %d fue enviado\n",ack,catchConIDs->IDmensaje);
	}
	//free(catchConIDs);
	while(1){
		int cod_op;
		recv(conexion, &cod_op, sizeof(int), 0);
		catchConIDs = recibir_CATCH_POKEMON(conexion, 0, 1);
		unCatchPokemonTemporal = catchConIDs->catchPokemon;
		loQueVoyALoguear = "[GAMEBOY]Llegó un nuevo mensaje a la cola CATCH_POKEMON.Recibí un %s,%i,%i, con ID: %d.";
		log_info(logger,loQueVoyALoguear,unCatchPokemonTemporal->nombre,unCatchPokemonTemporal->coordenadas.posicionX,unCatchPokemonTemporal->coordenadas.posicionY,catchConIDs->IDmensaje);
		int ack = 1;
		send(conexion, &ack, sizeof(int), 0);
	}
}

void suscribirCaughtPokemon(){
	int conexion;
	char* ip;
	char* puerto;

	ip = config_get_string_value(config, "IP_BROKER");
	puerto = config_get_string_value(config, "PUERTO_BROKER");
	puts("estoy en suscribir get");
	printf("el ip y el puerto son: %s %s", ip, puerto);

	int idSuscriptorPosta;
	int cod_op = 13;
	int tamanioLista;
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	CaughtPokemonConIDs* caughtConIDs;
	CaughtPokemon* unCaughtPokemonTemporal;


	conexion = crear_conexion(ip, puerto);
	char* loQueVoyALoguear = "[GAMEBOY]Se creó la conexión con BROKER, con el valor %d.";
	log_info(logger,loQueVoyALoguear,conexion);

	enviarSuscripcion(0,conexion,cod_op);
	loQueVoyALoguear = "[GAMEBOY]Me suscribí a la cola CAUGHT_POKEMON.";
	log_info(logger,loQueVoyALoguear);
	recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
	printf("\nRecibí mi id como suscriptor: %d\n",idSuscriptorPosta);

	recv(conexion,&tamanioLista,sizeof(int),MSG_WAITALL);
	if(tamanioLista == 0){
		puts("\nNo puedo recibir la lista porque esta vacia");
		printf("Tamaño lista: %d",tamanioLista);
	}
	printf("El tamaño de la lista que voy a recibir es: %d\n",tamanioLista);

	for(int i = 0; i<tamanioLista;i++){
		recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
		caughtConIDs = recibir_CAUGHT_POKEMON(conexion,&size,1);
		unCaughtPokemonTemporal = caughtConIDs->caughtPokemon;
		printf("[GAMEBOY]Recibí un %d[MensajeCaughtPokemon],con ID: %d\n",unCaughtPokemonTemporal->atrapar,caughtConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[GAMEBOY]ACK=%d del mensaje %d fue enviado\n",ack,caughtConIDs->IDmensaje);
	}
	//free(caughtConIDs);
	while(1){
		int cod_op;
		recv(conexion, &cod_op, sizeof(int), 0);
		caughtConIDs = recibir_CAUGHT_POKEMON(conexion, 0, 1);
		unCaughtPokemonTemporal = caughtConIDs->caughtPokemon;
		loQueVoyALoguear = "[GAMEBOY]Llegó un nuevo mensaje a la cola CAUGHT_POKEMON.Recibí un %d[MensajeCaughtPokemon],con ID: %d.";
		log_info(logger,loQueVoyALoguear,unCaughtPokemonTemporal->atrapar,caughtConIDs->IDmensaje);
		int ack = 1;
		send(conexion, &ack, sizeof(int), 0);
	}
}


int main(int argc, char *argv[]){
	int conexion;
	logger = iniciar_logger();
	//char* primerLog = "Soy game-boy";
	//log_info(logger, primerLog);
	config = leer_config();

	//Crear conexiones
	pthread_t hiloSuscripcion;

	if(!strcmp(argv[1],"SUSCRIPTOR")){
		puts("[Consola]: Se ha recibido un pedido de suscripción.");
			if(argv[2] == NULL){
				puts("ERROR: La cantidad mínima de parámetros es 4.Por favor intente de nuevo.");
			}else if(!strcmp(argv[2],"NEW_POKEMON")){
				if(atoi(argv[3]) <= 0){
					puts("ERROR:El tiempo que desea suscribirse es incorrecto. Por favor ingrese un tiempo mayor a 0.\n");
				}else{
					pthread_create(&hiloSuscripcion,NULL,(void*)suscribirNewPokemon,NULL);
					sleep(atoi(argv[3]));
					pthread_detach(hiloSuscripcion);
				}
			} else if(!strcmp(argv[2],"LOCALIZED_POKEMON")){
				if(atoi(argv[3]) <= 0){
					puts("ERROR:El tiempo que desea suscribirse es incorrecto. Por favor ingrese un tiempo mayor a 0.\n");
				}else{
					pthread_create(&hiloSuscripcion,NULL,(void*)suscribirLocalizedPokemon,NULL);
					sleep(atoi(argv[3]));
					pthread_detach(hiloSuscripcion);
				}
			} else if(!strcmp(argv[2],"GET_POKEMON")){
				if(atoi(argv[3]) <= 0){
					puts("ERROR:El tiempo que desea suscribirse es incorrecto. Por favor ingrese un tiempo mayor a 0.\n");
				}else{
					pthread_create(&hiloSuscripcion,NULL,(void*)suscribirGetPokemon,NULL);
					sleep(atoi(argv[3]));
					pthread_detach(hiloSuscripcion);
				}
			} else if(!strcmp(argv[2],"APPEARED_POKEMON")){
				if(atoi(argv[3]) <= 0){
					puts("ERROR:Ingrese el tiempo que desea suscribirse por favor (mayor a 0).\n");
				}else{
					pthread_create(&hiloSuscripcion,NULL,(void*)suscribirAppearedPokemon,NULL);
					sleep(atoi(argv[3]));
					pthread_detach(hiloSuscripcion);
				}
			} else if(!strcmp(argv[2],"CATCH_POKEMON")){
				if(atoi(argv[3]) <= 0){
					puts("ERROR:El tiempo que desea suscribirse es incorrecto. Por favor ingrese un tiempo mayor a 0.\n");
				}else{
					pthread_create(&hiloSuscripcion,NULL,(void*)suscribirCatchPokemon,NULL);
					sleep(atoi(argv[3]));
					pthread_detach(hiloSuscripcion);
				}
			}else if(!strcmp(argv[2],"CAUGHT_POKEMON")){
				if(atoi(argv[3]) <= 0){
					puts("ERROR:El tiempo que desea suscribirse es incorrecto. Por favor ingrese un tiempo mayor a 0.\n");
				}else{
					pthread_create(&hiloSuscripcion,NULL,(void*)suscribirCaughtPokemon,NULL);
					sleep(atoi(argv[3]));
					pthread_detach(hiloSuscripcion);
				}
			}else{
				puts("\nERROR:El nombre de la cola de mensajes ingresado es incorrecto. Por favor intente de nuevo.");
				puts("Formato: ./gameboy SUSCRIPTOR [COLA_DE_MENSAJES] [TIEMPO]");
				puts("Colas de mensajes posibles:");
				puts("NEW_POKEMON\nGET_POKEMON\nLOCALIZED_POKEMON\nAPPEARED_POKEMON\nCATCH_POKEMON\nCAUGHT_POKEMON\n");
			}
	}else if(!strcmp(argv[1],"BROKER")){
		puts("[Consola]: Se ha recibido un pedido para enviar un mensaje al proceso BROKER.");
		char* ipBroker = config_get_string_value(config,"IP_BROKER");
		char* puertoBroker = config_get_string_value(config,"PUERTO_BROKER");
		conexion = crear_conexion(ipBroker, puertoBroker);
		char* loQueVoyALoguear = "[GAMEBOY]Se creó la conexión con BROKER, con el valor %d.";
		log_info(logger,loQueVoyALoguear,conexion);

		if(argv[2] == NULL){
			puts("ERROR: La cantidad mínima de parámetros es 3.Por favor intente de nuevo.");
		}else if(!strcmp(argv[2],"NEW_POKEMON")){
				NewPokemon* newPokemon;
				if(argv[3] == NULL || argv[4] == NULL || argv[5] == NULL || argv[6] == NULL){
					puts("ERROR: La cantidad mínima de parámetros es 7.Por favor intente de nuevo.");
				}else if(string_length(argv[3]) <= 1 || !esUnNumero(argv[4]) || !esUnNumero(argv[5]) || !esUnNumero(argv[6])){
					puts("Argumento incorrecto. Por favor intente de nuevo.");
					puts("Formato: NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]");
				}else{
					newPokemon = parsearNewPokemon(argv[3], argv[4], argv[5], argv[6]);
					enviarNewPokemon(newPokemon, conexion,0);
				}
			}else if(!strcmp(argv[2],"GET_POKEMON")){
				GetPokemon* unGetPokemon;
				if(argv[3] == NULL){
					puts("ERROR: La cantidad mínima de parámetros es 4.Por favor intente de nuevo.");
				}else if(string_length(argv[3]) <= 1){
					puts("Argumento incorrecto. Por favor intente de nuevo.");
					puts("Formato: GET_POKEMON [POKEMON]");
				}else{
					unGetPokemon = parsearGetPokemon(argv[3]);
					enviarGetPokemon(unGetPokemon,conexion,0);
					int idMensaje;
					recv(conexion,&idMensaje,sizeof(int),0);
					printf("\nRecibí el ID=%d del mensaje GET_POKEMON anteriormente enviado.",idMensaje);
				}
			}else if(!strcmp(argv[2],"CATCH_POKEMON")){
				CatchPokemon* unCatchPokemon;
				if(argv[3] == NULL || argv[4] == NULL || argv[5] == NULL){
					puts("ERROR: La cantidad mínima de parámetros es 6. Por favor intente de nuevo.");
				}else if(string_length(argv[3]) <= 1 || !esUnNumero(argv[4]) || !esUnNumero(argv[5])){
					puts("Argumento incorrecto. Por favor intente de nuevo.");
					puts("Formato: CATCH_POKEMON [POKEMON] [POSX] [POSY]");
				}else{
					unCatchPokemon = parsearCatchPokemon(argv[3],argv[4],argv[5]);
					enviarCatchPokemon(unCatchPokemon,conexion,0);
					int idMensaje;
					recv(conexion,&idMensaje,sizeof(int),0);
					printf("\nRecibí el ID=%d del mensaje CATCH_POKEMON anteriormente enviado.",idMensaje);
				}
			}else if(!strcmp(argv[2],"CAUGHT_POKEMON")){
				CaughtPokemon* unCaughtPokemon;
				if(argv[3] == NULL || argv[4] == NULL){
					puts("ERROR: La cantidad mínima de parámetros es 5.Por favor intente de nuevo.");
				}else if(esUnNumero(argv[3]) || string_length(argv[4]) < 2){
					puts("Argumento incorrecto. Por favor intente de nuevo.");
					puts("CAUGHT_POKEMON [ID_MENSAJE_CORRELATIVO] [OK/FAIL]");
				}else{
					unCaughtPokemon = parsearCaughtPokemon(argv[4]);
					int id_correlativo = atoi(argv[3]);
					enviarCaughtPokemon(unCaughtPokemon,conexion,0,id_correlativo);
				}
			}else if(!strcmp(argv[2],"APPEARED_POKEMON")){
				AppearedPokemon* unAppearedPokemon;
				if(argv[3] == NULL || argv[4] == NULL || argv[5] == NULL || argv[6] == NULL){
					puts("ERROR: La cantidad mínima de parámetros es 7.Por favor intente de nuevo.");
				}else if(string_length(argv[3]) <= 1 || !esUnNumero(argv[4]) || !esUnNumero(argv[5])){
					puts("Argumento incorrecto. Por favor intente de nuevo.");
					puts("APPEARED_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE_CORRELATIVO]");
				}else{
					unAppearedPokemon = parsearAppearedPokemon(argv[3],argv[4],argv[5]);
					int id_correlativo = atoi(argv[6]);
					enviarAppearedPokemon(unAppearedPokemon,conexion,0,0);
				}
			}else{
				puts("\nERROR:El nombre de la cola de mensajes es incorrecto. Por favor intente de nuevo.");
				puts("Formato: ./gameboy BROKER [TIPO_MENSAJE] [ARGUMENTOS]*");
				puts("NEW_POKEMON\nGET_POKEMON\nAPPEARED_POKEMON\nCATCH_POKEMON\nCAUGHT_POKEMON\n");
			}
		}else if(!strcmp(argv[1],"TEAM")){
		puts("[Consola]: Se ha recibido un pedido para enviar un mensaje al proceso TEAM.");
		char* ipTeam = config_get_string_value(config,"IP_TEAM");
		char* puertoTeam = config_get_string_value(config,"PUERTO_TEAM");
		printf("\nEl ip y puerto de TEAM son: %s %s", ipTeam, puertoTeam);
		conexion = crear_conexion(ipTeam, puertoTeam);
		char* loQueVoyALoguear = "[GAMEBOY]Se creó la conexión con TEAM, con el valor %d.";
		log_info(logger,loQueVoyALoguear,conexion);

		if(argv[2] == NULL){
			puts("ERROR: La cantidad mínima de parámetros es 3.Por favor intente de nuevo.");
		}else if(!strcmp(argv[2],"APPEARED_POKEMON")){
				AppearedPokemon* unAppearedPokemon;
				if(argv[3] == NULL || argv[4] == NULL || argv[5] == NULL ){
					puts("ERROR: La cantidad mínima de parámetros es 6.Por favor intente de nuevo.");
				}else if(string_length(argv[3]) <= 1 || !esUnNumero(argv[4]) || !esUnNumero(argv[5])){
					puts("Argumento incorrecto. Por favor intente de nuevo.");
					puts("APPEARED_POKEMON [POKEMON] [POSX] [POSY]");
				}else{
					unAppearedPokemon = parsearAppearedPokemon(argv[3],argv[4],argv[5]);
					enviarAppearedPokemon(unAppearedPokemon,conexion,0,0);
				}
			}else if(!strcmp(argv[2],"LOCALIZED_POKEMON")){
				LocalizedPokemon* unLocalizedPokemon;
				int cantidadPares = atoi(argv[4]);
				char* pares[cantidadPares*2];

				if(argv[3] == NULL || argv[4] == NULL || pares == NULL){
					printf("ERROR: La cantidad mínima de parámetros es %d.Por favor intente de nuevo.",2 + cantidadPares*2);
				}else if(string_length(argv[3]) <= 1 || !esUnNumero(argv[4])){
					puts("Argumento incorrecto. Por favor intente de nuevo.");
					puts("Formato: [POKEMON] [CANT_PARES] [PARES]*");
				}else{
					for(int i=0; i<cantidadPares*2; i++){
						pares[i]= argv[5 + i];
					}
					unLocalizedPokemon = parsearLocalizedPokemon(argv[3],argv[4],pares);
					int correlativo=atoi(argv[5 + cantidadPares*2 ]);
					printf("\nMandé %d %s con ID=%d", unLocalizedPokemon->cantidadParesOrdenados, unLocalizedPokemon->nombre , correlativo);
					enviarLocalizedPokemon(unLocalizedPokemon,conexion,0,correlativo);
				}
			}else{
				puts("\nERROR:El nombre de la cola de mensajes ingresado es incorrecto para el proceso TEAM. Por favor intente de nuevo.");
				puts("Formato: ./gameboy TEAM [TIPO_MENSAJE] [ARGUMENTOS]*");
				puts("Mensajes posibles de enviar:");
				puts("APPEARED_POKEMON\nLOCALIZED_POKEMON\n");
			}
	}else if(!strcmp(argv[1],"GAMECARD")){
		puts("[Consola]: Se ha recibido un pedido para enviar un mensaje al proceso GAMECARD.");
		conexion = crear_conexion(ip, puerto);
		char* loQueVoyALoguear = "[GAMEBOY]Se creó la conexión con GAMECARD, con el valor %d.";
		log_info(logger,loQueVoyALoguear,conexion);

		if(argv[2] == NULL){
			puts("ERROR: La cantidad mínima de parámetros es 3.Por favor intente de nuevo.");
		}else if(!strcmp(argv[2],"NEW_POKEMON")){
				NewPokemon* newPokemon;
				if(argv[3] == NULL || argv[4] == NULL || argv[5] == NULL || argv[6] == NULL){
					puts("ERROR: La cantidad mínima de parámetros es 7.Por favor intente de nuevo.");
				}else if(string_length(argv[3]) <= 1 || !esUnNumero(argv[4]) || !esUnNumero(argv[5]) || !esUnNumero(argv[6])){
					puts("Argumento incorrecto. Por favor intente de nuevo.");
					puts("Formato: NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]");
				}else{
					newPokemon = parsearNewPokemon(argv[3], argv[4], argv[5], argv[6]);
					int correlativo = atoi(argv[7]);
					enviarNewPokemon(newPokemon, conexion,correlativo);
				}
			}else if(!strcmp(argv[2],"CATCH_POKEMON")){
				CatchPokemon* unCatchPokemon;
				if(argv[3] == NULL || argv[4] == NULL || argv[5] == NULL){
					puts("ERROR: La cantidad mínima de parámetros es 6.Por favor intente de nuevo.");
				}else if(string_length(argv[3]) <= 1 || !esUnNumero(argv[4]) || !esUnNumero(argv[5])){
					puts("Argumento incorrecto. Por favor intente de nuevo.");
					puts("Formato: CATCH_POKEMON [POKEMON] [POSX] [POSY]");
				}else{
					unCatchPokemon = parsearCatchPokemon(argv[3],argv[4],argv[5]);
					int correlativo = atoi(argv[6]);
					enviarCatchPokemon(unCatchPokemon,conexion,correlativo);
					int idMensaje;
					recv(conexion,&idMensaje,sizeof(int),0);
				}
			}else if(!strcmp(argv[2],"GET_POKEMON")){
				GetPokemon* unGetPokemon;
				if(argv[3] == NULL){
					puts("ERROR: La cantidad mínima de parámetros es 4.Por favor intente de nuevo.");
				}else if(string_length(argv[3]) <= 1){
					puts("Argumento incorrecto. Por favor intente de nuevo.");
					puts("Formato: GET_POKEMON [POKEMON]");
				}else{
					unGetPokemon = parsearGetPokemon(argv[3]);
					int correlativo = atoi(argv[4]);
					enviarGetPokemon(unGetPokemon,conexion,correlativo);
					int idMensaje;
					recv(conexion,&idMensaje,sizeof(int),0);
				}
			}else{
				puts("\nERROR:El nombre de la cola de mensajes ingresado es incorrecto para el proceso GAMECARD. Por favor intente de nuevo.");
				puts("Formato: ./gameboy GAMECARD [TIPO_MENSAJE] [ARGUMENTOS]*");
				puts("Mensajes posibles de enviar:");
				puts("NEW_POKEMON\nGET_POKEMON\nCATCH_POKEMON\n");
			}
	}else{
		puts("\nERROR:El comando ingresado es incorrecto. Por favor intente de nuevo.");
		puts("Formato: ./gameboy [PROCESO] [TIPO_MENSAJE] [ARGUMENTOS]*");
		puts("Procesos:");
		puts("BROKER\nTEAM\nGAMECARD\n");
	}

	terminar_programa(conexion, logger, config);
}


t_log* iniciar_logger(void){
	return log_create("game-boy.log", "Game Boy" , 1, LOG_LEVEL_INFO);
}

t_config* leer_config(void){
	return config_create("game-boy.config");
}

void terminar_programa(int conexion, t_log* logger, t_config* config){
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
