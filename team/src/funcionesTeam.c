#include "funcionesEntrenador.h"



void leer_config()
{
	archivo_config =  config_create("team.config"); //lee archivo de configuracion

	char** posiciones_entrenadores =  config_get_array_value(archivo_config, "POSICIONES_ENTRENADORES");

	char** pokemon_entrenadores =  config_get_array_value(archivo_config, "POKEMON_ENTRENADORES");

	char** objetivos_entrenadores =  config_get_array_value(archivo_config, "OBJETIVOS_ENTRENADORES");


	tiempoDeReconexion = config_get_int_value(archivo_config, "TIEMPO_RECONEXION");

	retardoCicloCPU = config_get_int_value(archivo_config, "RETARDO_CICLO_CPU");

	algoritmoPlanificacion = config_get_string_value(archivo_config, "ALGORITMO_PLANIFICACION");

	quantum = config_get_int_value(archivo_config, "QUANTUM");

	alpha = config_get_double_value(archivo_config, "ALPHA");

	estimacionInicial = config_get_double_value(archivo_config, "ESTIMACION_INICIAL");

	archivoLog = config_get_string_value(archivo_config, "LOG_FILE");

	armar_entrenadores(posiciones_entrenadores, pokemon_entrenadores, objetivos_entrenadores); //les pasas los arrays y la lista

	int cant;
	cant= cantidad(posiciones_entrenadores);


	for(int i =0;i<cant;i++){

		free(posiciones_entrenadores[i]);
	}
	free(posiciones_entrenadores);

	cant = cantidad(objetivos_entrenadores);

	for(int i =0;i<cant;i++){
			free(objetivos_entrenadores[i]);
		}
	free(objetivos_entrenadores);

	cant = cantidad(pokemon_entrenadores);

	for(int i =0;i<cant;i++){
			free(pokemon_entrenadores[i]);
		}
	free(pokemon_entrenadores);


	IDsuscripcionAppeared = config_get_int_value(archivo_config, "ID_APPEARED");

	IDsuscripcionCaught = config_get_int_value(archivo_config, "ID_CAUGHT");

	IDsuscripcionLocalized = config_get_int_value(archivo_config, "ID_LOCALIZED");


	ip = config_get_string_value(archivo_config,"IP");

	puerto = config_get_string_value(archivo_config,"PUERTO");

	//printf("ip y puerto de broker %s %s", ip, puerto);

	IP_TEAM = config_get_string_value(archivo_config,"IP_TEAM");

	PUERTO_TEAM = config_get_string_value(archivo_config,"PUERTO_TEAM");



	//free(archivo_config);
}

void armar_entrenadores(char** posiciones, char** pokemones, char** objetivos){


	int cantidad_entrenadores = cantidad(posiciones);



	for(int i=0; i<cantidad_entrenadores; i++){

		char** pokemones_separados;
		Entrenador* entrenador = malloc(sizeof(Entrenador));

		entrenador->ID = i+1;
		entrenador->estado = NEW;

		char** posiciones_separadas = string_split(posiciones[i], "|");

		entrenador->posicion.posicionX =atoi(posiciones_separadas[0]);
		entrenador->posicion.posicionY =atoi(posiciones_separadas[1]);

		int cant = cantidad(pokemones);


		//if(i<= cant){

		if(pokemones[i] != NULL){
		pokemones_separados = string_split(pokemones[i], "|");

		//free(pokemones);
		}else{
			pokemones_separados = NULL;
		}


		/*if(pokemones[0] == NULL){
			pokemones_separados = NULL;
		}*/

		char** objetivos_separados = string_split(objetivos[i], "|");
		//free(objetivos);
		entrenador->objetivos = list_create();

		entrenador->pokemonesQueTiene = list_create();

		entrenador->rafaga = estimacionInicial;

		obtener_objetivos(pokemones_separados, objetivos_separados, entrenador->objetivos);


		liberarListas(objetivos_separados);

		liberarListas(pokemones_separados);

		liberarListas(posiciones_separadas);





		entrenador->tieneAsignadoUnPokemon = false;

		entrenador->cantidad = list_size(entrenador->objetivos);

		list_add(entrenadores, entrenador);
	}
}


void obtener_objetivos(char** yaTiene, char** objetivos, t_list* listaObjetivos){


	int pokemones = cantidad(yaTiene);

	for(int i=0; i<cantidad(objetivos) ; i++ ){

		int flag=0;

		for(int j=0; j< pokemones;j++){

			if(string_equals_ignore_case(objetivos[i], yaTiene[j]) == true){

				for(int k=j; k<pokemones;k++){
					yaTiene[k] = yaTiene[k+1];
				}

				pokemones--;
				flag = 1;
				break;
			}
		}

		if(flag != 1){

			list_add(listaObjetivos, string_duplicate(objetivos[i]));

		}
		}
}

void obtener_objetivo_global(){

	for(int i=0; i<list_size(entrenadores);i++){

			Entrenador* entrenador;
			entrenador = (Entrenador*)list_get(entrenadores, i);
			list_add_all(objetivoGlobal, entrenador->objetivos);
	}
}

