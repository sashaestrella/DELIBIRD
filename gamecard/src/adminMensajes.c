#include "adminMensajes.h"

void abrirAtenciones(){
	pthread_t atencionNew;
	pthread_create(&atencionNew, NULL, atenderMensajesNew, NULL);
	pthread_detach(atencionNew);

	pthread_t atencionCatch;
	pthread_create(&atencionCatch, NULL, atenderMensajesCatch, NULL);
	pthread_detach(atencionCatch);

	pthread_t atencionGet;
	pthread_create(&atencionGet, NULL, atenderMensajesGet, NULL);
	pthread_detach(atencionGet);
}

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
	printf("Suscriptor numero: %d\n\n", IDsuscripcion);

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
				IDsuscripcionNew= IDsuscripcion;
				recv(conexion, &cantidadNewPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de New Pokemons: %d\n", cantidadNewPokemon);
				NewPokemonConIDs* nuevoNewPokemonConId;
				for(int i = 0; i<cantidadNewPokemon; i++){
					recv(conexion, &codigo, sizeof(op_code), MSG_WAITALL);
					printf("Codigo de cola: %d\n", codigo);
					nuevoNewPokemonConId = recibir_NEW_POKEMON(conexion, 0, 1);
					int ack=1;
					send(conexion, &ack, sizeof(int), 0);
					adminMensajeNewPokemon(nuevoNewPokemonConId);
					printf("Recibi mensaje com id: %d\n", nuevoNewPokemonConId->IDmensaje);
				}
				recibirMensajesNew(conexion);
				break;

			case SUSCRIPTOR_GETPOKEMON:
				IDsuscripcionGet= IDsuscripcion;

				recv(conexion, &cantidadGetPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Get Pokemons: %d\n", cantidadGetPokemon);
				GetPokemonConIDs* nuevoGetPokemonConId;
				for(int i = 0; i<cantidadGetPokemon; i++){
					recv(conexion, &codigo1, sizeof(op_code), MSG_WAITALL);
					printf("Codigo de cola: %d\n", codigo1);
					nuevoGetPokemonConId = recibir_GET_POKEMON(conexion, 0, 1);
					int ack=1;
					send(conexion, &ack, sizeof(int), 0);
					printf("%s", nuevoGetPokemonConId->getPokemon->nombre);
					adminMensajeGetPokemon(nuevoGetPokemonConId);
				}
				recibirMensajesGet(conexion);
				break;

			case SUSCRIPTOR_CATCHPOKEMON:
				IDsuscripcionCatch= IDsuscripcion;

				recv(conexion, &cantidadCatchPokemon, sizeof(int), MSG_WAITALL);
				printf("Cantidad de Catch Pokemons: %d\n", cantidadCatchPokemon);
				CatchPokemonConIDs* nuevoCatchPokemonConId;
				for(int i = 0; i<cantidadCatchPokemon; i++){
					recv(conexion, &codigo2, sizeof(op_code), MSG_WAITALL);
					printf("Codigo de cola: %d\n", codigo2);
					nuevoCatchPokemonConId = recibir_CATCH_POKEMON(conexion, 0, 1);
					int ack=1;
					send(conexion, &ack, sizeof(int), 0);
					adminMensajeCatch(nuevoCatchPokemonConId);
				}
				recibirMensajesCatch(conexion);
				break;
			default:
				printf("Llego un mensaje invalido");
				break;
	}
}

