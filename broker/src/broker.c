#include "broker.h"

void recibirSenial(int senial){
	int pid = getpid();
	if(senial == SIGUSR1){
		printf("\n\nProceso %d: señal SIGUSR1 recibida.\n",pid);
		dumpEnCache();
	}
}

void dumpEnCache(){
	FILE *archivoDump;
	PosicionLibre* unaPosicionLibre;
	PosicionOcupada* unaPosicionOcupada;
	int tamanioListaPosicionesLibres = list_size(listaPosicionesLibres);
	int tamanioListaPosicionesOcupadas = list_size(listaPosicionesOcupadas);
	time_t tiempo;
	struct tm* tm;
	char fechaYHora[128];
	char* loQueVoyALoguear;

	archivoDump = txt_open_for_append("/home/utnso/tp-2020-1c-BOMP/broker/dumpDeLaCache.txt");
	tiempo = time(NULL);
	tm = localtime(&tiempo);

	strftime(fechaYHora,128,"\nDump: %d/%m/%y %H:%M:%S",tm);
	printf("%s\n",fechaYHora);
	txt_write_in_file(archivoDump,fechaYHora);
	fprintf(archivoDump,"\nInicio de la memoria: %p",memoriaInterna);

	for(int i=0;i<tamanioListaPosicionesLibres;i++){
		unaPosicionLibre = list_get(listaPosicionesLibres,i);
		fprintf(archivoDump,"\nPartición %d: %p-%p.",i,unaPosicionLibre->posicion,unaPosicionLibre->posicion+(unaPosicionLibre->tamanio)-1);
		fputs("\t\t[L]",archivoDump);
		fprintf(archivoDump,"\t\tSize: %db",unaPosicionLibre->tamanio);
	}

	for(int j=0;j<tamanioListaPosicionesOcupadas;j++){
		unaPosicionOcupada = list_get(listaPosicionesOcupadas,j);
		fprintf(archivoDump,"\nPartición %d: %p-%p.",j,unaPosicionOcupada->posicion,unaPosicionOcupada->posicion+(unaPosicionOcupada->tamanio)-1);
		fputs("\t\t[X]",archivoDump);
		fprintf(archivoDump,"\t\tSize: %db",unaPosicionOcupada->tamanio);
		fprintf(archivoDump,"\t\tLRU: %d",unaPosicionOcupada->timestamp);
		fprintf(archivoDump,"\t\tCola: %d",unaPosicionOcupada->colaALaQuePertenece);
		fprintf(archivoDump,"\t\tID: %d",unaPosicionOcupada->ID);
	}
	fprintf(archivoDump,"\nEn posiciones relativas: ");
	for(int j=0;j<tamanioListaPosicionesOcupadas;j++){
		unaPosicionOcupada = list_get(listaPosicionesOcupadas,j);
		fprintf(archivoDump,"\nPartición %d: %i-%i.",j,unaPosicionOcupada->posicion-memoriaInterna,(unaPosicionOcupada->posicion-memoriaInterna)+(unaPosicionOcupada->tamanio)-1);
		fputs("\t\t[X]",archivoDump);
		fprintf(archivoDump,"\t\tSize: %db",unaPosicionOcupada->tamanio);
		fprintf(archivoDump,"\t\tLRU: %d",unaPosicionOcupada->timestamp);
		fprintf(archivoDump,"\t\tCola: %d",unaPosicionOcupada->colaALaQuePertenece);
		fprintf(archivoDump,"\t\tID: %d",unaPosicionOcupada->ID);
	}

	loQueVoyALoguear = "Se solicitó ejecución de Dump de Caché.";
	log_info(logger, loQueVoyALoguear);

	txt_close_file(archivoDump);

}

void errorExit(char* strerr){
	perror(strerr);
	exit(1);
}

int main(void){

	signal(SIGUSR1,recibirSenial);

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

	config= config_create("broker.config");
	IP_BROKER = config_get_string_value(config, "IP_BROKER");
	PUERTO_BROKER = config_get_string_value(config,"PUERTO_BROKER");
	tamanioMinimoParticion = atoi(config_get_string_value(config, "TAMANIO_MINIMO_PARTICION"));
	tamanioMemoria = atoi(config_get_string_value(config, "TAMANIO_MEMORIA"));
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
	busquedasFallidasPreviasACompactacionOriginal = busquedasFallidasPreviasACompactacion;
	printf("FRECUENCIA_COMPACTACION = %d\n",busquedasFallidasPreviasACompactacion);
	char* pathDelLogger = config_get_string_value(config,"LOG_FILE");
	printf("LOG_FILE = %s\n",pathDelLogger);
	logger = log_create(pathDelLogger, "Broker-Log" , 1, LOG_LEVEL_INFO);

	printf("------------FIN DE ARCHIVO DE CONFIGURACION BROKER\n");


	//El sistema tira un error si no se creó correctamente el hilo
		if(pthread_create(&hiloConexion, NULL,(void*)&iniciar_servidor,NULL) != 0){
				errorExit("El hilo no pudo ser creado");
		}


	pthread_join(hiloConexion, NULL); //Puede cambiarse por Detach

	printf("Han finalizado los thread.\n");

	return EXIT_SUCCESS;
}


