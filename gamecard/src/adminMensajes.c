#include "adminMensajes.h"

void generarConexiones(int tipoSuscriptor){
	crearDirectorioTG();
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

// ------------------------------------ SUSCRIPCIONES ------------------------------------ //

void noHayBroker(){

}


void* suscribirseAColaNew(){
	int IDsuscripcion;
	sem_wait(&suscripciones);
	int conexion = crear_conexion(ip, puerto);
	printf( "\nSe creo la suscripcion a cola New con el valor %d \n", conexion);

	enviarSuscripcion(0, conexion, SUSCRIPTOR_NEWPOKEMON);
	recv(conexion, &IDsuscripcion, sizeof(int), MSG_WAITALL);
	sem_post(&suscripciones);
	printf("Suscriptor numero: %d\n", IDsuscripcion);

	administradorMensajesColas(SUSCRIPTOR_NEWPOKEMON, conexion, IDsuscripcion);
}

void* suscribirseAColaGet(){
	int IDsuscripcion;
	sem_wait(&suscripciones);
	int conexion = crear_conexion(ip, puerto);
	printf( "\nSe creo la suscripcion a cola Get con el valor %d \n", conexion);

	enviarSuscripcion(0, conexion, SUSCRIPTOR_GETPOKEMON);
	recv(conexion, &IDsuscripcion, sizeof(int), MSG_WAITALL);
	sem_post(&suscripciones);
	printf("Suscriptor numero: %d\n", IDsuscripcion);

	administradorMensajesColas(SUSCRIPTOR_GETPOKEMON, conexion, IDsuscripcion);
}

void* suscribirseAColaCatch(){
	int IDsuscripcion;
	sem_wait(&suscripciones);
	int conexion = crear_conexion(ip, puerto);
	printf( "\nSe creo la suscripcion a cola Catch con el valor %d \n", conexion);

	enviarSuscripcion(0, conexion, SUSCRIPTOR_CATCHPOKEMON);
	recv(conexion, &IDsuscripcion, sizeof(int), MSG_WAITALL);
	sem_post(&suscripciones);
	printf("Suscriptor numero: %d\n", IDsuscripcion);

	administradorMensajesColas(SUSCRIPTOR_CATCHPOKEMON, conexion, IDsuscripcion);
}

// ------------------------------------ RECEPCION MENSAJES ------------------------------------ //

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

// ------------------------------------ ADMIN MENSAJES ------------------------------------ //

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
	armarFolderPara(nuevoNewPokemon);
}

void* adminMensajeGetPokemon(GetPokemonConIDs* nuevoGetPokemon){
	list_add(mensajesRecibidos, nuevoGetPokemon->IDmensaje);
	list_add(nuevosGetPokemon, nuevoGetPokemon);
	printf("Guarde un mensaje Get");
}

void* adminMensajeCatch(CatchPokemonConIDs* nuevoCatch){
	list_add(mensajesRecibidos, nuevoCatch->IDmensaje);
	list_add(nuevosCatch, nuevoCatch);
	printf("Guarde un mensaje Catch");
}

// --------------------- FILE SYSTEM --------------------- //

void* crearDirectorioTG(){
	char* path = string_new();
	string_append(&path,puntoMontaje);
	string_append(&path, "/TALL_GRASS");
	mkdir(path, 0777);

	crearMetadata(path);
	crearFiles(path);
	crearBlocks(path);
}

void* crearMetadata(char* pathOrigin){
	FILE* metadata;
	char* path = string_duplicate(pathOrigin);

	string_append(&path,"/Metadata");
	char* pathMetadata = string_duplicate(path);
	mkdir(path, 0777);
	string_append(&path, "/Metadata.bin");
	metadata = fopen(path, "wrb");
	fclose(metadata);


	t_config* md = config_create(path);
	config_set_value(md, "BLOCK_SIZE", "64");
	config_set_value(md, "BLOCKS", "5192");
	config_set_value(md, "MAGIC_NUMBER", "TALL_GRASS");
	config_save(md);

	generarBitmap(pathMetadata, md);
}

void* crearFiles(char* pathOrigin){
	FILE* metadata;
	char* path = string_duplicate(pathOrigin);

	string_append(&path, "/Files");
	mkdir(path, 0777);
	string_append(&path, "/Metadata.bin");
	metadata = fopen(path, "wrb");
	fclose(metadata);

	t_config* md = config_create(path);
	config_set_value(md, "DIRECTORY", "Y");
	config_save(md);
}

void* crearBlocks(char* path){
	string_append(&path, "/Blocks");
	mkdir(path, 0777);
}

void* generarBitmap(char* path, t_config* md){
	// GENERAR BITMAP ------------------------------------ !!!
	int cantidadDeBloques = 0;
	FILE* bitmapFile;
	char** bloques = config_get_array_value(md, "BLOCKS");

	char* size = config_get_array_value(md, "BLOCK_SIZE");

	int tamanioDeBloque = atoi(size);

	while(bloques[cantidadDeBloques] != NULL){
		cantidadDeBloques++;
	}

	string_append(&path, "/Bitmap.bin");
	bitmapFile = fopen(path, "wrb");
	fclose(bitmapFile);

	t_bitarray* bitmap = bitarray_create(bitmapFile, cantidadDeBloques * tamanioDeBloque);

	for(int i = 0; i < cantidadDeBloques; i++){
		bitarray_set_bit(bitmap, i);
	}

	puts(bitarray_get_max_bit(bitmap));
}

void* armarFolderPara(char* nombre){
	char* path = string_new();
	string_append(&path,puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, nombre);
	mkdir(path, 0777);
}

