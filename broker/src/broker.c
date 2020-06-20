#include "broker.h"

void errorExit(char* strerr){
	perror(strerr);
	exit(1);
}

int main(void)
{

	//iniciar_servidor();

	pthread_t hiloConexion;

	New_Pokemon = list_create();
	Localized_Pokemon = list_create();
	Get_Pokemon = list_create();
	Appeared_Pokemon = list_create();
	Catch_Pokemon = list_create();
	Caught_Pokemon = list_create();
	suscriptores_new_pokemon = list_create();
	suscriptores_localized_pokemon = list_create();
	suscriptores_get_pokemon = list_create();
	suscriptores_appeared_pokemon = list_create();
	suscriptores_catch_pokemon = list_create();
	suscriptores_caught_pokemon = list_create();
	listaPosicionesLibres=list_create();
	listaPosicionesOcupadas=list_create();


	config= config_create("/home/utnso/tp-2020-1c-BOMP/broker/Debug/broker.config");
	tamanioMinimoParticion = atoi(config_get_string_value(config, "TAMANIO_MINIMO_PARTICION"));
	int tamanioMemoria = atoi(config_get_string_value(config, "TAMANIO_MEMORIA"));
	memoriaInterna = malloc(tamanioMemoria);
	PosicionLibre* primerPosicionLibre = malloc(sizeof(PosicionLibre));
	list_add(listaPosicionesLibres,primerPosicionLibre);
	primerPosicionLibre->posicion = memoriaInterna;
	primerPosicionLibre->tamanio = tamanioMemoria;

	printf("------------ARCHIVO DE CONFIGURACION BROKER\n");
	printf("TAMANIO_MEMORIA = %d\n",tamanioMemoria);
	printf("TAMANIO_MINIMO_PARTICION = %d\n",tamanioMinimoParticion);

	algoritmoMemoria = config_get_string_value(config,"ALGORITMO_MEMORIA");
	printf("ALGORITMO_MEMORIA = %s\n", algoritmoMemoria);
	algoritmoReemplazo = config_get_string_value(config,"ALGORITMO_REEMPLAZO");
	printf("ALGORITMO_REEMPLAZO = %s\n",algoritmoReemplazo);
	algoritmoParticionLibre = config_get_string_value(config, "ALGORITMO_PARTICION_LIBRE");
	printf("ALGORITMO_PARTICION_LIBRE = %s\n",algoritmoParticionLibre);
	ip = atoi(config_get_string_value(config, "IP_BROKER"));
	printf("IP_BROKER = %d\n",ip);
	puerto = atoi(config_get_string_value(config, "PUERTO_BROKER"));
	printf("PUERTO_BROKER = %d\n",puerto);
	busquedasFallidasPreviasACompactacion = atoi(config_get_string_value(config, "FRECUENCIA_COMPACTACION"));
	printf("FRECUENCIA_COMPACTACION = %d\n",busquedasFallidasPreviasACompactacion);
	char* pathDelLogger = config_get_string_value(config,"LOG_FILE");
	printf("LOG_FILE = %s\n",pathDelLogger);
	logger = log_create(pathDelLogger, "Broker-Log" , 1, LOG_LEVEL_INFO);

	printf("------------FIN DE ARCHIVO DE CONFIGURACION BROKER\n");
/*
	PosicionLibre* unaPosicionLibre1 = malloc(sizeof(PosicionLibre));
	unaPosicionLibre1->posicion = 1;
	unaPosicionLibre1->tamanio = 160;
	list_add(listaPosicionesLibres,unaPosicionLibre1);

	PosicionLibre* unaPosicionLibre2 = malloc(sizeof(PosicionLibre));
	unaPosicionLibre2->posicion = 2;
	unaPosicionLibre2->tamanio = 50;
	list_add(listaPosicionesLibres,unaPosicionLibre2);

	PosicionLibre* unaPosicionLibre3 = malloc(sizeof(PosicionLibre));
	unaPosicionLibre3->posicion = 4;
	unaPosicionLibre3->tamanio = 400;
	list_add(listaPosicionesLibres,unaPosicionLibre3);



	PosicionOcupada* unaPosicionOcupada1 = malloc(sizeof(PosicionOcupada));
	unaPosicionOcupada1->tamanio = 150;
	unaPosicionOcupada1->posicion = 0;
	unaPosicionOcupada1->colaALaQuePertenece = 2;
	unaPosicionOcupada1->ID = 1;
	list_add(listaPosicionesOcupadas,unaPosicionOcupada1);

	PosicionOcupada* unaPosicionOcupada2 = malloc(sizeof(PosicionOcupada));
	unaPosicionOcupada2->tamanio = 60;
	unaPosicionOcupada2->posicion = 1;
	unaPosicionOcupada2->colaALaQuePertenece = 2;
	unaPosicionOcupada2->ID = 2;
	list_add(listaPosicionesOcupadas,unaPosicionOcupada2);

	PosicionOcupada* unaPosicionOcupada3 = malloc(sizeof(PosicionOcupada));
	unaPosicionOcupada3->tamanio = 450;
	unaPosicionOcupada3->posicion = 2;
	unaPosicionOcupada3->colaALaQuePertenece = 2;
	unaPosicionOcupada3->ID = 3;
	list_add(listaPosicionesOcupadas,unaPosicionOcupada3);


	puts("------------Agrego posiciones libres cualquiera\n");
	printf("Tamaño de posiciones libres: %d\n", list_size(listaPosicionesLibres));
	PosicionLibre* unaPosicion = list_get(listaPosicionesLibres,0);
	PosicionLibre* unaPosicion2 = list_get(listaPosicionesLibres,1);
	PosicionLibre* unaPosicion3 = list_get(listaPosicionesLibres,2);
	PosicionLibre* unaPosicion4 = list_get(listaPosicionesLibres,3);

	printf("Tamaño posicion 0: %d\n",unaPosicion->tamanio);
	printf("Tamaño posicion 1: %d\n",unaPosicion2->tamanio);
	printf("Tamaño posicion 2: %d\n",unaPosicion3->tamanio);
	printf("Tamaño posicion 3: %d\n",unaPosicion4->tamanio);

	puts("\n------------Pruebas FF\n");
	PosicionLibre* posicionQueVoyAPedir1;
	int tamanio1 = 150;
	printf("Ingrese el tamaño: %d\n",tamanio1);
	posicionQueVoyAPedir1 = pedirPosicionFF(tamanio1);

	PosicionLibre* posicionQueVoyAPedir2;
	int tamanio2 = 100;
	printf("\nIngrese el tamaño: %d",tamanio2);
	posicionQueVoyAPedir2 = pedirPosicionFF(tamanio2);

	PosicionLibre* posicionQueVoyAPedir3;
	int tamanio3 = 300;
	printf("\nIngrese el tamaño: %d",tamanio3);
	posicionQueVoyAPedir2 = pedirPosicionFF(tamanio3);
	puts("------------Fin Pruebas FF\n");


	puts("\n------------Pruebas BF\n");
	PosicionLibre* posicionQueVoyAPedir4;
	int tamanio4 = 150;
	printf("Ingrese el tamaño: %d\n",tamanio4);
	posicionQueVoyAPedir4 = pedirPosicionBF(tamanio1);

	PosicionLibre* posicionQueVoyAPedir5;
	int tamanio5 = 100;
	printf("\nIngrese el tamaño: %d",tamanio5);
	posicionQueVoyAPedir5 = pedirPosicionBF(tamanio2);

	PosicionLibre* posicionQueVoyAPedir6;
	int tamanio6 = 300;
	printf("\nIngrese el tamaño: %d",tamanio6);
	posicionQueVoyAPedir6 = pedirPosicionBF(tamanio3);

	PosicionLibre* posicionQueVoyAPedir7;
	int tamanio7 = 700;
	printf("\nIngrese el tamaño: %d",tamanio7);
	posicionQueVoyAPedir7 = pedirPosicionBF(tamanio7);
	puts("\n------------Fin Pruebas BF\n");

	puts("\n------------Pruebas borrar por FIFO\n");
	puts("------------Agrego posiciones ocupadas cualquiera\n");
	PosicionOcupada* unaPosicionOcupada = list_get(listaPosicionesOcupadas,0);
	PosicionOcupada* unaPosicionOc2 = list_get(listaPosicionesOcupadas,1);
	PosicionOcupada* unaPosicionOc3 = list_get(listaPosicionesOcupadas,2);

	printf("Tamaño posicion 0: %d,ID = %d\n",unaPosicionOcupada->tamanio,unaPosicionOcupada->ID);
	printf("Tamaño posicion 1: %d,ID = %d\n",unaPosicionOc2->tamanio,unaPosicionOc2->ID);
	printf("Tamaño posicion 2: %d,ID = %d\n",unaPosicionOc3->tamanio,unaPosicionOc3->ID);

	int tamanioOcupados = list_size(listaPosicionesOcupadas);
	for(int i=0;i<tamanioOcupados;i++){
		borrarFIFO();
	}
	puts("\n------------Fin Pruebas borrar por FIFO\n");

	puts("\n------------Pruebas borrar por LRU\n");
	PosicionOcupada* unaPosicionOcupada4 = malloc(sizeof(PosicionOcupada));
	unaPosicionOcupada4->tamanio = 48;
	unaPosicionOcupada4->posicion = 3;
	unaPosicionOcupada4->colaALaQuePertenece = 2;
	unaPosicionOcupada4->ID = 4;
	list_add(listaPosicionesOcupadas,unaPosicionOcupada4);

	PosicionOcupada* unaPosicionOcupada5 = malloc(sizeof(PosicionOcupada));
	unaPosicionOcupada5->tamanio = 650;
	unaPosicionOcupada5->posicion = 4;
	unaPosicionOcupada5->colaALaQuePertenece = 2;
	unaPosicionOcupada5->ID = 5;
	list_add(listaPosicionesOcupadas,unaPosicionOcupada5);

	PosicionOcupada* unaPosicionOcupada6 = malloc(sizeof(PosicionOcupada));
	unaPosicionOcupada6->tamanio = 270;
	unaPosicionOcupada6->posicion = 5;
	unaPosicionOcupada6->colaALaQuePertenece = 2;
	unaPosicionOcupada6->ID = 6;
	list_add(listaPosicionesOcupadas,unaPosicionOcupada6);

	PosicionOcupada* unaPosicionOcupada7 = malloc(sizeof(PosicionOcupada));
	unaPosicionOcupada7->tamanio = 70;
	unaPosicionOcupada7->posicion = 6;
	unaPosicionOcupada7->colaALaQuePertenece = 2;
	unaPosicionOcupada7->ID = 7;
	list_add(listaPosicionesOcupadas,unaPosicionOcupada7);

	borrarLRU();
	puts("\n------------Fin Pruebas borrar por LRU");
*/
/*
	NewPokemon* unNewPokemon2 = malloc(sizeof(NewPokemon));
	MensajeNewPokemon* mensaje2;
		char* nombre2 = malloc(9);
		nombre2 = "ALAKAZAM";
		unNewPokemon2->nombre = nombre2;
		unNewPokemon2->coordenadas.posicionX = 2;
		unNewPokemon2->coordenadas.posicionY = 3;
		unNewPokemon2->cantidad = 3;

		mensaje2 = guardarMensajeNewPokemon(unNewPokemon2);*/
/*

	LocalizedPokemon* localizedPokemon1 = malloc(sizeof(LocalizedPokemon));
	MensajeLocalizedPokemon* mensaje3;
		localizedPokemon1->nombre = "LOCALIZED1";
		localizedPokemon1->cantidadParesOrdenados = 2;
		int numero = 1;
		int idCorrelativo = 1;
		t_list* pares = list_create();
		list_add(pares,&numero);
		list_add(pares,&numero);
		list_add(pares,&numero);
		list_add(pares,&numero);
		localizedPokemon1->paresOrdenados = pares;
		int tamanioLista = list_size(localizedPokemon1->paresOrdenados);
		printf("\nEl tamaño de pares ordenados de localized es: %d",tamanioLista);

		mensaje3 = guardarMensajeLocalizedPokemon(localizedPokemon1,idCorrelativo);


	GetPokemon* unGetPokemon1 = malloc(sizeof(GetPokemon));
	MensajeGetPokemon* mensaje4;
		unGetPokemon1->nombre = "GET";

		mensaje4 = guardarMensajeGetPokemon(unGetPokemon1);

	AppearedPokemon* unAppearedPokemon1 = malloc(sizeof(AppearedPokemon));
	MensajeAppearedPokemon* mensaje5;
		unAppearedPokemon1->nombre = "APPEARED1";
		unAppearedPokemon1->coordenadas.posicionX = 2;
		unAppearedPokemon1->coordenadas.posicionY = 4;

		mensaje5 = guardarMensajeAppearedPokemon(unAppearedPokemon1);

	CatchPokemon* unCatchPokemon1 = malloc(sizeof(CatchPokemon));
	MensajeCatchPokemon* mensaje6;
		unCatchPokemon1->nombre = "CATCH1";
		unCatchPokemon1->coordenadas.posicionX = 5;
		unCatchPokemon1->coordenadas.posicionY = 6;
		printf("Coordenadas catch: %d %d\n",unCatchPokemon1->coordenadas.posicionX,unCatchPokemon1->coordenadas.posicionY);

		mensaje6 = guardarMensajeCatchPokemon(unCatchPokemon1);

	CaughtPokemon* unCaughtPokemon1 = malloc(sizeof(CaughtPokemon));
	MensajeCaughtPokemon* mensaje7;
		int idCorrelativoCaught = 1;
		unCaughtPokemon1->atrapar = 1;

		mensaje7 = guardarMensajeCaughtPokemon(unCaughtPokemon1,idCorrelativoCaught);

*/

	//El sistema tira un error si no se creó correctamente el hilo
		if(pthread_create(&hiloConexion, NULL,(void*)&iniciar_servidor,NULL) != 0){
				errorExit("El hilo no pudo ser creado");
		}


	pthread_join(hiloConexion, NULL); //Puede cambiarse por Detach

	printf("Han finalizado los thread.\n");

	return EXIT_SUCCESS;
}


