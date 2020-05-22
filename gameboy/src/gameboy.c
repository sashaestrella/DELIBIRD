

#include "game-boy.h"

int main(void)
{

	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;


	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	char* primerLog = "Soy un log";

	log_info(logger, primerLog);

	//Loggear "soy un log"

	config = leer_config();

	config_set_value(config, "IP", "127.0.0.1");
	config_set_value(config, "PUERTO", "4444");
	config_save(config);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	//antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	//crear conexion

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	conexion = crear_conexion(ip, puerto);

	printf( "\nSe creo la conexion con el valor %d \n", conexion);

	//enviar mensaje
/*
	NewPokemon* unNewPokemon = malloc(sizeof(NewPokemon));
	char* nombrePokemon = "PIKACHU";
	unNewPokemon->nombre = nombrePokemon;
	unNewPokemon->coordenadas.posicionX = 2;
	unNewPokemon->coordenadas.posicionY = 3;
	unNewPokemon->cantidad = 3;
	enviarNewPokemon(unNewPokemon, conexion);

	printf("\nEnvie el mensaje: %s\n",unNewPokemon->nombre);

*/
	LocalizedPokemon* localizedPokemon1 = malloc(sizeof(LocalizedPokemon));
	localizedPokemon1->nombre = "PIKACHU";
	localizedPokemon1->cantidadParesOrdenados = 2;
	int numero = 1;
		t_list* pares = list_create();
		list_add(pares,&numero);
		list_add(pares,&numero);
		list_add(pares,&numero);
		list_add(pares,&numero);
	localizedPokemon1->paresOrdenados = pares;
	enviarLocalizedPokemon(localizedPokemon1,conexion);

	printf("\nEnvie el mensaje: %s\n",localizedPokemon1->nombre);

/*
	int cod_op = 8;

	//enviarSuscripcionNewPokemon(conexion);
	send(conexion,&cod_op,sizeof(int),0);
	puts("\nSuscripcion a NewPokemon enviada");


	int idSuscriptor;
	recv(conexion,&idSuscriptor,sizeof(int),0);

	printf("\nRecibi mi id como suscriptor: %d\n",idSuscriptor);
	int ack = 1;
	int tamanioListaNP;
	int IDmensajeNP;
	recv(conexion,&tamanioListaNP,sizeof(int),0);
	printf("\nEl tamaño de la lista que voy a recibir es: %d",tamanioListaNP);
	int size;
	int variableQueNoUsoxd;
	NewPokemon* unNewPokemonTemporal;
	for(int i = 0; i<tamanioListaNP;i++){
		recv(conexion,&IDmensajeNP,sizeof(int),0);
		printf("\nRecibi el ID del mensaje,el cual es: %d",IDmensajeNP);
		recv(conexion,&variableQueNoUsoxd,sizeof(int),0);
		unNewPokemonTemporal = recibir_NEW_POKEMON(conexion,&size);
		printf("\n[gameboy] Recibi un %s",unNewPokemonTemporal->nombre);
		send(conexion,&ack,sizeof(int),0);
		printf("\nEl ack fue enviado\n");

	}
	free(unNewPokemonTemporal);
*/

/*
	int tamanio_maximo_mensaje = 256;

	char* mensajeAEnviar = malloc(tamanio_maximo_mensaje);

	printf("\nIngrese el mensaje:");

	fgets(mensajeAEnviar,tamanio_maximo_mensaje,stdin);

	printf("Se va a enviar el mensaje %s", mensajeAEnviar);

	enviar_mensaje(mensajeAEnviar, conexion); //Envia el mensaje

	//recibir mensaje

	char* mensajeRecibido;

	mensajeRecibido=recibir_mensaje(conexion);


	//loguear mensaje recibido

	log_info(logger, "Se recibio el mensaje: %s " ,mensajeRecibido);*/

	terminar_programa(conexion, logger, config);
}

//TODO
t_log* iniciar_logger(void)
{
	return log_create("gameboy.log", "Log de GameBoy" , 1, LOG_LEVEL_INFO);

}

//TODO
t_config* leer_config(void)
{
	return config_create("gameboy.config");

}

//TODO
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);

	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
}