void* crearMetadataPara(char* nombre){
	FILE* metadata;
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, nombre);
	string_append(&path, "/Metadata.bin");

	armarFolderPara(nombre);
	metadata = fopen(path, "wrb");
	fclose(metadata);

	t_config* md = config_create(path);
	config_set_value(md, "DIRECTORY", "N");
	config_set_value(md, "SIZE", "0");
	config_set_value(md, "BLOCKS", "[]");
	config_set_value(md, "OPEN", "N");
	config_save(md);
}

// --------------------- INTERACCION CON FS --------------------- //

int archivoAbierto(char* path){
	t_config* md = config_create(path);
	return config_get_string_value(md, "OPEN") == "Y";
}

int existePokemon(char* nombre){
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, nombre);
	string_append(&path, "/Metadata.bin");

	FILE* archivo = fopen(path, "r");
	if (archivo) {
		fclose(archivo);
	    return 1;
	} else {
	    fclose(archivo);
	    return 0;
	}
}

void agregarPokemon(NewPokemonConIDs* newPokemon){
	FILE* metadata;
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, newPokemon->newPokemon->nombre);
	string_append(&path, "/Metadata.bin");

	if(!existePokemon(newPokemon->newPokemon->nombre)){
		crearMetadataPara(newPokemon);
	} else {
		t_config* md = config_create(path);
		while(archivoAbierto(path)){
			sleep(tiempoReintento);
		}
		config_set_value(md,"OPEN","Y");
		config_save(md);

		char** bloques = config_get_array_value(md, "BLOCKS");
		char* cursor = string_duplicate(path);

		// Agregar Bloque -------------------------------_ !!

		sleep(tiempoRetardo);
		config_set_value(md,"OPEN","N");
		config_save(md);
		enviarMensajeAppeared(newPokemon->IDmensaje, newPokemon->newPokemon->nombre, newPokemon->newPokemon->coordenadas);
	}
}

int existePosicion(char** bloque, CoordenadasXY coordenadas){
	return 1;
}


void eliminarPokemon(CatchPokemonConIDs* pokemon){
	FILE* metadata;
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, pokemon->catchPokemon->nombre);
	string_append(&path, "/Metadata.bin");
	int encontrado = 0;

	char* error = string_new;
	string_append(&error ,"No existe el pokemon ");
	string_append(&error ,pokemon->catchPokemon->nombre);

	if(!existePokemon(pokemon->catchPokemon->nombre)){
		log_info(logger,error);
	} else {
		t_config* md = config_create(path);
		while(archivoAbierto(path)){
			sleep(tiempoReintento);
		}
		config_set_value(md,"OPEN","Y");
		config_save(md);
		char** bloques = config_get_array_value(md, "BLOCKS");
		int i = 0;
		while(bloques[i]!=NULL && !existePosicion(bloques[i], pokemon->catchPokemon->coordenadas)){
			i++;
		}
		if(bloques[i] == NULL){
			log_info(logger,error);

		} else {

			// Disminuir cantidad ---------------------- !!

			encontrado = 1;
		}
	sleep(tiempoRetardo);
	config_set_value(md,"OPEN","N");
	config_save(md);
	}
	enviarMensajeCaught(pokemon->IDmensaje, encontrado);
}

LocalizedPokemon* obtenerCantidadYPosiciones(GetPokemonConIDs* pokemon){
	FILE* metadata;
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, pokemon->getPokemon->nombre);
	string_append(&path, "/Metadata.bin");
	int encontrado = 0;
	char** bloques;

	char* error = string_new;
	string_append(&error ,"No existe el pokemon ");
	string_append(&error ,pokemon->getPokemon->nombre);

	if(!existePokemon(pokemon->getPokemon->nombre)){
		log_info(logger,error);
		// Enviar mensaje sin posiciones ni cantidades ----------------------------- !!
	} else {
		t_config* md = config_create(path);
			while(archivoAbierto(path)){
				sleep(tiempoReintento);
			}
			config_set_value(md,"OPEN","Y");
			config_save(md);
			bloques = config_get_array_value(md, "BLOCKS");
			sleep(tiempoRetardo);

			config_set_value(md,"OPEN","N");
			config_save(md);

			int i = 0;
			t_list* posiciones = list_create();
			while(bloques[i]!=NULL){
				i++;
				obtenerPosiciones(pokemon->IDmensaje, pokemon->getPokemon->nombre, bloques[i]);
			}
	}

}

t_list* obtenerPosiciones(int IDmensaje, char* nombre, char* Bloque){
	// ------------------------- Obtener posiciones y cantidad y enviarlas ------------------!!
}



// --------------------- Enviar Mensajes --------------------- //

int enviarMensajeAppeared(int IDmensaje, char* pokemon, CoordenadasXY coordenadas){
	AppearedPokemon* nuevo = parsearAppearedPokemon(pokemon, coordenadas.posicionX, coordenadas.posicionY);
	//enviarAppearedPokemon(nuevo, socket_suscriptor, 0, IDmensaje);
}

int enviarMensajeCaught(int IDmensaje, int resultado){
	CaughtPokemon* nuevo = malloc(sizeof(CaughtPokemon));
	nuevo->atrapar = resultado;
	//enviarCaughtPokemon(nuevo, socket_suscriptor, 0, IDmensaje)
}

int enviarMensajeLocalized(int IDmensaje, char* pokemon, CoordenadasXY coordenadas, int cantidad){
	LocalizedPokemon* nuevo = malloc(sizeof(LocalizedPokemon));
	nuevo->cantidadParesOrdenados = cantidad;
	// ----------------------------------- Ver que hacemos con pares ordenados!!
	nuevo->tamanioNombrePokemon = sizeof(pokemon)+1;
	nuevo->nombre = pokemon;
	//enviarLocalizedPokemon(nuevo, socket_suscriptor, 0, IDmensaje);
}


