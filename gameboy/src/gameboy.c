#include "game-boy.h"
#include <src/utilsGeneral.h>
#include <src/utilsBroker.h>

int main(int argc, char *argv[])
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

	config_set_value(config, "IP", "127.0.0.1");
	config_set_value(config, "PUERTO", "4444");
	config_save(config);


	//Rear conexion

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	conexion = crear_conexion(ip, puerto);

	char* loQueVoyALoguear = "\nSe creo la conexion con el valor %d";
	log_info(logger,loQueVoyALoguear,conexion);

	//Enviar mensaje
	if(!strcmp(argv[2],"NEW_POKEMON")){
		if(!strcmp(argv[1],"SUSCRIPTOR")){
			int cod_op = 8;
			int tiempo = atoi(argv[3]);
			printf("La suscripción a la cola %d durará %d segundos.\n",cod_op,tiempo);
			enviarSuscripcion(0,conexion,cod_op);
			char* loQueVoyALoguear = "\nMe suscribí a la cola de mensajes nro %d";
			log_info(logger,loQueVoyALoguear,cod_op);

			/*
			int tamanioListaAObtener = list_size(New_Pokemon);
			NewPokemon* unNew;
			for(int i=0;i<tamanioListaAObtener;i++){
				unNew = list_get(New_Pokemon,i);
			}
			*/
			int contador = 0;
			for(int i=0;i<tiempo;i++){
				i = contador;
				contador++;
				printf("%dseg\n",contador);
				sleep(1);
				if(contador == tiempo - 1){
					liberar_conexion(conexion);
				}
			}
		} else {
			puts("entre a new pokemon");
			NewPokemon* newPokemon;
			newPokemon = parsearNewPokemon(argv[3], argv[4], argv[5], argv[6]);
			enviarNewPokemon(newPokemon, conexion,0);
		}
	}

	if(!strcmp(argv[2],"GET_POKEMON")){
		if(!strcmp(argv[1],"SUSCRIPTOR")){
			int cod_op = 10;
			int tiempo = atoi(argv[3]);
			printf("La suscripción a la cola %d durará %d segundos.\n",cod_op,tiempo);
			enviarSuscripcion(0,conexion,cod_op);
			char* loQueVoyALoguear = "\nMe suscribí a la cola de mensajes nro %d";
			log_info(logger,loQueVoyALoguear,cod_op);

			int contador = 0;
			for(int i=0;i<tiempo;i++){
				i = contador;
				contador++;
				printf("%dseg\n",contador);
				sleep(1);
				if(contador == tiempo - 1){
					liberar_conexion(conexion);
				}
			}
		} else {
			GetPokemon* unGetPokemon;
			unGetPokemon = parsearGetPokemon(argv[3]);
			enviarGetPokemon(unGetPokemon,conexion,0);
			int idMensaje;
			recv(conexion,&idMensaje,sizeof(int),0);
		}
	}

	if(!strcmp(argv[2],"CATCH_POKEMON")){
		if(!strcmp(argv[1],"SUSCRIPTOR")){
			int cod_op = 12;
			int tiempo = atoi(argv[3]);
			printf("La suscripción a la cola %d durará %d segundos.\n",cod_op,tiempo);
			enviarSuscripcion(0,conexion,cod_op);
			char* loQueVoyALoguear = "\nMe suscribí a la cola de mensajes nro %d";
			log_info(logger,loQueVoyALoguear,cod_op);

			int contador = 0;
			for(int i=0;i<tiempo;i++){
				i = contador;
				contador++;
				printf("%dseg\n",contador);
				sleep(1);
				if(contador == tiempo - 1){
					liberar_conexion(conexion);
				}
			}
		} else {
			CatchPokemon* unCatchPokemon;
			unCatchPokemon = parsearCatchPokemon(argv[3],argv[4],argv[5]);
			enviarCatchPokemon(unCatchPokemon,conexion,0);
			int idMensaje;
			recv(conexion,&idMensaje,sizeof(int),0);
		}
	}

	if(!strcmp(argv[2],"CAUGHT_POKEMON")){
		if(!strcmp(argv[1],"SUSCRIPTOR")){
			int cod_op = 13;
			int tiempo = atoi(argv[3]);
			printf("La suscripción a la cola %d durará %d segundos.\n",cod_op,tiempo);
			enviarSuscripcion(0,conexion,cod_op);
			char* loQueVoyALoguear = "\nMe suscribí a la cola de mensajes nro %d";
			log_info(logger,loQueVoyALoguear,cod_op);

			int contador = 0;
			for(int i=0;i<tiempo;i++){
				i = contador;
				contador++;
				printf("%dseg\n",contador);
				sleep(1);
				if(contador == tiempo - 1){
					liberar_conexion(conexion);
				}
			}
		} else {
			CaughtPokemon* unCaughtPokemon;
			unCaughtPokemon = parsearCaughtPokemon(argv[4]);
			int id_correlativo = atoi(argv[3]);

			enviarCaughtPokemon(unCaughtPokemon,conexion,0,id_correlativo);
		}
	}

	if(!strcmp(argv[2],"APPEARED_POKEMON")){
		if(!strcmp(argv[1],"SUSCRIPTOR")){
			int cod_op = 11;
			int tiempo = atoi(argv[3]);
			printf("La suscripción a la cola %d durará %d segundos.\n",cod_op,tiempo);
			enviarSuscripcion(0,conexion,cod_op);
			char* loQueVoyALoguear = "\nMe suscribí a la cola de mensajes nro %d";
			log_info(logger,loQueVoyALoguear,cod_op);

			int contador = 0;
			for(int i=0;i<tiempo;i++){
				i = contador;
				contador++;
				printf("%dseg\n",contador);
				sleep(1);
				if(contador == tiempo - 1){
					liberar_conexion(conexion);
				}
			}
		} else {
			AppearedPokemon* unAppearedPokemon;
			unAppearedPokemon = parsearAppearedPokemon(argv[3],argv[4],argv[5]);
			int id_correlativo = atoi(argv[6]);

			enviarAppearedPokemon(unAppearedPokemon,conexion,0,id_correlativo);
		}

	}

	if(!strcmp(argv[2],"LOCALIZED_POKEMON")){
		if(!strcmp(argv[1],"SUSCRIPTOR")){
			int cod_op = 9;
			int tiempo = atoi(argv[3]);
			printf("La suscripción a la cola %d durará %d segundos.\n",cod_op,tiempo);
			enviarSuscripcion(0,conexion,cod_op);
			char* loQueVoyALoguear = "\nMe suscribí a la cola de mensajes nro %d";
			log_info(logger,loQueVoyALoguear,cod_op);

			int contador = 0;
			for(int i=0;i<tiempo;i++){
				i = contador;
				contador++;
				printf("%dseg\n",contador);
				sleep(1);
				if(contador == tiempo - 1){
					liberar_conexion(conexion);
				}
			}
		} else {
			LocalizedPokemon* unLocalizedPokemon;
			unLocalizedPokemon = parsearLocalizedPokemon(argv[3],argv[4],argv[5]); //no se como iria el 3er parametro de esta funcion
			int id_correlativo = 1;

			enviarLocalizedPokemon(unLocalizedPokemon,conexion,0,id_correlativo);
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
