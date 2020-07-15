#include "game-boy.h"
#include<src/utilsGeneral.h>
#include<src/utilsBroker.h>

t_log* logger;
t_config* config;

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

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	printf("El ip y el puerto son: %s %s", ip, puerto);
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
		printf("[GAMEBOY]Recibí un %s,%i,%i,%i con ID: %d\n",unNewPokemonTemporal->nombre,unNewPokemonTemporal->coordenadas.posicionX, unNewPokemonTemporal->coordenadas.posicionY, unNewPokemonTemporal->cantidad, newConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[GAMEBOY]ACK=%d del mensaje %d fue enviado\n",ack,newConIDs->IDmensaje);
	}
	/*if(tamanioListaNP != 0){
		free(newConIDs);
	}*/
	while(1){
		int cod_op;
		loQueVoyALoguear = "[GAMEBOY]Llegó un nuevo mensaje a la cola NEW_POKEMON.";
		log_info(logger,loQueVoyALoguear);
		recv(conexion, &cod_op, sizeof(int), 0);
		newConIDs = recibir_NEW_POKEMON(conexion, 0, 1);
		unNewPokemonTemporal = newConIDs->newPokemon;
		printf("[GAMEBOY]Recibí un %s,%i,%i,%i con ID: %d\n",unNewPokemonTemporal->nombre,unNewPokemonTemporal->coordenadas.posicionX, unNewPokemonTemporal->coordenadas.posicionY, unNewPokemonTemporal->cantidad, newConIDs->IDmensaje);
		int ack = 1;
		send(conexion, &ack, sizeof(int), 0);
	}
}

void suscribirLocalizedPokemon(){
	int conexion;
	char* ip;
	char* puerto;
	int idSuscriptorPosta;
	int cod_op = 9;
	int tamanioLista;
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	LocalizedPokemonConIDs* localizedConIDs;
	LocalizedPokemon* unLocalizedPokemonTemporal;

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	printf("El ip y el puerto son: %s %s", ip, puerto);
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
		printf("[GAMEBOY]Recibí un %s,con ID: %d\n",unLocalizedPokemonTemporal->nombre,localizedConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[GAMEBOY]ACK=%d del mensaje %d fue enviado\n",ack,localizedConIDs->IDmensaje);
	}
	//free(localizedConIDs);
	while(1){
		int cod_op;
		loQueVoyALoguear = "[GAMEBOY]Llegó un nuevo mensaje a la cola LOCALIZED_POKEMON.";
		log_info(logger,loQueVoyALoguear);
		recv(conexion, &cod_op, sizeof(int), 0);
		localizedConIDs = recibir_LOCALIZED_POKEMON(conexion, 0, 1);
		unLocalizedPokemonTemporal = localizedConIDs->localizedPokemon;
		printf("[GAMEBOY]Recibí un %s,con ID: %d\n",unLocalizedPokemonTemporal->nombre,localizedConIDs->IDmensaje);
		int ack = 1;
		send(conexion, &ack, sizeof(int), 0);
	}
}

void suscribirGetPokemon(){
	int conexion;
	char* ip;
	char* puerto;
	int idSuscriptorPosta;
	int cod_op = 10;
	int tamanioLista;
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	GetPokemonConIDs* getConIDs;
	GetPokemon* unGetPokemonTemporal;

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	printf("El ip y el puerto son: %s %s", ip, puerto);
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
		loQueVoyALoguear = "[GAMEBOY]Llegó un nuevo mensaje a la cola GET_POKEMON.";
		log_info(logger,loQueVoyALoguear);
		recv(conexion, &cod_op, sizeof(int), 0);
		getConIDs = recibir_GET_POKEMON(conexion, 0, 1);
		unGetPokemonTemporal = getConIDs->getPokemon;
		printf("[GAMEBOY]Recibí un %s,con ID: %d\n",unGetPokemonTemporal->nombre,getConIDs->IDmensaje);
		int ack = 1;
		send(conexion, &ack, sizeof(int), 0);
	}
}

void suscribirAppearedPokemon(){
	int conexion;
	char* ip;
	char* puerto;
	int idSuscriptorPosta;
	int cod_op = 11;
	int tamanioLista;
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	AppearedPokemonConIDs* appearedConIDs;
	AppearedPokemon* unAppearedPokemonTemporal;

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	printf("El ip y el puerto son: %s %s", ip, puerto);
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
		printf("[GAMEBOY]Recibí un %s,con ID: %d\n",unAppearedPokemonTemporal->nombre,appearedConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[GAMEBOY]ACK=%d del mensaje %d fue enviado\n",ack,appearedConIDs->IDmensaje);
	}
	//free(appearedConIDs);
	while(1){
		int cod_op;
		loQueVoyALoguear = "[GAMEBOY]Llegó un nuevo mensaje a la cola APPEARED_POKEMON.";
		log_info(logger,loQueVoyALoguear);
		recv(conexion, &cod_op, sizeof(int), 0);
		appearedConIDs = recibir_APPEARED_POKEMON(conexion, 0, 1, 0); // aca no iria id correlativo
		unAppearedPokemonTemporal = appearedConIDs->appearedPokemon;
		printf("[GAMEBOY]Recibí un %s,con ID: %d\n",unAppearedPokemonTemporal->nombre,appearedConIDs->IDmensaje);
		int ack = 1;
		send(conexion, &ack, sizeof(int), 0);
	}
}

