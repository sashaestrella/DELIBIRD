#include "team.h"

int main(int argc,char* argv[])
{

	sem_init(&ejecutate[0], 0, 0);
	sem_init(&ejecutate[1], 0, 0);
	sem_init(&ejecutate[2], 0, 0);

	sem_init(&suscripciones, 0, 1);

	sem_init(&finEjecucion[0], 0, 0);
	sem_init(&finEjecucion[1], 0, 0);
	sem_init(&finEjecucion[2], 0, 0);

	sem_init(&confirmacion_caught[0], 0, 0);
	sem_init(&confirmacion_caught[1], 0, 0);
	sem_init(&confirmacion_caught[2], 0, 0);

	sem_init(&solucionar_deadlock[0], 0, 0);
	sem_init(&solucionar_deadlock[1], 0, 0);
	sem_init(&solucionar_deadlock[2], 0, 0);

	sem_init(&mensajesCaught, 0, 0);
	sem_init(&nuevosPokemons, 0, 0);
	sem_init(&aparicion_pokemon, 0, 0);

	pthread_mutex_init(&colaReady, NULL);
	pthread_mutex_init(&colaBlocked_new, NULL);
	pthread_mutex_init(&mutex_mapa, NULL);

	algoritmoPlanificacion = "FIFO";

	entrenadores = list_create();
	objetivoGlobal = list_create();

	mensajesGetEnviados= list_create();
	mensajesCatchEnviados= list_create();
	mensajesRecibidos = list_create();
	nuevosPokemon = list_create();
	nuevosCaught = list_create();

	blocked_new= list_create();
	blocked_caught = list_create();
	ready= list_create();
	ejecutando = list_create();
	terminados = list_create();
	deadlock = list_create();
	pokemones_en_mapa= list_create();

	leer_config();
	obtener_objetivo_global();

	pokemonesParaPrueba();

	Pokemon* a=malloc(sizeof(Pokemon));
	for(int i=0;i<list_size(pokemones_en_mapa);i++){
		a=list_get(pokemones_en_mapa,i);
		printf("%s", a->nombre);
	}

	// Agrego Localized de Prueba //
	LocalizedPokemon * localized = malloc(sizeof(LocalizedPokemon));
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
	}


	//generarConexiones(0);
	int i;
	puts("\n\nObjetivo Global");
	for(i=0; i<list_size(objetivoGlobal);i++){
		 		printf("%s,", list_get(objetivoGlobal, i));
		}
	Entrenador* entrenador =malloc(sizeof(Entrenador));
	for(i=0; i<list_size(entrenadores);i++){
	entrenador= list_get (entrenadores, i);
		 printf("\nEntrenador1 posicion (%d, %d)", entrenador->posicion.posicionX, entrenador->posicion.posicionY);
		 puts("\nMi objetivo: ");
		for(int j=0; j<list_size(entrenador->objetivos);j++){
			printf("%s,", list_get(entrenador->objetivos, j));
		}

	}

	puts("\Voy a crear el hilo");
	pthread_t hiloConexionGameboy;
	pthread_create(&hiloConexionGameboy, NULL, (void*)noHayBroker, NULL);

	pthread_detach(hiloConexionGameboy);

	list_add_all(blocked_new, entrenadores);


	pasar_a_ready_por_cercania();

	//list_clean(blocked_new);

	Entrenador* entrenadorReady1 = malloc(sizeof(Entrenador));


	puts("\n\nLa cola ready quedo:");
	for(i=0; i<list_size(ready);i++){
	entrenadorReady1 = list_get (ready, i);
		 printf("\nEntrenador %d posicion (%d, %d)",entrenadorReady1->ID, entrenadorReady1->posicion.posicionX, entrenadorReady1->posicion.posicionY);
		 puts("\nMi objetivo: ");
		for(int j=0; j<list_size(entrenadorReady1->objetivos);j++){
			printf("%s,", list_get(entrenadorReady1->objetivos, j));
		}

	}



	void planificadorFIFO(){

		Entrenador* entrenador = malloc(sizeof(Entrenador));
		puts("\n");

		while(list_size(terminados) != list_size(entrenadores)){

			if(list_size(terminados)+list_size(deadlock) != list_size(entrenadores)){


				if(list_size(ready)==0){
					sem_wait(&aparicion_pokemon);
					pasar_a_ready_por_cercania();
				}


			for(i=0; i<list_size(ready);i++){
				entrenadorReady1 = list_get (ready, i);
					 printf("\nReady %d)",entrenadorReady1->ID);
			}
				//pthread_mutex_lock(&colaReady);
				entrenador = list_remove(ready, 0);
				//pthread_mutex_unlock(&colaReady);

				if(list_size(ejecutando) == 0){
					list_add(ejecutando, entrenador);


					sem_post(&ejecutate[entrenador->ID - 1]);
					printf("\n\nDesperte a %d", entrenador->ID);

					printf("\nEspero a %d,", entrenador->ID);
					sem_wait(&finEjecucion[entrenador->ID - 1]);
					puts("\nMe llego");
				}


				if(list_size(blocked_new) != 0){

					bool buscarPOkemon(Pokemon* pokemon){
					return pokemon->IdEntrenadorQueLoVaAatrapar == 0;
					}
					if(list_any_satisfy(pokemones_en_mapa,(void*)buscarPOkemon) ==1){
						pasar_a_ready_por_cercania();

					}

				}

			}else{
				puts("\nEmṕieza a solucionar deadlock");
				entrenador = list_remove(deadlock,0);
				list_add(ejecutando, entrenador);

				sem_post(&solucionar_deadlock[entrenador->ID]);

				sem_wait(&solucionar_deadlock[entrenador->ID]);

			}


		}
	}



planificadorFIFO();

sleep(1);
Pokemon* pokemon = malloc(sizeof(Pokemon));


for(i=0; i<list_size(terminados);i++){
	entrenadorReady1 = list_get(terminados,i);
	for(int j=0; j< list_size(entrenadorReady1->pokemonesQueTiene);j++){
	pokemon = list_get(entrenadorReady1->pokemonesQueTiene,j);
	printf("\nSoy %d y agarre a %s ",entrenadorReady1->ID, pokemon->nombre);
	}
}
	//abrirEscuchas();


	/*
	sleep(20);//puse esto para no poner semaforos porque paja
	for(int i=0; i<list_size(nuevosPokemon); i++){
		Pokemon* pokemon = list_get(nuevosPokemon, i);
		printf("Nombre %s, Posicion (%d,%d)", pokemon->nombre, pokemon->posicion.posicionX, pokemon->posicion.posicionY);
		puts("\n");
	}*/

	//pthread_join(hiloConexionGameboy,NULL);


for(int j=0; j<list_size(entrenadores);j++)
	pthread_detach(hiloEntrenador[j]);
	//pthread_join(hiloEntrenador[j], NULL);

sleep(5);
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

