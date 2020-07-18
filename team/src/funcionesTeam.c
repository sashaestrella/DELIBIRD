#include "funcionesEntrenador.h"



void leer_config()
{
	archivo_config =  config_create("team.config"); //lee archivo de configuracion

	char** posiciones_entrenadores =  config_get_array_value(archivo_config, "POSICIONES_ENTRENADORES");

	char** pokemon_entrenadores =  config_get_array_value(archivo_config, "POKEMON_ENTRENADORES");

	char** objetivos_entrenadores =  config_get_array_value(archivo_config, "OBJETIVOS_ENTRENADORES"); // mete a todo en arrays

	armar_entrenadores(posiciones_entrenadores, pokemon_entrenadores, objetivos_entrenadores); //les pasas los arrays y la lista

	tiempoDeReconexion = config_get_int_value(archivo_config, "TIEMPO_RECONEXION");

	retardoCicloCPU = config_get_int_value(archivo_config, "RETARDO_CICLO_CPU");

	algoritmoPlanificacion = config_get_string_value(archivo_config, "ALGORITMO_PLANIFICACION");

	quantum = config_get_int_value(archivo_config, "QUANTUM");

	//alpha = config_get_double_value(archivo_config, "ALPHA");

	estimacionInicial = config_get_double_value(archivo_config, "ESTIMACION_INICIAL");

	//archivoLog = config_get_string_value(archivo_config, "LOG_FILE");

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


	int cantidad_entrenadores = cantidad(posiciones); //calcula cuantos entrenadores hay para el for segun la cantidad de posiciones
	int i;

	for(i=0; i<cantidad_entrenadores; i++){// itera y mete a los entrenadores a la lista segun la cantidad que haya

		Entrenador* entrenador = malloc(sizeof(Entrenador));

		entrenador->ID = i+1;
		entrenador->estado = NEW;

		char** posiciones_separadas = string_split(posiciones[i], "|"); // devuelve un array sin los pipe

		entrenador->posicion.posicionX =atoi(posiciones_separadas[0]);
		entrenador->posicion.posicionY =atoi(posiciones_separadas[1]);

		char** pokemones_separados = string_split(pokemones[i], "|");

		char** objetivos_separados = string_split(objetivos[i], "|");

		entrenador->objetivos = list_create();

		entrenador->pokemonesQueTiene = list_create();

		entrenador->rafaga = estimacionInicial;

		/*for(i=0; i<cantidad(pokemones_separados);i++){
		list_add(entrenador->pokemonesQueTiene, pokemones_separados[i]);
		}*/

		obtener_objetivos(pokemones_separados, objetivos_separados, entrenador->objetivos);

		entrenador->tieneAsignadoUnPokemon = false;

		entrenador->cantidad = list_size(entrenador->objetivos);

		list_add(entrenadores, entrenador); //mete al entrenador en la lista
	}
}


char** obtener_objetivos(char** yaTiene, char** objetivos, t_list* listaObjetivos){

	int i;
	int pokemones = cantidad(yaTiene);

	for(i=0; i<cantidad(objetivos) ; i++ ){

		int flag=0;
		int j;
		for(j=0; j< pokemones;j++){

			if(string_equals_ignore_case(objetivos[i], yaTiene[j]) == true){
				int k;
				for(k=j; k<pokemones;k++){
					yaTiene[k] = yaTiene[k+1];
				}
				pokemones--;
				flag = 1;
				break;
			}
		}
		if(flag != 1){
			list_add(listaObjetivos, objetivos[i]);
		}
		}
}

