#include "game-boy.h"
#include <src/utilsGeneral.h>
#include <src/utilsBroker.h>

void recibirColaCaughtPokemon(int conexion){
	int tamanioLista;
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	CaughtPokemonConIDs* caughtConIDs;
	CaughtPokemon* unCaughtPokemonTemporal;

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
		printf("[gameboy] Recibi un %d[MensajeCaughtPokemon],con ID: %d\n",unCaughtPokemonTemporal->atrapar,caughtConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,caughtConIDs->IDmensaje);
	}

	free(caughtConIDs);
}

void recibirColaCatchPokemon(int conexion){
	int tamanioLista;
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	CatchPokemonConIDs* catchConIDs;
	CatchPokemon* unCatchPokemonTemporal;

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
		printf("[gameboy] Recibi un %s,con ID: %d\n",unCatchPokemonTemporal->nombre,catchConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,catchConIDs->IDmensaje);
	}

	free(catchConIDs);
}

void recibirColaAppearedPokemon(int conexion){
	int tamanioLista;
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	AppearedPokemonConIDs* appearedConIDs;
	AppearedPokemon* unAppearedPokemonTemporal;

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
		printf("[gameboy] Recibi un %s,con ID: %d\n",unAppearedPokemonTemporal->nombre,appearedConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,appearedConIDs->IDmensaje);
	}

	free(appearedConIDs);
}

void recibirColaGetPokemon(int conexion){
	int tamanioLista;
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	GetPokemonConIDs* getConIDs;
	GetPokemon* unGetPokemonTemporal;

	recv(conexion,&tamanioLista,sizeof(int),MSG_WAITALL);
	if(tamanioLista == 0){
		puts("\nNo puedo recibir la lista porque esta vacia");
		printf("Tamaño lista: %d",tamanioLista);
	}else {
		printf("El tamaño de la lista que voy a recibir es: %d\n",tamanioLista);
	}

	for(int i = 0; i<tamanioLista;i++){
		recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
		getConIDs = recibir_GET_POKEMON(conexion,&size,1);
		unGetPokemonTemporal = getConIDs->getPokemon;
		printf("[gameboy] Recibi un %s,con ID: %d\n",unGetPokemonTemporal->nombre,getConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,getConIDs->IDmensaje);
	}

	free(getConIDs);
}

void recibirColaLocalizedPokemon(int conexion){
	int tamanioLista;
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	LocalizedPokemonConIDs* localizedConIDs;
	LocalizedPokemon* unLocalizedPokemonTemporal;

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
		printf("[gameboy] Recibi un %s,con ID: %d\n",unLocalizedPokemonTemporal->nombre,localizedConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,localizedConIDs->IDmensaje);
	}

	free(localizedConIDs);
}

void recibirColaNewPokemon(int conexion,int tiempo){
	int size;
	int variableQueNoUsoxd;
	int ack = 1;
	int tamanioListaNP;
	NewPokemonConIDs* newConIDs;
	NewPokemon* unNewPokemonTemporal;
	clock_t t_ini, t_fin;
	t_ini = clock();

	recv(conexion,&tamanioListaNP,sizeof(int),MSG_WAITALL);
	if(tamanioListaNP == 0){
		puts("\nNo puedo recibir la lista porque esta vacia");
		printf("Tamaño lista: %d",tamanioListaNP);
	}else {
		printf("El tamaño de la lista que voy a recibir es: %d\n",tamanioListaNP);
	}

	int contador;
	for(int i = 0; i<tamanioListaNP;i++){
		recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
		newConIDs = recibir_NEW_POKEMON(conexion,&size,1);
		unNewPokemonTemporal = newConIDs->newPokemon;
		printf("[gameboy] Recibi un %s,%i,%i,%i con ID: %d\n",unNewPokemonTemporal->nombre,unNewPokemonTemporal->coordenadas.posicionX, unNewPokemonTemporal->coordenadas.posicionY, unNewPokemonTemporal->cantidad, newConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,newConIDs->IDmensaje);

		contador = i;
	}
	/*
	if(contador == tamanioListaNP - 1 && tiempo > 0){
		recv(conexion,&variableQueNoUsoxd,sizeof(int),MSG_WAITALL);
		newConIDs = recibir_NEW_POKEMON(conexion,&size,1);
		unNewPokemonTemporal = newConIDs->newPokemon;
		printf("[gameboy] Recibi un %s,%i,%i,%i con ID: %d\n",unNewPokemonTemporal->nombre,unNewPokemonTemporal->coordenadas.posicionX, unNewPokemonTemporal->coordenadas.posicionY, unNewPokemonTemporal->cantidad, newConIDs->IDmensaje);
		send(conexion,&ack,sizeof(int),0);
		printf("[gameboy]ACK=%d del mensaje %d fue enviado\n",ack,newConIDs->IDmensaje);
	}*/

	free(newConIDs);
 }


