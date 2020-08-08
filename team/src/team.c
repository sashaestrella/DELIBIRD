#include "team.h"

int main(int argc,char* argv[])
{




	ciclos_totales=0;


	entrenadores = list_create();
	objetivoGlobal = list_create();

	mensajesGetEnviados= list_create();
	mensajesCatchEnviados= list_create();
	mensajesRecibidos = list_create();
	mapa_auxiliar= list_create();
	especies_localizadas = list_create();
	nuevosCaught = list_create();

	blocked_new= list_create();
	blocked_caught = list_create();
	ready= list_create();
	readyDeadlock = list_create();
	ejecutando = list_create();
	terminados = list_create();
	deadlock = list_create();
	pokemones_en_mapa= list_create();
	especies_repetidas = list_create();
	readyAnterior = list_create();
	entraronPorPrimeraVez = list_create();

	leer_config();
	logger = iniciar_logger();

	inicializarSemaforos();

	obtener_objetivo_global();

	pthread_t conexionBroker;

	pthread_create(&conexionBroker, NULL, (void*)conexionConBroker, NULL);
	pthread_detach(conexionBroker);




	//pokemonesParaPrueba();

	cambios_contexto = 0;

	/*ciclos_entrenadores = malloc(sizeof(int)*list_size(entrenadores));
	for(int i=0;i<list_size(entrenadores);i++)
	ciclos_entrenadores[i]=(int)malloc(sizeof(int));*/


	ciclos = list_create();

	for(int i=0;i<list_size(entrenadores);i++){

	Ciclo* ciclo = malloc(sizeof(Contador));
	ciclo->ciclo=0;
	list_add(ciclos, ciclo);
	}


//PROBAR CON LISTAS

	/*contadorCiclosPorEntrenador = malloc(sizeof(int)*list_size(entrenadores));
	for(int i=0;i<list_size(entrenadores);i++)
	contadorCiclosPorEntrenador [i]=(int)malloc(sizeof(int));*/

	contadores = list_create();

	for(int i=0;i<list_size(entrenadores);i++){

	Contador* contador = malloc(sizeof(Contador));
	contador->contador=0;
	list_add(contadores, contador);
	}


	//hiloEntrenador = malloc(list_size(entrenadores) * sizeof(pthread_t));

	t_list* hilosEntrenadores = list_create();


	for(int j=0; j<list_size(entrenadores);j++){

		pthread_t hiloEntrenador;
		list_add(hilosEntrenadores, hiloEntrenador);
		pthread_create(&hiloEntrenador, NULL, (void*)&flujoEntrenador,list_get(entrenadores, j));
	}




	//probar con listas

	puts("\nInicio Servidor para Gameboy");
	pthread_t hiloConexionGameboy;
	pthread_create(&hiloConexionGameboy, NULL, (void*)noHayBroker, NULL);

	pthread_detach(hiloConexionGameboy);




	int i;
		puts("\n\nPokemones en Mapa");
		Pokemon* pokemon1;
		for(i=0; i<list_size(pokemones_en_mapa);i++){
			pokemon1 = list_get(pokemones_en_mapa, i);
			printf("%s, (%d, %d)", pokemon1->nombre, pokemon1->posicion.posicionX, pokemon1->posicion.posicionY );
			}


	puts("\n\nObjetivo Global");
	for(i=0; i<list_size(objetivoGlobal);i++){
		 		printf("%s,", (char*)list_get(objetivoGlobal, i));
		}

	Entrenador* entrenador;
	for(i=0; i<list_size(entrenadores);i++){
	entrenador= list_get (entrenadores, i);
		 printf("\nEntrenador %d posicion (%d, %d) rafaga %f", entrenador->ID, entrenador->posicion.posicionX, entrenador->posicion.posicionY,entrenador->rafaga);
		 puts("\nMi objetivo: ");
		for(int j=0; j<list_size(entrenador->objetivos);j++){
			printf("%s,", (char*)list_get(entrenador->objetivos, j));
		}

	}


	list_add_all(blocked_new, entrenadores);


	pthread_t aparicion_de_pokemones;

	pthread_create(&aparicion_de_pokemones, NULL, (void*)esperarApariciones, NULL);
	pthread_detach(aparicion_de_pokemones);
	//pasar_a_ready_por_cercania();

	list_add_all(readyAnterior, ready);


	Entrenador* entrenadorReady1 = malloc(sizeof(Entrenador));

	puts("\n\nLa cola ready quedo:");

	for(i=0; i<list_size(ready);i++){
	entrenadorReady1 = list_get (ready, i);
		 printf("\nEntrenador %d posicion (%d, %d)",entrenadorReady1->ID, entrenadorReady1->posicion.posicionX, entrenadorReady1->posicion.posicionY);
		 puts("\nMi objetivo: ");
		for(int j=0; j<list_size(entrenadorReady1->objetivos);j++){
			printf("%s,", (char*)list_get(entrenadorReady1->objetivos, j));
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


bool romperTodo(GetPokemonConIDs* get){
	return true;
}

list_destroy_and_destroy_elements(mensajesGetEnviados, (void*)romperTodo);

Pokemon* pokemon ;



for(i=0; i<list_size(terminados);i++){
	entrenadorReady1 = list_get(terminados,i);
	for(int j=0; j< list_size(entrenadorReady1->pokemonesQueTiene);j++){
	pokemon = list_get(entrenadorReady1->pokemonesQueTiene,j);
	printf("\nSoy %d y agarre a %s ",entrenadorReady1->ID, pokemon->nombre);
	}
}


pthread_t hilo;

for(int j=0; j<list_size(entrenadores);j++){

	hilo = (pthread_t)list_get(hilosEntrenadores, j);

	pthread_join(&hilo, NULL);

}

list_destroy(hilosEntrenadores);

log_info(logger, "Ciclos totales %d.", ciclos_totales);
log_info(logger, "Cambios de contexto %d.", cambios_contexto);

Contador* contador;
for(int i=0; i<list_size(entrenadores);i++){
	entrenador=list_get(entrenadores,i);
	contador=list_get(contadores,i);
	log_info(logger, "Ciclos entrenador %d, %d.",entrenador->ID, contador->contador );
}


}


t_log* iniciar_logger(void)
{
	return log_create(archivoLog, "Log de team" , 1, LOG_LEVEL_INFO);

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
		sem_init(&reintento_appeared, 0, 0);
		sem_init(&reintento_localized, 0, 0);
		sem_init(&reintento_caught, 0, 0);
		sem_init(&nuevoReadySJF, 0, 0);
}

void planificadorFIFO_RR(){

		Entrenador* entrenador;


		while(list_size(terminados) != list_size(entrenadores)){

			if(list_size(terminados)+list_size(deadlock) + list_size(readyDeadlock) != list_size(entrenadores)){

				if(list_size(ready)==0){

					//puts("\nEntre a ready vacio");
					//int valor;

					//sem_getvalue(&agregar_ready, &valor);
					//printf("\nValor %d", valor);
					sem_wait(&agregar_ready);


				}



				puts("\n_____________________________");
				for(int i=0; i<list_size(ready);i++){
					entrenador = list_get(ready,i);
					printf("\n%d)", entrenador->ID);
				}
				puts("\n_____________________________");

				entrenador = list_remove(ready, 0);

			//	puts("\nEmpieza a ejecutar");

				if(list_size(ejecutando) == 0){
					list_add(ejecutando, entrenador);
					log_info(logger, "Entrenador %d entro a EXEC", entrenador->ID);
					cambios_contexto++;

					sem_post(ejecutate[entrenador->ID - 1]);
					//printf("\n\nDesperte a %d", entrenador->ID);

					//printf("\nEspero a %d,", entrenador->ID);
					sem_wait(finEjecucion[entrenador->ID - 1]);
					//printf("\nMe llego %d,", entrenador->ID);
				}


				if(list_size(blocked_new) != 0){
					//printf("\nCantidad pokemones en mapa %d", list_size(pokemones_en_mapa));
					bool buscarPOkemon(Pokemon* pokemon){
					return pokemon->IdEntrenadorQueLoVaAatrapar == 0;
					}
					if(list_any_satisfy(pokemones_en_mapa,(void*)buscarPOkemon) ==1){
						pasar_a_ready_por_cercania();
						log_info(logger, "Entrenador %d entro a cola ready (hay pokemones en mapa para que siga atrapando)", entrenador->ID);
					}

				}




			}else{

				int i;
					if(i!=1){
					detectarDeadlocks();
					i=1;
					}

				//log_info(logger, "Empieza a solucionar deadlock.");
				puts("\n_____________________________");
				for(int i=0; i<list_size(deadlock);i++){
							entrenador = list_get(deadlock,i);
							printf("\n%d)", entrenador->ID);
						}
				puts("\n_____________________________");

				if(list_size(readyDeadlock)==0)
				list_add(readyDeadlock, list_remove(deadlock,0));


				entrenador = list_remove(readyDeadlock,0);
				log_info(logger, "Entrenador %d entro a cola ready (esta resolviendo deadlock)", entrenador->ID);
				if(!cumplioSusObjetivos(entrenador)){

				if(list_size(ejecutando) == 0){
				list_add(ejecutando, entrenador);
				log_info(logger, "Entrenador %d entro a EXEC", entrenador->ID);
				cambios_contexto++;

				sem_post(ejecutate[entrenador->ID - 1]);
				//printf("\nDesperte a %d", entrenador->ID);

				//puts("\nEspero");
				sem_wait(finEjecucion[entrenador->ID -1]);

				}
				}else{

					list_add(ejecutando, entrenador);
					sem_post(ejecutate[entrenador->ID - 1]);

					sem_wait(finEjecucion[entrenador->ID -1]);
				}
			}


		}
	}

void planificadorSJF_SD(){

		Entrenador* entrenador;

		while(list_size(terminados) != list_size(entrenadores)){

			if(list_size(terminados)+list_size(deadlock) + list_size(readyDeadlock) != list_size(entrenadores)){

				if(list_size(ready)==0){
					//puts("\nEntre vacio");
					sem_wait(&agregar_ready);


				}

				reordenarSJF_SD(1);

				puts("\n_____________________________");

				for(int i=0; i<list_size(ready);i++){
					entrenador = list_get(ready,i);
					//printf("\n%d) raf %f", entrenador->ID, entrenador->rafaga);
				}
				puts("\n_____________________________");

				//pthread_mutex_lock(&colaReady);
				entrenador = list_remove(ready, 0);
				//pthread_mutex_unlock(&colaReady);

				if(list_size(ejecutando) == 0){
					list_add(ejecutando, entrenador);
					log_info(logger, "Entrenador %d entro a EXEC", entrenador->ID);
					cambios_contexto++;

					sem_post(ejecutate[entrenador->ID - 1]);
					//printf("\n\nDesperte a %d", entrenador->ID);

					//printf("\nEspero a %d,", entrenador->ID);
					sem_wait(finEjecucion[entrenador->ID - 1]);
					//printf("\nMe llego %d,", entrenador->ID);
				}


				if(list_size(blocked_new) != 0){

					bool buscarPOkemon(Pokemon* pokemon){
					return pokemon->IdEntrenadorQueLoVaAatrapar == 0;
					}
					if(list_any_satisfy(pokemones_en_mapa,(void*)buscarPOkemon) ==1){
						pasar_a_ready_por_cercania();
						log_info(logger, "Entrenador %d entro a cola ready (hay pokemones en mapa para que siga atrapando)", entrenador->ID);
					}

				}

			}else{

				//log_info(logger, "Empieza a solucionar deadlock.");

				int i;
				if(i!=1){
				detectarDeadlocks();
				i=1;
				}

				//log_info(logger, "Empieza a solucionar deadlock.");
				puts("\n_____________________________");
				for(int i=0; i<list_size(deadlock);i++){
				entrenador = list_get(deadlock,i);
				printf("\n%d)", entrenador->ID);
				}
				puts("\n_____________________________");

				if(list_size(readyDeadlock)==0)
				list_add(readyDeadlock, list_remove(deadlock,0));

				entrenador = list_remove(readyDeadlock,0);
				log_info(logger, "Entrenador %d entro a cola ready (esta resolviendo deadlock)", entrenador->ID);


				if(!cumplioSusObjetivos(entrenador)){

				if(list_size(ejecutando) == 0){
				list_add(ejecutando, entrenador);
				log_info(logger, "Entrenador %d entro a EXEC", entrenador->ID);
				cambios_contexto++;
				sem_post(ejecutate[entrenador->ID - 1]);
				//printf("\nDesperte a %d", entrenador->ID);

				//puts("\nEspero");
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


double estimarProximaRafaga(Entrenador* entrenador){
	Ciclo* ciclo = list_get(ciclos, entrenador->ID -1);
	//printf("\nResultado %f",alpha * ciclos_entrenadores[entrenador->ID -1] + (1-alpha) * entrenador->rafaga);

 return alpha * ciclo->ciclo + (1-alpha) * entrenador->rafaga;


}

void reordenarSJF_SD(int lista){

	bool ordenarPorRafaga(Entrenador* entrenador1, Entrenador* entrenador2){

		return (estimarProximaRafaga(entrenador1) < estimarProximaRafaga(entrenador2));

	}
	if(lista==1)
		list_sort(ready, (void*)ordenarPorRafaga);

	if(lista==2)
		list_sort(deadlock, (void*)ordenarPorRafaga);
}