void* recibirMensajesNew(int conexionNewPokemon){
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

void* recibirMensajesGet(int conexionGetPokemon){
	pthread_t admin;
	GetPokemonConIDs* nuevoGetPokemon;
	while(1){
		int cod_op;
		recv(conexionGetPokemon, &cod_op, sizeof(int), 0);
		nuevoGetPokemon = recibir_GET_POKEMON(conexionGetPokemon, 0, 1);
		int ack = 1;
		send(conexionGetPokemon, &ack, sizeof(int), 0);
		pthread_create(&admin, NULL, adminMensajeGetPokemon, nuevoGetPokemon);
		pthread_detach(admin);
	}
}

void* recibirMensajesCatch(int conexionCatch){
	pthread_t admin;
	CatchPokemonConIDs* nuevoCatch;

	while(1){
		int cod_op;
		recv(conexionCatch, &cod_op, sizeof(int), 0);
		nuevoCatch = recibir_CATCH_POKEMON(conexionCatch, 0, 1);
		int ack = 1;
		send(conexionCatch, &ack, sizeof(int), 0);
		pthread_create(&admin, NULL, adminMensajeCatch, nuevoCatch);
		pthread_detach(admin);
	}
}

// ------------------------------------ ADMIN MENSAJES ------------------------------------ //

void* adminMensajeNewPokemon(NewPokemonConIDs* nuevoNewPokemon){
	list_add(mensajesRecibidos, nuevoNewPokemon->IDmensaje);
	list_add(mensajesNew, nuevoNewPokemon);
	printf("Guarde un mensaje New\n\n");
	sem_post(&colaNew);
}

void* adminMensajeGetPokemon(GetPokemonConIDs* nuevoGetPokemon){

	list_add(mensajesRecibidos, nuevoGetPokemon->IDmensaje);
	list_add(mensajesGet, nuevoGetPokemon);
	printf("Guarde un mensaje Get\n\n");
	sem_post(&colaGet);

}

void* adminMensajeCatch(CatchPokemonConIDs* nuevoCatch){

	list_add(mensajesRecibidos, nuevoCatch->IDmensaje);
	list_add(mensajesCatch, nuevoCatch);
	printf("Guarde un mensaje Catch\n\n");
	sem_post(&colaCatch);
}

// --------------------- ATENDER MENSAJES --------------------- //

void* atenderMensajesNew(){
	while(1){

		sem_wait(&colaNew);
		NewPokemonConIDs* mensajeLeido = malloc(sizeof(NewPokemonConIDs));
		mensajeLeido = (NewPokemonConIDs*)list_remove(mensajesNew ,0);
		agregarPokemon(mensajeLeido);

	}
}

void* atenderMensajesCatch(){
	while(1){

		sem_wait(&colaCatch);
		CatchPokemonConIDs* mensajeLeido = malloc(sizeof(CatchPokemonConIDs));
		mensajeLeido = (CatchPokemonConIDs*)list_remove(mensajesCatch ,0);
		eliminarPokemon(mensajeLeido);

	}
}

void* atenderMensajesGet(){
	while(1){

		sem_wait(&colaGet);
		GetPokemonConIDs* mensajeLeido = malloc(sizeof(GetPokemonConIDs));
		mensajeLeido = (GetPokemonConIDs*)list_remove(mensajesGet ,0);
		obtenerCantidadYPosiciones(mensajeLeido);

	}
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

	int cantidadDeBloques = config_get_int_value(md, "BLOCKS");

	int tamanioDeBloque = config_get_int_value(md, "BLOCK_SIZE");

	string_append(&path, "/Bitmap.bin");
	FILE* bitmapFile = fopen(path, "wrb");

	void* punteroABitmap = malloc(cantidadDeBloques/8);

	t_bitarray* bitmap = bitarray_create_with_mode(punteroABitmap, cantidadDeBloques, MSB_FIRST);

	for(int i = 0; i < cantidadDeBloques; i++){
		bitarray_clean_bit(bitmap,i);
	}
	fwrite(punteroABitmap, 1, cantidadDeBloques/8, bitmapFile);
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

int existePokemon(char* nombreOriginal){
	char* nombre = string_duplicate(nombreOriginal);
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");

	string_append(&path, nombre);

	string_append(&path, "/Metadata.bin");


	FILE* archivo = fopen(path, "r");
	if (archivo == NULL) {
		//fclose(archivo);
	    return 0;
	} else {
	    fclose(archivo);
	    return 1;
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
		crearMetadataPara(newPokemon->newPokemon->nombre);
		printf("Cree metadata para %s\n\n", newPokemon->newPokemon->nombre);
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
		printf("Aca se debería agregar al FS al pokemon %s\n\n", newPokemon->newPokemon->nombre);

		config_set_value(md,"OPEN","N");
		config_save(md);
	}
	sleep(tiempoRetardo);
	enviarMensajeAppeared(newPokemon->IDmensaje, newPokemon->newPokemon->nombre, newPokemon->newPokemon->coordenadas);
	printf("Envie un appeared\n\n");
}

int existePosicion(char** bloque, CoordenadasXY coordenadas){
	return 1; // -------------------------------------------------!!
}


void eliminarPokemon(CatchPokemonConIDs* pokemon){
	FILE* metadata;
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, pokemon->catchPokemon->nombre);
	string_append(&path, "/Metadata.bin");
	int encontrado = 0;

	char* error = string_new();
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
			puts("No hay pokemon en esa posicion");
		} else {

			// Disminuir cantidad ---------------------- !!
			printf("Aca se debería disminuir la cantidad de %s en el FL \n\n", pokemon->catchPokemon->nombre);
			encontrado = 1;

		}
		config_set_value(md,"OPEN","N");
		config_save(md);
	}
	sleep(tiempoRetardo);
	enviarMensajeCaught(pokemon->IDmensaje, encontrado);
	printf("Envie un Caught\n\n");
}

