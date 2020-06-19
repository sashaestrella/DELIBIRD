#include "adminMensajes.h"

void generarConexiones(int tipoSuscriptor){

	// -- Hilo de suscripcion a cola new -- //
	ParametrosSuscripcion* new = malloc(sizeof(ParametrosSuscripcion));
	new->colaASuscribirse = SUSCRIPTOR_NEWPOKEMON;
	new->nuevoExistente = tipoSuscriptor;

	pthread_t hiloNew;
	pthread_create(&hiloNew, NULL, suscribirseAColaNew, NULL);

	// -- Hilo de suscripcion a catch -- //
	ParametrosSuscripcion* catch = malloc(sizeof(ParametrosSuscripcion));
	catch->colaASuscribirse = SUSCRIPTOR_CATCHPOKEMON;
	catch->nuevoExistente = tipoSuscriptor;

	pthread_t hiloCatch;
	pthread_create(&hiloCatch, NULL, suscribirseAColaCatch, NULL);


	// -- Hilo de suscripcion a cola get -- //
	ParametrosSuscripcion* get = malloc(sizeof(ParametrosSuscripcion));
	get->colaASuscribirse = SUSCRIPTOR_GETPOKEMON;
	get->nuevoExistente = tipoSuscriptor;

	pthread_t hiloGet;
	pthread_create(&hiloGet, NULL, suscribirseAColaGet, NULL);


	pthread_join(hiloNew,NULL);
	pthread_join(hiloGet,NULL);
	pthread_join(hiloCatch,NULL);
	free(new);
	free(get);
	free(catch);
}

void abrirEscuchaNew(){
	pthread_t escuchaNew;
	pthread_create(&escuchaNew, NULL, recibirMensajesNew, NULL);
	pthread_join(escuchaNew, NULL);
}

void abrirEscuchaGet(){
	pthread_t escuchaGet;
	pthread_create(&escuchaGet, NULL, recibirMensajesGet, NULL);
	pthread_join(escuchaGet, NULL);
}

void abrirEscuchaCatch(){
	pthread_t escuchaCatch;
	pthread_create(&escuchaCatch, NULL, recibirMensajesCatch, NULL);
	pthread_join(escuchaCatch, NULL);
}

// ------------------------------------ AUXILIARES ------------------------------------ //

void noHayBroker(){
	//suscribirseACola(conexionGameBoy, 0, 0, msgGameBoy);
	//pthread_t hiloGameBoy;
	//pthread_create(&hiloGameBoy, NULL, recibirMensaje, conexionGameBoy);
	//pthread_join(hiloGameBoy,NULL);
}


void* suscribirseAColaNew(){
	int IDsuscripcion;
	int conexion = crear_conexion(ip, puerto);
	printf( "\nSe creo la suscripcion a cola New con el valor %d \n", conexion);

	enviarSuscripcion(0, conexion, SUSCRIPTOR_NEWPOKEMON);
	recv(conexion, &IDsuscripcion, sizeof(int), MSG_WAITALL);

	printf("Suscriptor numero: %d\n", IDsuscripcion);

	administradorMensajesColas(SUSCRIPTOR_NEWPOKEMON, conexion, IDsuscripcion);
}

void* suscribirseAColaGet(){
	int IDsuscripcion;
	int conexion = crear_conexion(ip, puerto);
	printf( "\nSe creo la suscripcion a cola Get con el valor %d \n", conexion);

	enviarSuscripcion(0, conexion, SUSCRIPTOR_GETPOKEMON);
	recv(conexion, &IDsuscripcion, sizeof(int), MSG_WAITALL);

	printf("Suscriptor numero: %d\n", IDsuscripcion);

	administradorMensajesColas(SUSCRIPTOR_GETPOKEMON, conexion, IDsuscripcion);
}

void* suscribirseAColaCatch(){
	int IDsuscripcion;
	int conexion = crear_conexion(ip, puerto);
	printf( "\nSe creo la suscripcion a cola Catch con el valor %d \n", conexion);

	enviarSuscripcion(0, conexion, SUSCRIPTOR_CATCHPOKEMON);
	recv(conexion, &IDsuscripcion, sizeof(int), MSG_WAITALL);

	printf("Suscriptor numero: %d\n", IDsuscripcion);

	administradorMensajesColas(SUSCRIPTOR_CATCHPOKEMON, conexion, IDsuscripcion);
}



