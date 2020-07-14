#include "team.h"

int main(int argc,char* argv[])
{
	estimacionInicial = 1;
	alpha = 0.5;
	logger = iniciar_logger();



	sem_init (&agregar_ready,0,0);

	ciclos_totales=0;

	algoritmoPlanificacion = "FIFO";

	entrenadores = list_create();
	objetivoGlobal = list_create();

	mensajesGetEnviados= list_create();
	mensajesCatchEnviados= list_create();
	mensajesRecibidos = list_create();
	mapa_auxiliar= list_create();
	nuevosCaught = list_create();

	blocked_new= list_create();
	blocked_caught = list_create();
	ready= list_create();
	ejecutando = list_create();
	terminados = list_create();
	deadlock = list_create();
	pokemones_en_mapa= list_create();

	readyAnterior = list_create();
	leer_config();

	inicializarSemaforos();


	quantum =3;


	obtener_objetivo_global();

	//aca mandar gets

	enviar_gets();

	//pokemonesParaPrueba(); //aca armo con los localized

	sem_init(&pruebaLocalized, 0, 0);


	ciclos_entrenadores = malloc(sizeof(int)*list_size(entrenadores));

	for(int i=0;i<list_size(entrenadores);i++)
	ciclos_entrenadores[i]=malloc(sizeof(int));


	contadorCiclosPorEntrenador = malloc(sizeof(int)*list_size(entrenadores));

	for(int i=0;i<list_size(entrenadores);i++)
	contadorCiclosPorEntrenador [i]=malloc(sizeof(int));



	hiloEntrenador = malloc(list_size(entrenadores) * sizeof(pthread_t));

	for(int j=0; j<list_size(entrenadores);j++){
		pthread_create(&hiloEntrenador[j],NULL, flujoEntrenador,list_get(entrenadores, j));
	}

	generarConexiones(0);
	sleep(2);

	puts("\Voy a crear el hilo");
	/*pthread_t hiloConexionGameboy;
	pthread_create(&hiloConexionGameboy, NULL, (void*)noHayBroker, NULL);

	pthread_detach(hiloConexionGameboy);*/

	//sem_wait(&pruebaLocalized);
	//sem_wait(&pruebaLocalized);
	//sem_wait(&pruebaLocalized);
	//sem_wait(&pruebaLocalized);


	int i;
		puts("\n\nPokemones en Mapa");
		Pokemon* pokemon1 = malloc(sizeof(Pokemon));
		for(i=0; i<list_size(pokemones_en_mapa);i++){
			pokemon1 = list_get(pokemones_en_mapa, i);
			printf("%s, (%d, %d)", pokemon1->nombre, pokemon1->posicion.posicionX, pokemon1->posicion.posicionY );
			}

		puts("\n\nMapa auxiliar");
			for(i=0; i<list_size(mapa_auxiliar);i++){
				pokemon1 = list_get(mapa_auxiliar, i);
				 printf("%s, (%d, %d)", pokemon1->nombre, pokemon1->posicion.posicionX, pokemon1->posicion.posicionY );
				}

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


	list_add_all(blocked_new, entrenadores);


	pasar_a_ready_por_cercania();
	list_add_all(readyAnterior, ready);


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


		switch(algoritmoAUtilizar(algoritmoPlanificacion)){

			case FIFO:
				planificadorFIFO_RR();
				break;
			case RR:
				planificadorFIFO_RR();
				break;
			case SJF_CD:
				planificadorSJF_SD();
				break;
			case SJF_SD:
				planificadorSJF_SD();
				break;
		}








Pokemon* pokemon = malloc(sizeof(Pokemon));


for(i=0; i<list_size(terminados);i++){
	entrenadorReady1 = list_get(terminados,i);
	for(int j=0; j< list_size(entrenadorReady1->pokemonesQueTiene);j++){
	pokemon = list_get(entrenadorReady1->pokemonesQueTiene,j);
	printf("\nSoy %d y agarre a %s ",entrenadorReady1->ID, pokemon->nombre);
	}
}
	//abrirEscuchas();




for(int j=0; j<list_size(entrenadores);j++)
	pthread_join(hiloEntrenador[j], NULL);
	//pthread_detach(hiloEntrenador[j]);

log_info(logger, "Ciclos totales %d.", ciclos_totales);
for(int i=0; i<list_size(entrenadores);i++){
	entrenador=list_get(entrenadores,i);
//log_info(logger, "Ciclos entrenador %d, %d.",entrenador->ID, ciclos_entrenadores[entrenador->ID-1]);
}
}

//TODO
t_log* iniciar_logger(void)
{
	return log_create("team.log", "Log de team" , 1, LOG_LEVEL_INFO);

}

void inicializarSemaforos(){

		ejecutate = malloc(sizeof(sem_t)*list_size(entrenadores));
		finEjecucion= malloc(sizeof(sem_t)*list_size(entrenadores));
		confirmacion_caught= malloc(sizeof(sem_t)*list_size(entrenadores));


		for(int i=0;i<list_size(entrenadores);i++){
		ejecutate[i] = malloc(sizeof(sem_t));
		finEjecucion[i] = malloc(sizeof(sem_t));
		confirmacion_caught[i] = malloc(sizeof(sem_t));
		}

		for(int i=0;i<list_size(entrenadores);i++){
		sem_init(ejecutate[i], 0, 0);
		sem_init(finEjecucion[i], 0, 0);
		sem_init(confirmacion_caught[i], 0, 0);
		}

		sem_init(&suscripciones, 0, 1);

		sem_init(&mensajesCaught, 0, 0);
		sem_init(&nuevosPokemons, 0, 0);
		sem_init(&aparicion_pokemon, 0, 0);
		sem_init(&agregar_ready, 0, 0);

}