int obtenerCantidadObjetivo(char* nombre){
	bool compararNombre(char* objetivo){
		 return strcmp(nombre,objetivo);
	}
	return list_count_satisfying(objetivoGlobal, (void*)compararNombre);
}

int cantidad(char** lista){

	if(lista == NULL)
		return 0;

		int i=0;
		while(lista [i] != NULL){
			i++;
		}

		return i;
}

void liberarListas(char** lista){

	if(lista != NULL){

		int i=0;
		while(lista [i] != NULL){

			free(lista[i]);

			i++;
		}
		free(lista);
	}

	if(lista == NULL){
		free(lista);
	}



}

Algoritmos algoritmoAUtilizar(char* algoritmoPlanificacion){

	if (strcmp(algoritmoPlanificacion, "FIFO") == 0)
		return FIFO;
	if (strcmp(algoritmoPlanificacion, "RR") == 0)
		return RR;
	if (strcmp(algoritmoPlanificacion, "SJF-CD") == 0)
		return SJF_CD;
	if (strcmp(algoritmoPlanificacion, "SJF-SD") == 0)
		return SJF_SD;
	else
		printf("Error en el algoritmo ingresado");
		return -1;
}


void enviar_gets(){

	GetPokemon* getPokemon = malloc(sizeof(getPokemon));
	char* pokemon;


	for(int i=0; i<list_size(objetivoGlobal);i++){

		pokemon = list_get(objetivoGlobal,i);

		if(noEstaEnLaLista(especies_repetidas, pokemon)){

		list_add(especies_repetidas, pokemon);
		pokemon = list_get(objetivoGlobal,i);
		//printf("\nMAndo a %s", pokemon);
		getPokemon->nombre = pokemon;
		getPokemon->tamanioNombrePokemon = strlen(pokemon) +1;

		enviar_getPokemon(getPokemon);
		}
	}

	free(getPokemon);

}


void conexionConBroker(){

	enviar_gets();

	generarConexiones();


	while(1){
		sem_wait(&reintento_appeared);
		sem_wait(&reintento_localized);
		sem_wait(&reintento_caught);

		while(crear_conexion(ip, puerto) == -1){
		log_info(logger, "Error de conexion. Inicia reintento");

		sleep(tiempoDeReconexion);
		}
		IDsuscripcionAppeared =0;
		IDsuscripcionLocalized =0;
		IDsuscripcionCaught = 0;
		generarConexiones();
		log_info(logger, "Conexion Exitosa");


	}
}


