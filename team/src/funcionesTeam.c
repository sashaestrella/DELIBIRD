#include "funcionesEntrenador.h"



void leer_config()
{
	t_config* archivo_config =  config_create("team.config"); //lee archivo de configuracion

	char** posiciones_entrenadores =  config_get_array_value(archivo_config, "POSICIONES_ENTRENADORES");

	char** pokemon_entrenadores =  config_get_array_value(archivo_config, "POKEMON_ENTRENADORES");

	char** objetivos_entrenadores =  config_get_array_value(archivo_config, "OBJETIVOS_ENTRENADORES"); // mete a todo en arrays

	armar_entrenadores(posiciones_entrenadores, pokemon_entrenadores, objetivos_entrenadores); //les pasas los arrays y la lista

	//tiempoDeReconexion = config_get_int_value(archivo_config, "TIEMPO_RECONEXION");

	//retardoCicloCPU = config_get_int_value(archivo_config, "RETARDO_CICLO_CPU");

	//algoritmoPlanificacion = config_get_array_value(archivo_config, "ALGORITMO_PLANIFICACION");

	//quantum = config_get_int_value(archivo_config, "QUANTUM");

	//alpha = config_get_array_value(archivo_config, "ALPHA");

	//estimacionInicial = config_get_int_value(archivo_config, "ESTIMACION_INICIAL");

	//archivoLog = config_get_array_value(archivo_config, "LOG_FILE");

	ip = config_get_string_value(archivo_config,"IP");

	puerto = config_get_string_value(archivo_config,"PUERTO");

	free(archivo_config);
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



