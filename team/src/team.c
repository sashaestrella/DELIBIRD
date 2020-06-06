#include "team.h"


int main(int argc,char* argv[])
{

	condicionTeam[0]=0;
	condicionTeam[1]=0;
	condicionTeam[2]=0;
	pthread_mutex_init(&mutexTeam,NULL);
	pthread_cond_init(&cond[0], NULL);
	pthread_cond_init(&cond[1], NULL);
	pthread_cond_init(&cond[2], NULL);

	int hiloCreado;

	cargarDatosConexionConfig();
	//generarConexiones(0);
	//abrirEscuchas();

	entrenadores = list_create();
	objetivoGlobal = list_create();

	leer_config();

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
	pthread_mutex_lock(&mutexTeam);
	condicionTeam[0]=1;
	pthread_cond_signal(&cond[0]);
	pthread_mutex_unlock(&mutexTeam);
	//--------------------------------------------------------

	//------------Espera que se mueva para mostrarlo
	pthread_mutex_lock(&mutexTeam);
	if(condicionTeam[0]==1){
	pthread_cond_wait(&cond[0], &mutexTeam);
	printf("Entrenador1 nueva posicion (%d, %d)", entrenador->posicion.posicionX, entrenador->posicion.posicionY);
	}
	pthread_mutex_unlock(&mutexTeam);


	Entrenador* entrenador1 = malloc(sizeof(Entrenador));
	entrenador1 = list_get (entrenadores, 1);
	printf("\n\nEntrenador2 posicion (%d, %d)", entrenador1->posicion.posicionX, entrenador1->posicion.posicionY);
	puts("\nMi objetivo: ");

	for(i=0; i<list_size(entrenador1->objetivos);i++){
	 		printf("%s,", list_get(entrenador1->objetivos, i));
	}

	pthread_mutex_lock(&mutexTeam);
	condicionTeam[1]=1;
	pthread_cond_signal(&cond[1]);
	pthread_mutex_unlock(&mutexTeam);

	pthread_mutex_lock(&mutexTeam);
	if(condicionTeam[1]==1){
	pthread_cond_wait(&cond[1], &mutexTeam);
	printf("Entrenador2 nueva posicion (%d, %d)", entrenador1->posicion.posicionX, entrenador1->posicion.posicionY);
	}
	pthread_mutex_unlock(&mutexTeam);


	Entrenador* entrenador2 = malloc(sizeof(Entrenador));
	entrenador2 = list_get (entrenadores, 2);
	printf("\n\nEntrenador3 posicion (%d, %d)", entrenador2->posicion.posicionX, entrenador2->posicion.posicionY);
	puts("\nMi objetivo: ");
	for(i=0; i<list_size(entrenador2->objetivos);i++){
	 		printf("%s, ", list_get(entrenador2->objetivos, i));
	}
	pthread_mutex_lock(&mutexTeam);
	condicionTeam[2]=1;
	pthread_cond_signal(&cond[2]);
	pthread_mutex_unlock(&mutexTeam);


	pthread_mutex_lock(&mutexTeam);
	if(condicionTeam[2]==1){
	pthread_cond_wait(&cond[2], &mutexTeam);
	printf("Entrenador3 nueva posicion (%d, %d)", entrenador2->posicion.posicionX, entrenador2->posicion.posicionY);
	}
	pthread_mutex_unlock(&mutexTeam);


	puts("\n\nObjetivo Global");
	for(i=0; i<list_size(objetivoGlobal);i++){
		 		printf("%s,", list_get(objetivoGlobal, i));
		}

	}

	for(int j=0; j<list_size(entrenadores);j++)
			pthread_join(hiloEntrenador[j], NULL);


	/*-----------------------------------------------PARTE 2-------------------------------------------------------------*/



	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	char* primerLog = "Soy un log";

	log_info(logger, primerLog);



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