void suscribirCatchPokemon(){
	int conexion;
	char* ip;
	char* puerto;
	int cod_op = 12;
	int idSuscriptorPosta;
	int tamanioLista;
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	CatchPokemonConIDs* catchConIDs;
	CatchPokemon* unCatchPokemonTemporal;

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	printf("El ip y el puerto son: %s %s", ip, puerto);
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
		printf("[GAMEBOY]Recibí un %s,con ID: %d\n",unCatchPokemonTemporal->nombre,catchConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[GAMEBOY]ACK=%d del mensaje %d fue enviado\n",ack,catchConIDs->IDmensaje);
	}
	//free(catchConIDs);
	while(1){
		int cod_op;
		loQueVoyALoguear = "[GAMEBOY]Llegó un nuevo mensaje a la cola CATCH_POKEMON.";
		log_info(logger,loQueVoyALoguear);
		recv(conexion, &cod_op, sizeof(int), 0);
		catchConIDs = recibir_CATCH_POKEMON(conexion, 0, 1);
		unCatchPokemonTemporal = catchConIDs->catchPokemon;
		printf("[GAMEBOY]Recibí un %s,con ID: %d\n",unCatchPokemonTemporal->nombre,catchConIDs->IDmensaje);
		int ack = 1;
		send(conexion, &ack, sizeof(int), 0);
	}
}