void obtener_objetivo_global(){

	for(int i=0; i<list_size(entrenadores);i++){

			Entrenador* entrenador = malloc(sizeof(Entrenador));
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

		int i=0;
		while(lista [i] != NULL){
			i++;
		}

		return i;
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
	t_list* especies_repetidas = list_create();

	for(int i=0; i<list_size(objetivoGlobal);i++){

		pokemon = list_get(objetivoGlobal,i);

		if(noEstaEnLaLista(especies_repetidas, pokemon)){

		list_add(especies_repetidas, pokemon);
		pokemon = list_get(objetivoGlobal,i);

		getPokemon->nombre = pokemon;
		getPokemon->tamanioNombrePokemon = strlen(pokemon) +1;

		enviar_getPokemon(getPokemon);
		}
	}

}


void conexionConBroker(){

	enviar_gets();

	generarConexiones();


	while(1){
		sem_wait(&reintento_appeared);
		sem_wait(&reintento_localized);
		sem_wait(&reintento_caught);

		while(crear_conexion(ip, puerto) == -1){
			puts("\nReintento");

		sleep(tiempoDeReconexion);
		}
		IDsuscripcionAppeared =0;
		IDsuscripcionLocalized =0;
		IDsuscripcionCaught = 0;
		generarConexiones();
		puts("\nConexion exitosa");

	}
}


void detectarDeadlocks(){

	Entrenador* entrenador = malloc(sizeof(Entrenador));
	Entrenador* entrenadorEncontrado = malloc(sizeof(Entrenador));
	Entrenador* entrenadorPrimero = malloc(sizeof(Entrenador));
	char* pokemon;
	int id;
	t_list* deadlockTemporal = list_create();
	t_list* deadlockCircular = list_create();
	list_add_all(deadlockTemporal, deadlock);

while(list_size(deadlockTemporal) !=0){
		entrenadorPrimero = list_remove(deadlockTemporal, 0);
		if(list_size(deadlockTemporal) == 0)
			break;
		entrenador = entrenadorPrimero;
		//puts("\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
		int i =1;
		list_add(deadlockCircular, entrenador);
		while(list_size(deadlockCircular) != 0 || i==1){ //no es distinto de 0
			i=0;


		//printf("\nTAmaño deadlock circular %d", list_size(deadlockCircular));
		bool tieneUnoQueNecesito(Entrenador* e){
			bool tiene;
			char* poke1;

			for(int k=0; k<list_size(entrenador->objetivos);k++){
				poke1 = list_get(entrenador->objetivos, k);
				bool esta(Pokemon* p){
					return !strcmp(p->nombre, poke1);
				}

				tiene = list_any_satisfy(e->pokemonesQueTiene, esta) && e->ID != entrenador->ID;
				if(tiene)
					break;
			}

			return tiene;
		}



		entrenadorEncontrado = list_find(deadlockTemporal, tieneUnoQueNecesito);

		//printf("\nEntrenador encontrado %d", entrenadorEncontrado->ID);

		bool tengoUnoQueNecesita(char* poke){


			bool esta(Pokemon* p){
				return !strcmp(p->nombre, poke);
			}

			return list_any_satisfy(entrenadorPrimero->pokemonesQueTiene, esta);
		}


		if(list_any_satisfy(entrenadorEncontrado->objetivos, tengoUnoQueNecesita)){

			bool quitar(Entrenador* e){
				return e->ID == entrenadorEncontrado->ID;
			}

			//printf("\nMETI A %d", entrenadorEncontrado->ID);

			if(list_size(entrenadorEncontrado->objetivos) ==1)
			list_remove_by_condition(deadlockTemporal, quitar);

			list_add(deadlockCircular, entrenadorEncontrado );

			Entrenador* mostrar;// = malloc(sizeof(Entrenador));
		//	printf("\nTAmaño deadlock circular %d", list_size(deadlockCircular));
			//puts("\nSE DETECTO DEADLOCK ENTRE:");
			for(int i=0; i<list_size(deadlockCircular); i++){
				mostrar = list_get(deadlockCircular,i);
				printf(" \n %d)", mostrar->ID);
			}
			list_clean(deadlockCircular);
			break;


		}else{
			//puts("\nPAse por aqui");

			bool quitar(Entrenador* e){
				return e->ID == entrenadorEncontrado->ID;
			}

			if(list_size(entrenadorEncontrado->objetivos) ==1)
			list_remove_by_condition(deadlockTemporal, quitar);

		//	printf("\nMETI A %d", entrenadorEncontrado->ID);
			list_add(deadlockCircular, entrenadorEncontrado);
			//printf("\nTAmaño deadlock circular %d", list_size(deadlockCircular));
			entrenador = entrenadorEncontrado;
			entrenadorEncontrado = NULL;
			}

		}


		}


}

bool noEstaEnLaLista(t_list* lista, char* pokemon){

	bool buscarEspecie(char* especie){
		return !strcmp(especie, pokemon);
	}

	return ! list_any_satisfy(lista, buscarEspecie);
}
