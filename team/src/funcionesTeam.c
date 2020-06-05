#include "funcionesEntrenador.h"



void leer_config(t_list* new_entrenadores)
{
	t_config* archivo_config =  config_create("team.config"); //lee archivo de configuracion

	char** posiciones_entrenadores =  config_get_array_value(archivo_config, "POSICIONES_ENTRENADORES");

	char** pokemon_entrenadores =  config_get_array_value(archivo_config, "POKEMON_ENTRENADORES");

	char** objetivos_entrenadores =  config_get_array_value(archivo_config, "OBJETIVOS_ENTRENADORES"); // mete a todo en arrays

	armar_entrenadores(posiciones_entrenadores, pokemon_entrenadores, objetivos_entrenadores, new_entrenadores); //les pasas los arrays y la lista

	obtener_objetivo_global(new_entrenadores);
}

void armar_entrenadores(char** posiciones, char** pokemones, char** objetivos,t_list* new_entrenadores){

	pthread_t hilo_entrenador;
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

		obtener_objetivos(pokemones_separados, objetivos_separados, entrenador->objetivos);

		list_add(entrenadores, entrenador); //mete al entrenador en la lista

		pthread_create(&hilo_entrenador, NULL, planificar, entrenador);
		pthread_join(hilo_entrenador,NULL); //los join habria que ponerlos al final del main
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

void obtener_objetivo_global(t_list* entrenadores){

	for(int i=0; i<list_size(entrenadores);i++){

			Entrenador* entrenador = malloc(sizeof(Entrenador));
			entrenador = (Entrenador*)list_get(entrenadores, i);
			list_add_all(objetivoGlobal, entrenador->objetivos);
	}
}

int cantidad(char** lista){

		int i=0;
		while(lista [i] != NULL){
			i++;
		}

		return i;
}