void suscribirCaughtPokemon(){
	int conexion;
	char* ip;
	char* puerto;
	int idSuscriptorPosta;
	int cod_op = 13;
	int tamanioLista;
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	CaughtPokemonConIDs* caughtConIDs;
	CaughtPokemon* unCaughtPokemonTemporal;

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	printf("El ip y el puerto son: %s %s", ip, puerto);
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
		loQueVoyALoguear = "[GAMEBOY]Llegó un nuevo mensaje a la cola CAUGHT_POKEMON.";
		log_info(logger,loQueVoyALoguear);
		recv(conexion, &cod_op, sizeof(int), 0);
		caughtConIDs = recibir_CAUGHT_POKEMON(conexion, 0, 1);
		unCaughtPokemonTemporal = caughtConIDs->caughtPokemon;
		printf("[GAMEBOY]Recibí un %d[MensajeCaughtPokemon],con ID: %d\n",unCaughtPokemonTemporal->atrapar,caughtConIDs->IDmensaje);
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
		if(!strcmp(argv[2],"NEW_POKEMON")){
			pthread_create(&hiloSuscripcion,NULL,(void*)suscribirNewPokemon,NULL);
			sleep(atoi(argv[3]));
			pthread_detach(hiloSuscripcion);
		}
		if(!strcmp(argv[2],"LOCALIZED_POKEMON")){
			pthread_create(&hiloSuscripcion,NULL,(void*)suscribirLocalizedPokemon,NULL);
			sleep(atoi(argv[3]));
			pthread_detach(hiloSuscripcion);
		}
		if(!strcmp(argv[2],"GET_POKEMON")){
			pthread_create(&hiloSuscripcion,NULL,(void*)suscribirGetPokemon,NULL);
			sleep(atoi(argv[3]));
			pthread_detach(hiloSuscripcion);
		}
		if(!strcmp(argv[2],"APPEARED_POKEMON")){
			pthread_create(&hiloSuscripcion,NULL,(void*)suscribirAppearedPokemon,NULL);
			sleep(atoi(argv[3]));
			pthread_detach(hiloSuscripcion);
		}
		if(!strcmp(argv[2],"CATCH_POKEMON")){
			pthread_create(&hiloSuscripcion,NULL,(void*)suscribirCatchPokemon,NULL);
			sleep(atoi(argv[3]));
			pthread_detach(hiloSuscripcion);
		}
		if(!strcmp(argv[2],"CAUGHT_POKEMON")){
			pthread_create(&hiloSuscripcion,NULL,(void*)suscribirCaughtPokemon,NULL);
			sleep(atoi(argv[3]));
			pthread_detach(hiloSuscripcion);
		}
	}

	//Enviar mensajes
	if(!strcmp(argv[1],"BROKER")){
		char* ipBroker = config_get_string_value(config,"IP_BROKER");
		char* puertoBroker = config_get_string_value(config,"PUERTO_BROKER");
		conexion = crear_conexion(ipBroker, puertoBroker);
		char* loQueVoyALoguear = "[GAMEBOY]Se creó la conexión con BROKER, con el valor %d.";
		log_info(logger,loQueVoyALoguear,conexion);

		if(!strcmp(argv[2],"NEW_POKEMON")){
			NewPokemon* newPokemon;
			newPokemon = parsearNewPokemon(argv[3], argv[4], argv[5], argv[6]);
			enviarNewPokemon(newPokemon, conexion,0);
		}

		if(!strcmp(argv[2],"GET_POKEMON")){
			GetPokemon* unGetPokemon;
			unGetPokemon = parsearGetPokemon(argv[3]);
			enviarGetPokemon(unGetPokemon,conexion,0);
			int idMensaje;
			recv(conexion,&idMensaje,sizeof(int),0);
			printf("\nRecibí el ID=%d del mensaje GET_POKEMON anteriormente enviado.",idMensaje);
		}

		if(!strcmp(argv[2],"CATCH_POKEMON")){
			CatchPokemon* unCatchPokemon;
			unCatchPokemon = parsearCatchPokemon(argv[3],argv[4],argv[5]);
			enviarCatchPokemon(unCatchPokemon,conexion,0);
			int idMensaje;
			recv(conexion,&idMensaje,sizeof(int),0);
			printf("\nRecibí el ID=%d del mensaje CATCH_POKEMON anteriormente enviado.",idMensaje);
		}

		if(!strcmp(argv[2],"CAUGHT_POKEMON")){
			CaughtPokemon* unCaughtPokemon;
			unCaughtPokemon = parsearCaughtPokemon(argv[4]);
			int id_correlativo = atoi(argv[3]);
			enviarCaughtPokemon(unCaughtPokemon,conexion,0,id_correlativo);
		}

		if(!strcmp(argv[2],"APPEARED_POKEMON")){
			AppearedPokemon* unAppearedPokemon;
			unAppearedPokemon = parsearAppearedPokemon(argv[3],argv[4],argv[5]);
			int id_correlativo = atoi(argv[6]);
			enviarAppearedPokemon(unAppearedPokemon,conexion,0,id_correlativo);
		}
	}

	if(!strcmp(argv[1],"TEAM")){
		char* ipTeam = config_get_string_value(config,"IP_TEAM");
		char* puertoTeam = config_get_string_value(config,"PUERTO_TEAM");
		printf("\nEl ip y puerto de TEAM son: %s %s", ipTeam, puertoTeam);
		conexion = crear_conexion(ipTeam, puertoTeam);
		char* loQueVoyALoguear = "[GAMEBOY]Se creó la conexión con TEAM, con el valor %d.";
		log_info(logger,loQueVoyALoguear,conexion);

		if(!strcmp(argv[2],"APPEARED_POKEMON")){
			AppearedPokemon* unAppearedPokemon;
			unAppearedPokemon = parsearAppearedPokemon(argv[3],argv[4],argv[5]);
			enviarAppearedPokemon(unAppearedPokemon,conexion,0,0);
		}

		if(!strcmp(argv[2],"LOCALIZED_POKEMON")){
			LocalizedPokemon* unLocalizedPokemon;
			int cantidadPares = atoi(argv[4]);
			char* pares[cantidadPares*2];

			for(int i=0; i<cantidadPares*2; i++){
				pares[i]= argv[5 + i];
			}
			unLocalizedPokemon = parsearLocalizedPokemon(argv[3],argv[4],pares);
			int correlativo=atoi(argv[5 + cantidadPares*2 ]);
			printf("\nMandé %d %s con ID=%d", unLocalizedPokemon->cantidadParesOrdenados, unLocalizedPokemon->nombre , correlativo);
			enviarLocalizedPokemon(unLocalizedPokemon,conexion,0,correlativo);
		}
	}

	if(!strcmp(argv[1],"GAMECARD")){
		conexion = crear_conexion(ip, puerto);
		char* loQueVoyALoguear = "[GAMEBOY]Se creó la conexión con GAMECARD, con el valor %d.";
		log_info(logger,loQueVoyALoguear,conexion);
		if(!strcmp(argv[2],"NEW_POKEMON")){
			NewPokemon* newPokemon;
			newPokemon = parsearNewPokemon(argv[3], argv[4], argv[5], argv[6]);
			int correlativo = atoi(argv[7]);
			enviarNewPokemon(newPokemon, conexion,correlativo);
		}

		if(!strcmp(argv[2],"CATCH_POKEMON")){
			CatchPokemon* unCatchPokemon;
			unCatchPokemon = parsearCatchPokemon(argv[3],argv[4],argv[5]);
			int correlativo = atoi(argv[6]);
			enviarCatchPokemon(unCatchPokemon,conexion,correlativo);
			int idMensaje;
			recv(conexion,&idMensaje,sizeof(int),0);
		}

		if(!strcmp(argv[2],"GET_POKEMON")){
			GetPokemon* unGetPokemon;
			unGetPokemon = parsearGetPokemon(argv[3]);
			int correlativo = atoi(argv[4]);
			enviarGetPokemon(unGetPokemon,conexion,correlativo);
			int idMensaje;
			recv(conexion,&idMensaje,sizeof(int),0);
		}

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