void obtenerCantidadYPosiciones(GetPokemonConIDs* pokemon){

	FILE* metadata;
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, pokemon->getPokemon->nombre);
	string_append(&path, "/Metadata.bin");
	int encontrado = 0;
	char** bloques;

	char* error = string_new();
	string_append(&error ,"No existe el pokemon ");
	string_append(&error ,pokemon->getPokemon->nombre);
	if(!existePokemon(pokemon->getPokemon->nombre)){
		log_info(logger,"%s",error);
		t_list* paresOrdenados = list_create();

		sleep(tiempoRetardo);
		enviarMensajeLocalized(pokemon->IDmensaje, pokemon->getPokemon->nombre, paresOrdenados);

		list_destroy(paresOrdenados);
		printf("Envie un localized vacio\n\n");
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
			t_list* coordenadas = list_create();
			t_list* posiciones = list_create();
			while(bloques[i]!=NULL){
				i++;
				posiciones = obtenerPosiciones(pokemon->IDmensaje, pokemon->getPokemon->nombre, bloques[i]);
				list_add_all(coordenadas ,posiciones);
			}
			sleep(tiempoRetardo);
			enviarMensajeLocalized(pokemon->IDmensaje, pokemon->getPokemon->nombre, coordenadas);
			list_destroy(posiciones);
			list_destroy(coordenadas);
			printf("Envie un localized\n\n");
	}

}

t_list* obtenerPosiciones(int IDmensaje, char* nombre, char* Bloque){
	t_list* posiciones = list_create();
	printf("Aca se deben obtener las posiciones de %s \n\n", nombre);
	// ------------------------- Obtener posiciones y cantidad y enviarlas ------------------!!
	return posiciones;
}



// --------------------- Enviar Mensajes --------------------- //

void enviarMensajeAppeared(int IDmensaje, char* pokemon, CoordenadasXY coordenadas){
	printf("Aca llegue\n\n");
	AppearedPokemon* nuevo = malloc(sizeof(AppearedPokemon));
	nuevo->coordenadas = coordenadas;
	nuevo->nombre = pokemon;
	nuevo->tamanioNombrePokemon = strlen(pokemon) + 1;
	int socket_suscriptor = crear_conexion(ip, puerto);
	enviarAppearedPokemon(nuevo, socket_suscriptor, 0, IDmensaje);
	liberar_conexion(socket_suscriptor);
}

void enviarMensajeCaught(int IDmensaje, int resultado){
	CaughtPokemon* nuevo = malloc(sizeof(CaughtPokemon));
	nuevo->atrapar = resultado;
	int socket_suscriptor = crear_conexion(ip, puerto);
	enviarCaughtPokemon(nuevo, socket_suscriptor, 0, IDmensaje);
	liberar_conexion(socket_suscriptor);
}

void enviarMensajeLocalized(int IDmensaje, char* pokemon, t_list* coordenadas){
	LocalizedPokemon* nuevo = malloc(sizeof(LocalizedPokemon));
	nuevo->cantidadParesOrdenados = list_size(coordenadas);
	nuevo->tamanioNombrePokemon = sizeof(pokemon)+1;
	nuevo->nombre = pokemon;
	nuevo->paresOrdenados = coordenadas;
	int socket_suscriptor = crear_conexion(ip, puerto);
	enviarLocalizedPokemon(nuevo, socket_suscriptor, 0, IDmensaje);
	printf(" para el broker\n\n");
	liberar_conexion(socket_suscriptor);
	free(nuevo);
}

// --------------------- BITMAP --------------------- //

int proximoDisponible(){
	int i=0;
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/Metadata");
	string_append(&path, "/Bitmap.bin");
	FILE* bitmapFile = fopen(path, "wrb");

	//void* punteroABitmap = malloc(cantidadDeBloques/8);

	//t_bitarray* bitmap = bitarray_create_with_mode(punteroABitmap, cantidadDeBloques, MSB_FIRST);

	//bitarray_test_bit(t_bitarray*, off_t bit_index);

	//if(i == bitarray_get_max_bit())

	return i;
}


