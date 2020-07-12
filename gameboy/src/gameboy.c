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

	if(!strcmp(argv[1],"SUSCRIPTOR")){
		if(!strcmp(argv[2],"NEW_POKEMON")){
			int idSuscriptorPosta;
			int cod_op = 8;


			enviarSuscripcion(0,conexion,cod_op);
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

			free(newConIDs);
		}
		if(!strcmp(argv[2],"LOCALIZED_POKEMON")){
			int idSuscriptorPosta;
			int cod_op = 9;

			Suscriptor* unSuscriptor1 = malloc(sizeof(Suscriptor));
			unSuscriptor1->socketSuscriptor = 4;
			unSuscriptor1->IDsuscriptor = 0;

			enviarSuscripcion(unSuscriptor1->IDsuscriptor,conexion,cod_op);
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
		}
		if(!strcmp(argv[2],"GET_POKEMON")){
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
		}
		if(!strcmp(argv[2],"APPEARED_POKEMON")){
			int idSuscriptorPosta;
			int cod_op = 11;

			Suscriptor* unSuscriptor1 = malloc(sizeof(Suscriptor));
			unSuscriptor1->socketSuscriptor = 4;
			unSuscriptor1->IDsuscriptor = 0;

			enviarSuscripcion(unSuscriptor1->IDsuscriptor,conexion,cod_op);
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

			free(appearedConIDs);
		}
		if(!strcmp(argv[2],"CATCH_POKEMON")){
			int cod_op = 12;
			int idSuscriptorPosta;

			Suscriptor* unSuscriptor1 = malloc(sizeof(Suscriptor));
			unSuscriptor1->socketSuscriptor = 4;
			unSuscriptor1->IDsuscriptor = 0;

			enviarSuscripcion(unSuscriptor1->IDsuscriptor,conexion,cod_op);
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

			free(catchConIDs);
		}
		if(!strcmp(argv[2],"CAUGHT_POKEMON")){
		    int idSuscriptorPosta;
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
		}

	}

	if(!strcmp(argv[1],"BROKER")){
	//Enviar mensaje
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
	//Recibir mensaje

	//char* mensajeRecibido;

//	mensajeRecibido=recibir_mensaje(conexion);

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
