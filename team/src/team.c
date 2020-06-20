#include "team.h"

int main(int argc,char* argv[])
{

	sem_init(&sem[0], 0, 0);
	sem_init(&sem[1], 0, 0);
	sem_init(&sem[2], 0, 0);

	sem_init(&suscripciones, 0, 1);

	sem_init(&sem2[0], 0, 0);
	sem_init(&sem2[1], 0, 0);
	sem_init(&sem2[2], 0, 0);

	sem_init(&mensajesCaught, 0, 0);
	sem_init(&nuevosPokemons, 0, 0);

	int hiloCreado;
	algoritmoPlanificacion = "FIFO";

	entrenadores = list_create();
	objetivoGlobal = list_create();

	mensajesGetEnviados= list_create();
	mensajesCatchEnviados= list_create();
	mensajesRecibidos = list_create();
	nuevosPokemon = list_create();
	nuevosCaught = list_create();

	leer_config();
	obtener_objetivo_global();

	// Agrego Localized de Prueba //
	/*LocalizedPokemon * localized = malloc(sizeof(LocalizedPokemon));
	localized->tamanioNombrePokemon = strlen("Pikachu")+1;
	localized->nombre = "Pikachu";
	localized->cantidadParesOrdenados = 3;
	localized->paresOrdenados=list_create();

	LocalizedPokemonConIDs* localizedNuevo = malloc(sizeof(LocalizedPokemonConIDs));
	localizedNuevo -> IDcorrelativo = 0;
	localizedNuevo -> IDmensaje = 0;
	localizedNuevo->localizedPokemon = localized;


	CoordenadasXY* coordenadas1 = malloc(sizeof(CoordenadasXY));
	coordenadas1->posicionX = 2;
	coordenadas1->posicionY = 3;
	list_add(localizedNuevo ->localizedPokemon ->paresOrdenados, coordenadas1);


	CoordenadasXY* coordenadas2 = malloc(sizeof(CoordenadasXY));
	coordenadas2->posicionX = 1;
	coordenadas2->posicionY = 1;
	list_add(localizedNuevo ->localizedPokemon ->paresOrdenados, coordenadas2);


	CoordenadasXY* coordenadas3 = malloc(sizeof(CoordenadasXY));
	coordenadas3->posicionX = 3;
	coordenadas3->posicionY = 2;
	list_add(localizedNuevo ->localizedPokemon ->paresOrdenados, coordenadas3);


	char* nombre = "Pikachu";

	AppearedPokemon* appearedPokemon = malloc(sizeof(AppearedPokemon));
	appearedPokemon -> nombre = nombre;
	//appearedPokemon ->tamanioNombrePokemon = sizeof("Pikachu");

	AppearedPokemonConIDs* nuevoAppeared = malloc(sizeof(AppearedPokemonConIDs));
	nuevoAppeared -> IDcorrelativo = 0;
	nuevoAppeared -> IDmensaje = 0;
	nuevoAppeared -> appearedPokemon = appearedPokemon;

	if(descartar_appeared_no_deseados(nuevoAppeared)){
		printf("Funciona\n");
	}

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

*/
	generarConexiones(0);
	//abrirEscuchas();

	/*puts("\Voy a crear el hilo");
	pthread_t hiloConexionGameboy;
	pthread_create(&hiloConexionGameboy, NULL, (void*)noHayBroker, NULL);

	pthread_detach(hiloConexionGameboy);
	sleep(20);//puse esto para no poner semaforos porque paja
	for(int i=0; i<list_size(nuevosPokemon); i++){
		Pokemon* pokemon = list_get(nuevosPokemon, i);
		printf("Nombre %s, Posicion (%d,%d)", pokemon->nombre, pokemon->posicion.posicionX, pokemon->posicion.posicionY);
		puts("\n");
	}*/

	//pthread_join(hiloConexionGameboy,NULL);


	t_log* logger;

	logger = iniciar_logger();

	char* primerLog = "Soy un log";

	log_info(logger, primerLog);

}

//TODO
t_log* iniciar_logger(void)
{
	return log_create("team.log", "Log de team" , 1, LOG_LEVEL_INFO);

}