void* administradorMensajesColas(int op_code, int conexion, int IDsuscripcion){

	int cantidadGetPokemon;
	int cantidadNewPokemon;
	int cantidadCatchPokemon;
	uint32_t codigo;
	uint32_t codigo1;
	uint32_t codigo2;
	switch(op_code){

			case SUSCRIPTOR_NEWPOKEMON:
				conexionNewPokemon = conexion;
				IDsuscripcionNew= IDsuscripcion;
				recv(conexion, &cantidadNewPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de New Pokemons: %d\n", cantidadNewPokemon);
				NewPokemonConIDs* nuevoNewPokemonConId;
				for(int i = 0; i<cantidadNewPokemon; i++){
					recv(conexion, &codigo, sizeof(op_code), MSG_WAITALL);
					printf("Codigo de cola: %d\n", codigo);
					nuevoNewPokemonConId = recibir_NEW_POKEMON(conexion, 0, 1);
					send(conexion, 1, sizeof(int), 0); // No se si pasar el 1 con un void*
					adminMensajeNewPokemon(nuevoNewPokemonConId);
					printf("Recibi mensaje com id: %d\n", nuevoNewPokemonConId->IDmensaje);
				}
				abrirEscuchaNew();
				break;

			case SUSCRIPTOR_GETPOKEMON:
				conexionGetPokemon = conexion;
				IDsuscripcionGet= IDsuscripcion;

				recv(conexion, &cantidadGetPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Get Pokemons: %d\n", cantidadGetPokemon);
				recv(conexion, &codigo1, sizeof(op_code), MSG_WAITALL);
				printf("Codigo de cola: %d\n", codigo1);
				GetPokemonConIDs* nuevoGetPokemonConId;
				for(int i = 0; i<cantidadGetPokemon; i++){
					nuevoGetPokemonConId = recibir_GET_POKEMON(conexion, 0, 1);
					send(conexion, 1, sizeof(int), 0);
					adminMensajeGetPokemon(nuevoGetPokemonConId);
				}
				abrirEscuchaGet();
				break;

			case SUSCRIPTOR_CATCHPOKEMON:
				conexionCatch = conexion;
				IDsuscripcionCatch= IDsuscripcion;

				recv(conexion, &cantidadCatchPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Catch Pokemons: %d\n", cantidadCatchPokemon);
				recv(conexion, &codigo2, sizeof(op_code), MSG_WAITALL);
				printf("Codigo de cola: %d\n", codigo2);
				CatchPokemonConIDs* nuevoCatchPokemonConId;
				for(int i = 0; i<cantidadCatchPokemon; i++){
					nuevoCatchPokemonConId = recibir_CATCH_POKEMON(conexion, 0, 1);
					send(conexion, 1, sizeof(int), 0);
					adminMensajeCatch(nuevoCatchPokemonConId);
				}
				abrirEscuchaCatch();
				break;
			default:
				printf("Llego un mensaje invalido");
				break;
	}
}

// ------------------------------- Funciones Game Card ------------------------------------ //

void* recibirMensajesNew(){
	pthread_t admin;
	void* mensajeRecibido;
	NewPokemonConIDs* nuevoNewPokemon;
	while(1){
		int cod_op;
		recv(conexionNewPokemon, &cod_op, sizeof(int), 0);
		nuevoNewPokemon = recibir_NEW_POKEMON(conexionNewPokemon, 0, 1);
		int ack = 1;
		send(conexionNewPokemon, &ack, sizeof(int), 0);
		pthread_create(&admin, NULL, adminMensajeNewPokemon, nuevoNewPokemon);
		pthread_detach(admin);
	}
}

void* adminMensajeNewPokemon(NewPokemonConIDs* nuevoNewPokemon){
	AppearedPokemon* appearedPokemon1 = malloc(sizeof(AppearedPokemon));
	char* nombre = malloc(9);
	nombre = "APPEARED1";
	appearedPokemon1->nombre = nombre;
	appearedPokemon1->coordenadas.posicionX = nuevoNewPokemon->newPokemon->coordenadas.posicionX;
	appearedPokemon1->coordenadas.posicionY = nuevoNewPokemon->newPokemon->coordenadas.posicionY;
	printf("Coordenadas de appeared: %d %d",appearedPokemon1->coordenadas.posicionX,appearedPokemon1->coordenadas.posicionY);
	int id = 0;
	int idCorrelativo = nuevoNewPokemon->IDmensaje;
	int conexion = crear_conexion(ip, puerto);
	enviarAppearedPokemon(appearedPokemon1,conexion,id,idCorrelativo);

	printf("\nEnvie el mensaje: %s\n",appearedPokemon1->nombre);

}

void* recibirMensajesGet(){
	pthread_t admin;
	GetPokemonConIDs* nuevoGetPokemon;
	while(1){
		int cod_op;
		recv(conexionGetPokemon, &cod_op, sizeof(int), 0);
		nuevoGetPokemon = recibir_GET_POKEMON(conexionGetPokemon, 0, 1);
		int ack = 1;
		send(conexionNewPokemon, &ack, sizeof(int), 0);
		pthread_create(&admin, NULL, adminMensajeGetPokemon, nuevoGetPokemon);
		pthread_detach(admin);
	}
}

void* adminMensajeGetPokemon(GetPokemonConIDs* nuevoGetPokemon){
	list_add(mensajesRecibidos, nuevoGetPokemon->IDmensaje);
	list_add(nuevosGetPokemon, nuevoGetPokemon);
	printf("Guarde un mensaje Get");
}

void* recibirMensajesCatch(){
	pthread_t admin;
	CatchPokemonConIDs* nuevoCatch;

	while(1){
		int cod_op;
		recv(conexionCatch, &cod_op, sizeof(int), 0);
		nuevoCatch = recibir_CATCH_POKEMON(conexionCatch, 0, 1);
		int ack = 1;
		send(conexionNewPokemon, &ack, sizeof(int), 0);
		pthread_create(&admin, NULL, adminMensajeCatch, nuevoCatch);
		pthread_detach(admin);
	}
}

void* adminMensajeCatch(CatchPokemonConIDs* nuevoCatch){
	list_add(mensajesRecibidos, nuevoCatch->IDmensaje);
	list_add(nuevosCatch, nuevoCatch);
	printf("Guarde un mensaje Catch");
}