int main(int argc, char *argv[]){

	int conexion;
	char* ip;
	char* puerto;
	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	//Loggear "soy un log"
	//char* primerLog = "Soy game-boy";

	//log_info(logger, primerLog);

	config = leer_config();

	config_set_value(config, "IP_BROKER", "127.0.0.1");
	config_set_value(config, "PUERTO_BROKER", "4444");
	config_save(config);

	//Crear conexion con BROKER
	ip = config_get_string_value(config, "IP_BROKER");
	puerto = config_get_string_value(config, "PUERTO_BROKER");
	conexion = crear_conexion(ip, puerto);

	char* loQueVoyALoguear = "Se creó la conexión con [BROKER],el valor es; %d.";
	log_info(logger,loQueVoyALoguear,conexion);
	int idSuscriptorPosta;
	int cod_op;
	int tiempo;

	//Enviar suscripciones temporales
	if(!strcmp(argv[1],"SUSCRIPTOR")){
		if(!strcmp(argv[2],"NEW_POKEMON")){
			tiempo = atoi(argv[3]);
			cod_op = 8;

			enviarSuscripcion(0,conexion,cod_op);
			printf("Envie suscripcion para la cola de mensajes %d.\n",cod_op);
			log_info(logger,"Suscripto a la cola de mensajes New Pokemon correctamente.");
			recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
			printf("Recibi mi id como suscriptor: %d.\n",idSuscriptorPosta);

			recibirColaNewPokemon(conexion,tiempo);
			sleep(tiempo);

		}

		if(!strcmp(argv[2],"LOCALIZED_POKEMON")){
			tiempo = atoi(argv[3]);
			cod_op = 9;

			enviarSuscripcion(0,conexion,cod_op);
			printf("Envie suscripcion para la cola de mensajes %d.\n",cod_op);
			log_info(logger,"Suscripto a la cola de mensajes Localized Pokemon correctamente.");
			recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
			printf("Recibi mi id como suscriptor: %d.\n",idSuscriptorPosta);

			recibirColaLocalizedPokemon(conexion);
			sleep(tiempo);
		}
		if(!strcmp(argv[2],"GET_POKEMON")){
			tiempo = atoi(argv[3]);
			cod_op = 10;

			enviarSuscripcion(0,conexion,cod_op);
			printf("Envie suscripcion para la cola de mensajes %d.\n",cod_op);
			log_info(logger,"Suscripto a la cola de mensajes Get Pokemon correctamente.");
			recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
			printf("Recibi mi id como suscriptor: %d.\n",idSuscriptorPosta);

			recibirColaGetPokemon(conexion);
			sleep(tiempo);
		}
		if(!strcmp(argv[2],"APPEARED_POKEMON")){
			tiempo = atoi(argv[3]);
			cod_op = 11;

			enviarSuscripcion(0,conexion,cod_op);
			printf("Envie suscripcion para la cola de mensajes %d.\n",cod_op);
			log_info(logger,"Suscripto a la cola de mensajes Appeared Pokemon correctamente.");
			recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
			printf("Recibi mi id como suscriptor: %d.\n",idSuscriptorPosta);

			recibirColaAppearedPokemon(conexion);
			sleep(tiempo);
		}
		if(!strcmp(argv[2],"CATCH_POKEMON")){
			tiempo = atoi(argv[3]);
			cod_op = 12;

			enviarSuscripcion(0,conexion,cod_op);
			printf("Envie suscripcion para la cola de mensajes %d.\n",cod_op);
			log_info(logger,"Suscripto a la cola de mensajes Catch Pokemon correctamente.");
			recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
			printf("Recibi mi id como suscriptor: %d.\n",idSuscriptorPosta);

			recibirColaCatchPokemon(conexion);
			sleep(tiempo);
		}
		if(!strcmp(argv[2],"CAUGHT_POKEMON")){
			tiempo = atoi(argv[3]);
			cod_op = 13;

			enviarSuscripcion(0,conexion,cod_op);
			printf("Envie suscripcion para la cola de mensajes %d.\n",cod_op);
			log_info(logger,"Suscripto a la cola de mensajes Caught Pokemon correctamente.");
			recv(conexion,&idSuscriptorPosta,sizeof(int),MSG_WAITALL);
			printf("Recibi mi id como suscriptor: %d.\n",idSuscriptorPosta);

			recibirColaCaughtPokemon(conexion);
			sleep(tiempo);
		}

	}

	//Enviar mensajes
	if(!strcmp(argv[1],"BROKER")){
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
			printf("Recibi el ID=%d del mensaje GET_POKEMON.\n",idMensaje);
		}

		if(!strcmp(argv[2],"CATCH_POKEMON")){
			CatchPokemon* unCatchPokemon;
			unCatchPokemon = parsearCatchPokemon(argv[3],argv[4],argv[5]);
			enviarCatchPokemon(unCatchPokemon,conexion,0);
			int idMensaje;
			recv(conexion,&idMensaje,sizeof(int),0);
			printf("Recibi el ID=%d del mensaje CATCH_POKEMON.\n",idMensaje);
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
		if(!strcmp(argv[2],"APPEARED_POKEMON")){
			AppearedPokemon* unAppearedPokemon;
			unAppearedPokemon = parsearAppearedPokemon(argv[3],argv[4],argv[5]);
			int id_correlativo = atoi(argv[6]);

			enviarAppearedPokemon(unAppearedPokemon,conexion,0,id_correlativo);
		}
	}

	if(!strcmp(argv[1],"GAMECARD")){
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
			printf("Recibi el ID=%d del mensaje GET_POKEMON.\n",idMensaje);
		}

		if(!strcmp(argv[2],"CATCH_POKEMON")){
			CatchPokemon* unCatchPokemon;
			unCatchPokemon = parsearCatchPokemon(argv[3],argv[4],argv[5]);
			enviarCatchPokemon(unCatchPokemon,conexion,0);
			int idMensaje;
			recv(conexion,&idMensaje,sizeof(int),0);
			printf("Recibi el ID=%d del mensaje CATCH_POKEMON.\n",idMensaje);

		}
	}

	//Recibir mensaje

	//char* mensajeRecibido;

//	mensajeRecibido=recibir_mensaje(conexion);

	//Loguear mensaje recibido
/*
	log_info(logger, "Se recibio el mensaje: %s " ,mensajeRecibido);
*/


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