void detectarDeadlocks(){

	typedef struct entrenador{
		uint32_t ID;
		t_list* objetivos;
		t_list* pokemonesQueTiene;
	}EntrenadorDeadlock;

	EntrenadorDeadlock* entrenador = malloc(sizeof(EntrenadorDeadlock));
	EntrenadorDeadlock* entrenadorEncontrado = malloc(sizeof(EntrenadorDeadlock));
	EntrenadorDeadlock* entrenadorPrimero = malloc(sizeof(EntrenadorDeadlock));
	char* pokemon;
	int id;
	t_list* deadlockTemporal = list_create();
	t_list* deadlockCircular = list_create();
	//list_add_all(deadlockTemporal, deadlock);

	log_info(logger, "Inicia algotirmo de deteccion de deadlock");

	for(int i=0; i<list_size(deadlock); i++){
		EntrenadorDeadlock* entrenadorAux = malloc(sizeof(EntrenadorDeadlock));
		Entrenador* entrenador ;
		entrenadorAux->objetivos = list_create();
		entrenadorAux->pokemonesQueTiene= list_create();

		entrenador = list_get(deadlock, i);

		entrenadorAux->ID = entrenador->ID;
		entrenadorAux->objetivos = list_take(entrenador->objetivos,list_size(entrenador->objetivos));
		entrenadorAux->pokemonesQueTiene = list_take(entrenador->pokemonesQueTiene, list_size(entrenador->pokemonesQueTiene));

		list_add(deadlockTemporal, entrenadorAux);
	}



	for(int i=0; i<list_size(deadlockTemporal);i++){

		EntrenadorDeadlock* entrenador;

		entrenador = list_get(deadlockTemporal, i);



		bool loNecesita(Pokemon* pokemon){

			bool esta(char* poke){
				return !strcmp(pokemon->nombre, poke);
			}

			if(list_any_satisfy(entrenador->objetivos, (void*)esta)){

				char* p = list_remove_by_condition(entrenador->objetivos, (void*)esta);
				/*printf("\nEntrenador %d)", entrenador->ID);
				printf("\nSaque a %s", p);
				printf("\nY a %s", pokemon->nombre);*/

				return true;
			}else{

			return false;
			}
		}

		list_remove_by_condition(entrenador->pokemonesQueTiene, (void*)loNecesita);



		//printf("\nEntrenador %d)", entrenador->ID);
		for(int i=0; i<list_size(entrenador->pokemonesQueTiene);i++){
			Pokemon* pokemon;

			pokemon = list_get(entrenador->pokemonesQueTiene,i);
			//printf("\nAhora tengo %s", pokemon->nombre );

		}
		for(int i=0; i<list_size(entrenador->objetivos);i++){

				char* poke;
				poke = list_get(entrenador->objetivos, i);
				//printf("\nY necesito %s", poke );
			}

	}



while(list_size(deadlockTemporal) !=0){
		entrenadorPrimero = list_remove(deadlockTemporal, 0);
		if(list_size(deadlockTemporal) == 0)
			break;
		entrenador = entrenadorPrimero;

		int i =1;
		list_add(deadlockCircular, entrenador);
		while(list_size(deadlockCircular) != 0 || i==1){ //no es distinto de 0
			i=0;



		bool tieneUnoQueNecesito(EntrenadorDeadlock* e){
			bool tiene;
			char* poke1;

			for(int k=0; k<list_size(entrenador->objetivos);k++){
				poke1 = list_get(entrenador->objetivos, k);
				bool esta(Pokemon* p){
					return !strcmp(p->nombre, poke1);
				}

				tiene = list_any_satisfy(e->pokemonesQueTiene, (void*)esta) && e->ID != entrenador->ID;
				if(tiene)
					break;
			}

			return tiene;
		}



		entrenadorEncontrado = list_find(deadlockTemporal, (void*)tieneUnoQueNecesito);

		//printf("\nEntrenador encontrado %d", entrenadorEncontrado->ID);

		bool tengoUnoQueNecesita(char* poke){


			bool esta(Pokemon* p){
				return !strcmp(p->nombre, poke);
			}

			return list_any_satisfy(entrenadorPrimero->pokemonesQueTiene, (void*)esta);
		}


		if(list_any_satisfy(entrenadorEncontrado->objetivos, (void*)tengoUnoQueNecesita)){

			bool quitar(EntrenadorDeadlock* e){
				return e->ID == entrenadorEncontrado->ID;
			}

			//printf("\nMETI A %d", entrenadorEncontrado->ID);

			if(list_size(entrenadorEncontrado->objetivos) ==1)
			list_remove_by_condition(deadlockTemporal, (void*)quitar);

			list_add(deadlockCircular, entrenadorEncontrado );

			Entrenador* mostrar;// = malloc(sizeof(Entrenador));
			//printf("\nTAmaño deadlock circular %d", list_size(deadlockCircular));
			puts("\nSE DETECTO DEADLOCK ENTRE:");
			log_info(logger, "SE DETECTO DEADLOCK ENTRE:");
			for(int i=0; i<list_size(deadlockCircular); i++){
				mostrar = list_get(deadlockCircular,i);
				printf(" \n %d)", mostrar->ID);
				log_info(logger, "%d)", mostrar->ID);
			}
			list_clean(deadlockCircular);
			break;


		}else{


			bool quitar(EntrenadorDeadlock* e){
				return e->ID == entrenadorEncontrado->ID;
			}

			if(list_size(entrenadorEncontrado->objetivos) ==1)
			list_remove_by_condition(deadlockTemporal, (void*)quitar);

			//printf("\nMETI A %d", entrenadorEncontrado->ID);
			list_add(deadlockCircular, entrenadorEncontrado);
			//printf("\nTAmaño deadlock circular %d", list_size(deadlockCircular));
			entrenador = entrenadorEncontrado;
			entrenadorEncontrado = NULL;
			}

		}


		}


}


void esperarApariciones(){

	Pokemon* pokemon;

	pasar_a_ready_por_cercania();

	for(int i=0; i<list_size(pokemones_en_mapa); i++){
		pokemon = list_get(pokemones_en_mapa,i);
		log_info(logger, "Etrenador %d entro a cola ready (por cercania a un pokemon en el mapa)", pokemon->IdEntrenadorQueLoVaAatrapar);
	}
	int agrego;
	while(1){

	sem_wait(&aparicion_pokemon);

	agrego = pasar_a_ready_por_cercania();

	pokemon= list_get(pokemones_en_mapa, list_size(pokemones_en_mapa)-1);

	if(pokemon->IdEntrenadorQueLoVaAatrapar !=0)
	log_info(logger, "Etrenador %d entro a cola ready (aparicion de nuevo pokemon)", pokemon->IdEntrenadorQueLoVaAatrapar);

	if(agrego==1 && list_size(ready)==1 && list_size(ejecutando)==0){
		int valor;
		sem_post(&agregar_ready);
		sem_getvalue(&agregar_ready,&valor);

	}
	if(list_size(ejecutando)==1 ){
		int valor;
		sem_getvalue(&nuevoReadySJF, &valor);

		if(valor == 0)
		sem_post(&nuevoReadySJF);

	}
}
}


bool noEstaEnLaLista(t_list* lista, char* pokemon){

	bool buscarEspecie(char* especie){
		return !strcmp(especie, pokemon);
	}

	return ! list_any_satisfy(lista, (void*)buscarEspecie);
}