void planificadorFIFO_RR(){

		Entrenador* entrenador = malloc(sizeof(Entrenador));
		int* valor;

		while(list_size(terminados) != list_size(entrenadores)){

			if(list_size(terminados)+list_size(deadlock) != list_size(entrenadores)){

				if(list_size(ready)==0){

					if(list_size(pokemones_en_mapa)==0){
					sem_wait(&aparicion_pokemon);//ponerle un nombre mas expresivo
					puts("\nPase el sem appeared");
					pasar_a_ready_por_cercania();
					sem_post(&agregar_ready);
					}
					sem_wait(&agregar_ready);
					puts("\nPase el sem ready");
				}


				for(int i=0; i<list_size(ready);i++){
					entrenador = list_get(ready,i);
					printf("\n%d)", entrenador->ID);
				}

				//pthread_mutex_lock(&colaReady);
				entrenador = list_remove(ready, 0);
				//pthread_mutex_unlock(&colaReady);

				if(list_size(ejecutando) == 0){
					list_add(ejecutando, entrenador);


					sem_post(ejecutate[entrenador->ID - 1]);
					printf("\n\nDesperte a %d", entrenador->ID);

					printf("\nEspero a %d,", entrenador->ID);
					sem_wait(finEjecucion[entrenador->ID - 1]);
					printf("\nMe llego %d,", entrenador->ID);
				}


				if(list_size(blocked_new) != 0){
					printf("\nCantidad pokemones en mapa %d", list_size(pokemones_en_mapa));
					bool buscarPOkemon(Pokemon* pokemon){
					return pokemon->IdEntrenadorQueLoVaAatrapar == 0;
					}
					if(list_any_satisfy(pokemones_en_mapa,(void*)buscarPOkemon) ==1){
						pasar_a_ready_por_cercania();
					}

				}

			}else{

				log_info(logger, "Empieza a solucionar deadlock.");

				entrenador = list_remove(deadlock,0);

				if(!cumplioSusObjetivos(entrenador)){

				if(list_size(ejecutando) == 0){
				list_add(ejecutando, entrenador);

				sem_post(ejecutate[entrenador->ID - 1]);
				printf("\nDesperte a %d", entrenador->ID);

				puts("\nEspero");
				sem_wait(finEjecucion[entrenador->ID -1]);

				}
				}else{
					log_info(logger, "Soy el ultimo entrenador de deadlock por lo tanto ya tengo todos los que necesito.");
					list_add(ejecutando, entrenador);
					sem_post(ejecutate[entrenador->ID - 1]);

					sem_wait(finEjecucion[entrenador->ID -1]);
				}
			}


		}
	}

void planificadorSJF_SD(){

		Entrenador* entrenador = malloc(sizeof(Entrenador));


		while(list_size(terminados) != list_size(entrenadores)){

			if(list_size(terminados)+list_size(deadlock) != list_size(entrenadores)){

				if(list_size(ready)==0){
					sem_wait(&aparicion_pokemon);
					pasar_a_ready_por_cercania();

				}
				reordenarSJF_SD(1);

				for(int i=0; i<list_size(ready);i++){
					entrenador = list_get(ready,i);
					printf("\n%d) raf %f", entrenador->ID, entrenador->rafaga);
				}


				//pthread_mutex_lock(&colaReady);
				entrenador = list_remove(ready, 0);
				//pthread_mutex_unlock(&colaReady);

				if(list_size(ejecutando) == 0){
					list_add(ejecutando, entrenador);


					sem_post(ejecutate[entrenador->ID - 1]);
					printf("\n\nDesperte a %d", entrenador->ID);

					printf("\nEspero a %d,", entrenador->ID);
					sem_wait(finEjecucion[entrenador->ID - 1]);
					printf("\nMe llego %d,", entrenador->ID);
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

				log_info(logger, "Empieza a solucionar deadlock.");

				for(int i=0; i<list_size(deadlock);i++){
					entrenador = list_get(deadlock,i);
					printf("\n%d) raf %f", entrenador->ID, entrenador->rafaga);
				}

				reordenarSJF_SD(2);

				entrenador = list_remove(deadlock,0);

				if(!cumplioSusObjetivos(entrenador)){

				if(list_size(ejecutando) == 0){
				list_add(ejecutando, entrenador);

				sem_post(ejecutate[entrenador->ID - 1]);
				printf("\nDesperte a %d", entrenador->ID);

				puts("\nEspero");
				sem_wait(finEjecucion[entrenador->ID -1]);

				}
				}else{
					log_info(logger, "Soy el ultimo entrenador de deadlock por lo tanto ya tengo todos los que necesito.");
					list_add(ejecutando, entrenador);
					sem_post(ejecutate[entrenador->ID - 1]);

					sem_wait(finEjecucion[entrenador->ID -1]);
				}
			}


		}
	}


float estimarProximaRafaga(Entrenador* entrenador){



 return alpha * ciclos_entrenadores[entrenador->ID -1] + (1-alpha) * entrenador->rafaga;


}

void reordenarSJF_SD(int lista){

	bool ordenarPorRafaga(Entrenador* entrenador1, Entrenador* entrenador2){

		return (estimarProximaRafaga(entrenador1) < estimarProximaRafaga(entrenador2));

	}
	if(lista==1)
		list_sort(ready, (void*)ordenarPorRafaga);

	if(lista==2)
		list_sort(ready, (void*)ordenarPorRafaga);
}


