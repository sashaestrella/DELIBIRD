#include "team.h"

int main(int argc,char* argv[])
{

	sem_init(&sem[0], 0, 0);
	sem_init(&sem[1], 0, 0);
	sem_init(&sem[2], 0, 0);


	sem_init(&sem2[0], 0, 0);
	sem_init(&sem2[1], 0, 0);
	sem_init(&sem2[2], 0, 0);

	int hiloCreado;
	algoritmoPlanificacion = "FIFO";

	entrenadores = list_create();
	objetivoGlobal = list_create();

	mensajesGetEnviados= list_create();
	mensajesCatchEnviados= list_create();

	GetPokemonConIDs* mensajeGet = malloc(sizeof(GetPokemonConIDs));

	GetPokemon* getPokemon= malloc(sizeof(GetPokemon));
	getPokemon->nombre = "Pikachu";
	getPokemon->tamanioNombrePokemon=strlen("Pikachu") +1;

	mensajeGet->IDmensaje =1;
	mensajeGet->getPokemon = getPokemon;

	list_add(mensajesGetEnviados, mensajeGet);

	CatchPokemonConIDs* mensajeCatch = malloc(sizeof(CatchPokemonConIDs));

	CatchPokemon* CatchPokemon= malloc(sizeof(CatchPokemon));
	CatchPokemon->nombre = "Pikachu";
	CatchPokemon->tamanioNombrePokemon=strlen("Pikachu") +1;

	mensajeCatch->IDmensaje =1;
	mensajeCatch->catchPokemon = CatchPokemon;

	list_add(mensajesGetEnviados, mensajeGet);
	list_add(mensajesCatchEnviados, mensajeCatch);
	leer_config();

	obtener_objetivo_global();


	hiloEntrenador = malloc(list_size(entrenadores) * sizeof(pthread_t));

	for(int j=0; j<list_size(entrenadores);j++){

		pthread_create(&hiloEntrenador[j],NULL, planificar,list_get(entrenadores, j));
		hiloCreado = j;
	}

	if(hiloCreado ==2){
	sleep(1);
	int i;
	Entrenador* entrenador = malloc(sizeof(Entrenador));
	entrenador = list_get (entrenadores, 0);
	 printf("\nEntrenador1 posicion (%d, %d)", entrenador->posicion.posicionX, entrenador->posicion.posicionY);
	 puts("\nMi objetivo: ");
	for(i=0; i<list_size(entrenador->objetivos);i++){
		printf("%s,", list_get(entrenador->objetivos, i));
	}
	//-------------Le dice al hilo del entrenador que se mueva

	sem_post(&sem[0]);
	//--------------------------------------------------------

	//------------Espera que se mueva para mostrarlo


	sem_wait(&sem2[0]);

	printf("Entrenador1 nueva posicion (%d, %d)", entrenador->posicion.posicionX, entrenador->posicion.posicionY);


	Entrenador* entrenador1 = malloc(sizeof(Entrenador));
	entrenador1 = list_get (entrenadores, 1);
	printf("\n\nEntrenador2 posicion (%d, %d)", entrenador1->posicion.posicionX, entrenador1->posicion.posicionY);
	puts("\nMi objetivo: ");

	for(i=0; i<list_size(entrenador1->objetivos);i++){
	 		printf("%s,", list_get(entrenador1->objetivos, i));
	}

	sem_post(&sem[1]);

	sem_wait(&sem2[1]);
	printf("Entrenador2 nueva posicion (%d, %d)", entrenador1->posicion.posicionX, entrenador1->posicion.posicionY);



	Entrenador* entrenador2 = malloc(sizeof(Entrenador));
	entrenador2 = list_get (entrenadores, 2);
	printf("\n\nEntrenador3 posicion (%d, %d)", entrenador2->posicion.posicionX, entrenador2->posicion.posicionY);
	puts("\nMi objetivo: ");
	for(i=0; i<list_size(entrenador2->objetivos);i++){
	 		printf("%s, ", list_get(entrenador2->objetivos, i));
	}

	sem_post(&sem[2]);

	sem_wait(&sem2[2]);
	printf("Entrenador3 nueva posicion (%d, %d)", entrenador2->posicion.posicionX, entrenador2->posicion.posicionY);



	puts("\n\nObjetivo Global");
	for(i=0; i<list_size(objetivoGlobal);i++){
		 		printf("%s,", list_get(objetivoGlobal, i));
		}

	}

	for(int j=0; j<list_size(entrenadores);j++)
			pthread_join(hiloEntrenador[j], NULL);



	generarConexiones(0);
	enviar_getPokemon(getPokemon);
	//abrirEscuchas();



	/*-----------------------------------------------PARTE 2-------------------------------------------------------------*/



	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	char* primerLog = "Soy un log";

	log_info(logger, primerLog);

	sleep(3);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	//antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	//crear conexion


	//conexion = crear_conexion(ip, puerto);

	//printf( "\nSe creo la conexion con el valor %d \n", conexion);

	//enviar mensaje

	/*int tamanio_maximo_mensaje = 256;

	char* mensajeAEnviar = malloc(tamanio_maximo_mensaje);

	printf("\nIngrese el mensaje:");

	fgets(mensajeAEnviar,tamanio_maximo_mensaje,stdin);

	printf("Se va a enviar el mensaje %s", mensajeAEnviar);

	enviar_mensaje(mensajeAEnviar, conexion); //Envia el mensaje

	//recibir mensaje

	char* mensajeRecibido;

	//mensajeRecibido=recibir_mensaje(conexion);*/


	//loguear mensaje recibido

	//log_info(logger, "Se recibio el mensaje: %s " ,mensajeRecibido);

	//terminar_programa(conexion, logger, config);
}

//TODO
t_log* iniciar_logger(void)
{
	return log_create("team.log", "Log de team" , 1, LOG_LEVEL_INFO);

}

//TODO


//TODO
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);

	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
}
