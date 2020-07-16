#include "game-boy.h"
#include <src/utilsGeneral.h>
#include <src/utilsBroker.h>
t_log* logger;
t_config* config;

void suscribirNewPokemon(){
	int conexion;
	char* ip;
	char* puerto;
	ip = config_get_string_value(config, "IP_BROKER");
	puerto = config_get_string_value(config, "PUERTO_BROKER");
	printf("el ip y el puerto son: %s %s", ip, puerto);
	conexion = crear_conexion(ip, puerto);
	char* loQueVoyALoguear = "\n[GAMEBOY]Se creo la conexion con broker con el valor %d";
	log_info(logger,loQueVoyALoguear,conexion);


	int idSuscriptorPosta;
	int cod_op = 8;


	enviarSuscripcion(0,conexion,cod_op);
	loQueVoyALoguear = "\n[GAMEBOY]Me suscribi a la cola NEW_POKEMON";
	log_info(logger,loQueVoyALoguear);
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
		printf("[gameboy] Recibi un %s,%i,%i,%i con ID: %d\n",unNewPokemonTemporal->nombre,unNewPokemonTemporal->coordenadas.posicionX, unNewPokemonTemporal->coordenadas.posicionY, unNewPokemonTemporal->cantidad, newConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,newConIDs->IDmensaje);
	}

	/*if(tamanioListaNP != 0){
		free(newConIDs);
	}*/
	while(1){
		int cod_op;
		recv(conexion, &cod_op, sizeof(int), 0);
		newConIDs = recibir_NEW_POKEMON(conexion, 0, 1);
		unNewPokemonTemporal = newConIDs->newPokemon;
		printf("[gameboy] Recibi un %s,%i,%i,%i con ID: %d\n",unNewPokemonTemporal->nombre,unNewPokemonTemporal->coordenadas.posicionX, unNewPokemonTemporal->coordenadas.posicionY, unNewPokemonTemporal->cantidad, newConIDs->IDmensaje);
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
	conexion = crear_conexion(ip, puerto);
	char* loQueVoyALoguear = "\n[GAMEBOY]Se creo la conexion con broker con el valor %d";
	log_info(logger,loQueVoyALoguear,conexion);

	int idSuscriptorPosta;
	int cod_op = 9;
	Suscriptor* unSuscriptor1 = malloc(sizeof(Suscriptor));
	unSuscriptor1->IDsuscriptor = 0;
	enviarSuscripcion(unSuscriptor1->IDsuscriptor,conexion,cod_op);
	printf("Envie suscripcion para la cola de mensajes %d",cod_op);
	loQueVoyALoguear = "\n[GAMEBOY]Me suscribi a la cola LOCALIZED_POKEMON";
	log_info(logger,loQueVoyALoguear);
	recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
	printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptorPosta);

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

	//free(localizedConIDs);
	while(1){
		int cod_op;
		recv(conexion, &cod_op, sizeof(int), 0);
		localizedConIDs = recibir_LOCALIZED_POKEMON(conexion, 0, 1);
		unLocalizedPokemonTemporal = localizedConIDs->localizedPokemon;
		printf("[gameboy] Recibi un %s,con ID: %d\n",unLocalizedPokemonTemporal->nombre,localizedConIDs->IDmensaje);
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
	conexion = crear_conexion(ip, puerto);
	char* loQueVoyALoguear = "\n[GAMEBOY]Se creo la conexion con broker con el valor %d";
	log_info(logger,loQueVoyALoguear,conexion);

	int idSuscriptorPosta;
	int cod_op = 10;

	Suscriptor* unSuscriptor1 = malloc(sizeof(Suscriptor));
	unSuscriptor1->IDsuscriptor = 0;

	enviarSuscripcion(unSuscriptor1->IDsuscriptor,conexion,cod_op);
	loQueVoyALoguear = "\n[GAMEBOY]Me suscribi a la cola GET_POKEMON";
	log_info(logger,loQueVoyALoguear);
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

	//free(getConIDs);

	while(1){
		int cod_op;
		recv(conexion, &cod_op, sizeof(int), 0);
		getConIDs = recibir_GET_POKEMON(conexion, 0, 1);
		unGetPokemonTemporal = getConIDs->getPokemon;
		printf("[gameboy] Recibi un %s,con ID: %d\n",unGetPokemonTemporal->nombre,getConIDs->IDmensaje);
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
	conexion = crear_conexion(ip, puerto);
	char* loQueVoyALoguear = "\n[GAMEBOY]Se creo la conexion con broker con el valor %d";
	log_info(logger,loQueVoyALoguear,conexion);

	int idSuscriptorPosta;
	int cod_op = 11;

	Suscriptor* unSuscriptor1 = malloc(sizeof(Suscriptor));
	unSuscriptor1->IDsuscriptor = 0;

	enviarSuscripcion(unSuscriptor1->IDsuscriptor,conexion,cod_op);
	loQueVoyALoguear = "\n[GAMEBOY]Me suscribi a la cola APPEARED_POKEMON";
	log_info(logger,loQueVoyALoguear);
	printf("Envie suscripcion para la cola de mensajes %d",cod_op);
	recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
	printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptorPosta);

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

	//free(appearedConIDs);
	while(1){
		int cod_op;
		recv(conexion, &cod_op, sizeof(int), 0);
		appearedConIDs = recibir_APPEARED_POKEMON(conexion, 0, 1, 0); // aca no iria id correlativo
		unAppearedPokemonTemporal = appearedConIDs->appearedPokemon;
		printf("[gameboy] Recibi un %s,con ID: %d\n",unAppearedPokemonTemporal->nombre,appearedConIDs->IDmensaje);
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
	conexion = crear_conexion(ip, puerto);
	char* loQueVoyALoguear = "\n[GAMEBOY]Se creo la conexion con broker con el valor %d";
	log_info(logger,loQueVoyALoguear,conexion);

	int cod_op = 12;
	int idSuscriptorPosta;

	Suscriptor* unSuscriptor1 = malloc(sizeof(Suscriptor));
	unSuscriptor1->IDsuscriptor = 0;

	enviarSuscripcion(unSuscriptor1->IDsuscriptor,conexion,cod_op);
	loQueVoyALoguear = "\n[GAMEBOY]Me suscribi a la cola CATCH_POKEMON";
	log_info(logger,loQueVoyALoguear);
	printf("Envie suscripcion para la cola de mensajes %d",cod_op);
	recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
	printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptorPosta);

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

	//free(catchConIDs);
	while(1){
		int cod_op;
		recv(conexion, &cod_op, sizeof(int), 0);
		catchConIDs = recibir_CATCH_POKEMON(conexion, 0, 1);
		unCatchPokemonTemporal = catchConIDs->catchPokemon;
		printf("[gameboy] Recibi un %s,con ID: %d\n",unCatchPokemonTemporal->nombre,catchConIDs->IDmensaje);
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
	conexion = crear_conexion(ip, puerto);
	char* loQueVoyALoguear = "\n[GAMEBOY]Se creo la conexion con broker con el valor %d";
	log_info(logger,loQueVoyALoguear,conexion);

    int idSuscriptorPosta;
	int cod_op = 13;

	Suscriptor* unSuscriptor1 = malloc(sizeof(Suscriptor));
	unSuscriptor1->IDsuscriptor = 0;

	enviarSuscripcion(unSuscriptor1->IDsuscriptor,conexion,cod_op);
	loQueVoyALoguear = "\n[GAMEBOY]Me suscribi a la cola CAUGHT_POKEMON";
	log_info(logger,loQueVoyALoguear);
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

	//free(caughtConIDs);

	while(1){
		int cod_op;
		recv(conexion, &cod_op, sizeof(int), 0);
		caughtConIDs = recibir_CAUGHT_POKEMON(conexion, 0, 1);
		unCaughtPokemonTemporal = caughtConIDs->caughtPokemon;
		printf("[gameboy] Recibi un %d[MensajeCaughtPokemon],con ID: %d\n",unCaughtPokemonTemporal->atrapar,caughtConIDs->IDmensaje);
		int ack = 1;
		send(conexion, &ack, sizeof(int), 0);

	}
}


int main(int argc, char *argv[])
{

	int conexion;



	logger = iniciar_logger();

	char* primerLog = "Soy game-boy";

	log_info(logger, primerLog);

	config = leer_config();

	//Rear conexion

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

	if(!strcmp(argv[1],"BROKER")){
		//Enviar mensaje
		char* ipBroker = config_get_string_value(config,"IP_BROKER");
		char* puertoBroker = config_get_string_value(config,"PUERTO_BROKER");
		conexion = crear_conexion(ipBroker, puertoBroker);
		char* loQueVoyALoguear = "\n[GAMEBOY]Se creo la conexion con broker con el valor %d";
		log_info(logger,loQueVoyALoguear,conexion);

		if(!strcmp(argv[2],"NEW_POKEMON")){
			puts("entre a new pokemon");
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
		}

		if(!strcmp(argv[2],"CATCH_POKEMON")){
			CatchPokemon* unCatchPokemon;
			unCatchPokemon = parsearCatchPokemon(argv[3],argv[4],argv[5]);
			enviarCatchPokemon(unCatchPokemon,conexion,0);
			int idMensaje;
			recv(conexion,&idMensaje,sizeof(int),0);
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
		//Enviar mensaje
			char* ipTeam = config_get_string_value(config,"IP_TEAM");
			char* puertoTeam = config_get_string_value(config,"PUERTO_TEAM");
			printf("\nip y puerto de team %s %s", ipTeam, puertoTeam);
			conexion = crear_conexion(ipTeam, puertoTeam);
			char* loQueVoyALoguear = "\n[GAMEBOY]Se creo la conexion con TEAM con el valor %d";
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
				printf("\nMande %d %s con id %d", unLocalizedPokemon->cantidadParesOrdenados, unLocalizedPokemon->nombre , correlativo);
				enviarLocalizedPokemon(unLocalizedPokemon,conexion,0,correlativo);
			}

		}

	if(!strcmp(argv[1],"GAMECARD")){
		//Enviar mensaje
		conexion = crear_conexion(ip, puerto);
		char* loQueVoyALoguear = "\n[GAMEBOY]Se creo la conexion con GAMECARD con el valor %d";
		log_info(logger,loQueVoyALoguear,conexion);
		if(!strcmp(argv[2],"NEW_POKEMON")){
				puts("entre a new pokemon");
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
}
