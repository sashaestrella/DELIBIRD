#include "adminMensajes.h"

// ------------------------------------ CONEXION GAMEBOY ------------------------------------ //

void noHayBroker(){
	iniciar_servidor_gamecard();
}

void iniciar_servidor_gamecard(void){
	int socket_servidor;

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(miIP, miPUERTO, &hints, &servinfo);

    for (p=servinfo; p != NULL; p = p->ai_next)
    {
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_servidor);
            continue;
        }
        break;
    }

	listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);

    while(1)
    	esperar_cliente_gamecard(socket_servidor);
}

void esperar_cliente_gamecard(int socket_servidor) {
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&thread,NULL,(void*)serve_client_gamecard,&socket_cliente);
	pthread_detach(thread);

}

void serve_client_gamecard(int* socket) {
	int cod_op;

	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1) cod_op = -1;
	process_request_gamecard(cod_op, *socket);
}

void process_request_gamecard(int cod_op, int cliente_fd) {
	int size;
	void* algoARecibir;
	pthread_t admin;

	NewPokemonConIDs* newConIDs;
	GetPokemonConIDs* getConIDs;
	CatchPokemonConIDs* catchConIDs;

	char* loQueVoyALoguear;
	int idCorrelativo;
		switch (cod_op) {
				case 0:
					pthread_exit(NULL);
				case -1:
					pthread_exit(NULL);
				case NEW_POKEMON:
					newConIDs = recibir_NEW_POKEMON(cliente_fd, &size,1);
					adminMensajeNewPokemon(newConIDs);
					break;
				case GET_POKEMON:
					getConIDs = recibir_GET_POKEMON(cliente_fd,&size,1);
					adminMensajeGetPokemon(getConIDs);
					break;
				case CATCH_POKEMON:
					catchConIDs = recibir_CATCH_POKEMON(cliente_fd,&size,1);
					adminMensajeCatch(catchConIDs);
					break;
	}
}


// ------------------------------------ CONEXION BROKER ------------------------------------ //

void* conexionConBroker(){
	generarConexiones();
	while(1){
		sem_wait(&reintentoNew);
		sem_wait(&reintentoGet);
		sem_wait(&reintentoCatch);
		int conexion = crear_conexion(ip,puerto);
		while(conexion == -1){
			sleep(tiempoReconexion);
			conexion = crear_conexion(ip,puerto);
		}
		close(conexion);
		IDsuscripcionCatch = 0;
		IDsuscripcionGet = 0;
		IDsuscripcionNew = 0;
		generarConexiones();
	}
}

void generarConexiones(){
	// -- Hilo de suscripcion a cola new -- //

	pthread_t hiloNew;
	pthread_create(&hiloNew, NULL, suscribirseAColaNew, NULL);

	// -- Hilo de suscripcion a catch -- //

	pthread_t hiloCatch;
	pthread_create(&hiloCatch, NULL, suscribirseAColaCatch, NULL);


	// -- Hilo de suscripcion a cola get -- //

	pthread_t hiloGet;
	pthread_create(&hiloGet, NULL, suscribirseAColaGet, NULL);


	pthread_join(hiloNew,NULL);
	pthread_join(hiloGet,NULL);
	pthread_join(hiloCatch,NULL);
}

// ------------------------------------ SUSCRIPCIONES BROKER ------------------------------------ //


void* suscribirseAColaNew(){
	int IDsuscripcion;
	sem_wait(&suscripciones);
	int conexion = crear_conexion(ip, puerto);
	if(conexion!=-1){

		enviarSuscripcion(IDsuscripcionNew, conexion, SUSCRIPTOR_NEWPOKEMON);
		recv(conexion, &IDsuscripcion, sizeof(int), MSG_WAITALL);
		char* id = string_itoa(IDsuscripcion);
		//config_set_value(config, "ID_SUSCRIPCION_NEW", id);
		config_save(config);
		free(id);
		sem_post(&suscripciones);
		administradorMensajesColas(SUSCRIPTOR_NEWPOKEMON, conexion, IDsuscripcion);
	} else {
		sem_post(&reintentoNew);
		sem_post(&suscripciones);
	}
}

void* suscribirseAColaGet(){
	int IDsuscripcion;
	sem_wait(&suscripciones);
	int conexion = crear_conexion(ip, puerto);
	if(conexion != -1){

		enviarSuscripcion(IDsuscripcionGet, conexion, SUSCRIPTOR_GETPOKEMON);
		recv(conexion, &IDsuscripcion, sizeof(int), MSG_WAITALL);
		char* id = string_itoa(IDsuscripcion);
		//config_set_value(config, "ID_SUSCRIPCION_GET", id);
		config_save(config);
		free(id);
		sem_post(&suscripciones);

		administradorMensajesColas(SUSCRIPTOR_GETPOKEMON, conexion, IDsuscripcion);
	} else {
		sem_post(&reintentoGet);
		sem_post(&suscripciones);
	}
}

void* suscribirseAColaCatch(){
	int IDsuscripcion;
	sem_wait(&suscripciones);
	int conexion = crear_conexion(ip, puerto);
	if(conexion != -1){

		enviarSuscripcion(IDsuscripcionCatch, conexion, SUSCRIPTOR_CATCHPOKEMON);
		recv(conexion, &IDsuscripcion, sizeof(int), MSG_WAITALL);
		char* id = string_itoa(IDsuscripcion);
		//config_set_value(config, "ID_SUSCRIPCION_CATCH", id);
		config_save(config);
		free(id);
		sem_post(&suscripciones);

		administradorMensajesColas(SUSCRIPTOR_CATCHPOKEMON, conexion, IDsuscripcion);
	} else {
		sem_post(&reintentoCatch);
		sem_post(&suscripciones);
	}
}

// ------------------------------------ RECEPCION BROKER ------------------------------------ //

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
				NewPokemonConIDs* nuevoNewPokemonConId;
				for(int i = 0; i<cantidadNewPokemon; i++){
					recv(conexion, &codigo, sizeof(op_code), MSG_WAITALL);
					nuevoNewPokemonConId = recibir_NEW_POKEMON(conexion, 0, 1);
					int ack=1;
					send(conexion, &ack, sizeof(int), 0);
					pthread_t admin;
					pthread_create(&admin, NULL, adminMensajeNewPokemon, nuevoNewPokemonConId);
					pthread_detach(admin);
				}
				recibirMensajesNew(conexion);
				break;

			case SUSCRIPTOR_GETPOKEMON:
				IDsuscripcionGet= IDsuscripcion;

				recv(conexion, &cantidadGetPokemon, sizeof(int), MSG_WAITALL);
				GetPokemonConIDs* nuevoGetPokemonConId;
				for(int i = 0; i<cantidadGetPokemon; i++){
					recv(conexion, &codigo1, sizeof(op_code), MSG_WAITALL);
					nuevoGetPokemonConId = recibir_GET_POKEMON(conexion, 0, 1);
					int ack=1;
					send(conexion, &ack, sizeof(int), 0);
					pthread_t admin;
					pthread_create(&admin, NULL, adminMensajeGetPokemon, nuevoGetPokemonConId);
					pthread_detach(admin);
				}
				recibirMensajesGet(conexion);
				break;

			case SUSCRIPTOR_CATCHPOKEMON:
				IDsuscripcionCatch= IDsuscripcion;

				recv(conexion, &cantidadCatchPokemon, sizeof(int), MSG_WAITALL);
				CatchPokemonConIDs* nuevoCatchPokemonConId;
				for(int i = 0; i<cantidadCatchPokemon; i++){
					recv(conexion, &codigo2, sizeof(op_code), MSG_WAITALL);
					nuevoCatchPokemonConId = recibir_CATCH_POKEMON(conexion, 0, 1);
					int ack=1;
					send(conexion, &ack, sizeof(int), 0);
					pthread_t admin;
					pthread_create(&admin, NULL, adminMensajeCatch, nuevoCatchPokemonConId);
					pthread_detach(admin);
				}
				recibirMensajesCatch(conexion);
				break;
			default:
				break;
	}
}

void* recibirMensajesNew(int conexionNewPokemon){
	pthread_t admin;
	void* mensajeRecibido;
	NewPokemonConIDs* nuevoNewPokemon;
	while(1){
		int cod_op;
		int error = recv(conexionNewPokemon, &cod_op, sizeof(int), 0);
		if(error == 0){
			close(conexionNewPokemon);
			sem_post(&reintentoNew);
			pthread_exit(NULL);
		}
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
		int error = recv(conexionGetPokemon, &cod_op, sizeof(int), 0);
		if(error == 0){
			close(conexionGetPokemon);
			sem_post(&reintentoGet);
			pthread_exit(NULL);
		}
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
		int error = recv(conexionCatch, &cod_op, sizeof(int), 0);
		if(error == 0){
			close(conexionCatch);
			sem_post(&reintentoCatch);
			pthread_exit(NULL);
		}
		nuevoCatch = recibir_CATCH_POKEMON(conexionCatch, 0, 1);
		int ack = 1;
		send(conexionCatch, &ack, sizeof(int), 0);
		pthread_create(&admin, NULL, adminMensajeCatch, nuevoCatch);
		pthread_detach(admin);
	}
}

// ------------------------------------ ADMIN MENSAJES ------------------------------------ //

void* adminMensajeNewPokemon(NewPokemonConIDs* mensajeLeido){
	agregarPokemon(mensajeLeido);
	free(mensajeLeido->newPokemon->nombre);
	free(mensajeLeido->newPokemon);
	free(mensajeLeido);
}

void* adminMensajeGetPokemon(GetPokemonConIDs* mensajeLeido){
	obtenerCantidadYPosiciones(mensajeLeido);
	free(mensajeLeido->getPokemon->nombre);
	free(mensajeLeido->getPokemon);
	free(mensajeLeido);
}

void* adminMensajeCatch(CatchPokemonConIDs* mensajeLeido){
	eliminarPokemon(mensajeLeido);
	free(mensajeLeido->catchPokemon->nombre);
	free(mensajeLeido->catchPokemon);
	free(mensajeLeido);
}

// --------------------- LEVANTAR FILE SYSTEM --------------------- //

void crearDirectorioTG(){
	char* path = string_new();
	string_append(&path,puntoMontaje);
	string_append(&path, "/TALL_GRASS");

	if(!mkdir(path, 0777)){
		log_info(logger, "Genere el directorio para el FL en %s", puntoMontaje);
		crearMetadata(path);
		crearFiles(path);
		crearBlocks(path);
	} else {
	free(path);
	}
}


void crearMetadata(char* pathOrigin){
	FILE* metadata;
	char* path = string_duplicate(pathOrigin);

	string_append(&path,"/Metadata");
	char* pathMetadata = string_duplicate(path);
	mkdir(path, 0777);

	log_info(logger, "Agrego la carpeta Metadata");

	string_append(&path, "/Metadata.bin");
	metadata = fopen(path, "wrb");
	fclose(metadata);
	t_config* md = config_create(path);
	char* tamanio = string_itoa(tamanioBloque);
	char* cantidad = string_itoa(cantidadBloques);
	config_set_value(md, "BLOCK_SIZE", tamanio);
	config_set_value(md, "BLOCKS", cantidad);
	config_set_value(md, "MAGIC_NUMBER", magicNumber);
	config_save(md);
	free(tamanio);
	free(cantidad);

	log_info(logger, "Agrego el Metadata del File System");

	generarBitmap(pathMetadata, md);
	config_destroy(md);
	free(pathMetadata);
	free(path);
}


void crearFiles(char* pathOrigin){
	FILE* metadata;
	char* path = string_duplicate(pathOrigin);

	log_info(logger, "Agrego la carpeta Files");

	string_append(&path, "/Files");
	mkdir(path, 0777);
	string_append(&path, "/Metadata.bin");
	metadata = fopen(path, "wrb");
	fclose(metadata);

	t_config* md = config_create(path);
	config_set_value(md, "DIRECTORY", "Y");
	config_save(md);

	log_info(logger, "Agrego el Metadata de Files");

	config_destroy(md);
	free(path);
}


void crearBlocks(char* path){
	string_append(&path, "/Blocks");
	mkdir(path, 0777);
	log_info(logger, "Agrego la carpeta Blocks");
	free(path);
}


void generarBitmap(char* pathOrigin, t_config* md){

	char* path = string_duplicate(pathOrigin);
	string_append(&path, "/Bitmap.bin");

	void* punteroABitmap = malloc(cantidadBloques/8);

	t_bitarray* bitmap = bitarray_create_with_mode(punteroABitmap, cantidadBloques/8, MSB_FIRST);
	for(int i = 0; i < cantidadBloques; i++){
		bitarray_clean_bit(bitmap, i);
	}
	FILE* bitmapFile = fopen(path, "wrb");
	fwrite(punteroABitmap, cantidadBloques/8, 1, bitmapFile);
	fclose(bitmapFile);
	bitarray_destroy(bitmap);
	free(punteroABitmap);
	free(path);

	log_info(logger, "Genere el bitmap");
}

// --------------------- MANEJO FILE SYSTEM --------------------- //

void armarFolderPara(char* nombre){
	char* path = string_new();
	string_append(&path,puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, nombre);
	mkdir(path, 0777);
	free(path);
	log_info(logger, "Genere la carpeta para %s", nombre);
}

void crearMetadataPara(char* nombre){
	FILE* metadata;
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, nombre);
	string_append(&path, "/Metadata.bin");

	armarFolderPara(nombre);
	metadata = fopen(path, "wrb");
	fclose(metadata);
	//fflush(metadata);

	t_config* md = config_create(path);
	config_set_value(md, "DIRECTORY", "N");
	config_set_value(md, "SIZE", "0");
	config_set_value(md, "BLOCKS", "[]");
	config_set_value(md, "OPEN", "N");
	config_save(md);
	config_destroy(md);
	free(path);
	log_info(logger, "Cree el Metadata para %s", nombre);
}

int archivoAbierto(char* path){
	t_config* md = config_create(path);
	int abierto = !strcmp(config_get_string_value(md, "OPEN"), "Y");
	config_destroy(md);
	return abierto;
}

// --------------------- INTERACCION CON ARCHIVOS --------------------- //

int existePokemon(char* nombreOriginal){
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, nombreOriginal);
	string_append(&path, "/Metadata.bin");
	FILE* fd = fopen(path, "r");

	if(fd != NULL){
		fclose(fd);
		free(path);
		return 1;
	} else {
		free(path);
		return 0;
	}
}

void actualizarTamanioPokemon(char* pokemon){
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, pokemon);
	string_append(&path, "/Metadata.bin");

	int tamanio = 0;
	int i = 0;

	FILE *fd = fopen(path, "r");

	if(fd != NULL){

		fclose(fd);

		t_config* conf = config_create(path);
		char** bloques = config_get_array_value(conf, "BLOCKS");

		while(bloques[i]!=NULL){
			tamanio = tamanio + tamanioDeBloque(bloques[i]);
			i++;
		}

		char* tamanioTotal = string_itoa(tamanio);
		config_set_value(conf, "SIZE", tamanioTotal);
		free(tamanioTotal);
		config_save(conf);
		config_destroy(conf);
		free(path);
		liberar_lista(bloques);
		log_info(logger, "Actualiza SIZE de %s", pokemon);
	}
}

// --------------------- AGREGAR - NEW --------------------- //

void agregarPokemon(NewPokemonConIDs* newPokemon){
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, newPokemon->newPokemon->nombre);
	string_append(&path, "/Metadata.bin");
	if(!existePokemon(newPokemon->newPokemon->nombre)){
		char* escritura = string_new();
		char* coorX = string_itoa(newPokemon->newPokemon->coordenadas.posicionX);
		char* coorY = string_itoa(newPokemon->newPokemon->coordenadas.posicionY);
		string_append(&escritura, coorX);
		string_append(&escritura, "-");
		string_append(&escritura, coorY);
		string_append(&escritura, "=");
		char* cantidad = string_itoa(newPokemon->newPokemon->cantidad);
		string_append(&escritura, cantidad);
		string_append(&escritura, "\n");
		crearMetadataPara(newPokemon->newPokemon->nombre);
		agregarBloqueAMetadata(escritura, newPokemon->newPokemon->nombre);
		free(coorX);
		free(coorY);
		free(cantidad);
	} else {
		t_config* md = config_create(path);
		while(archivoAbierto(path)){
			sleep(tiempoReintento);
		}
		config_set_value(md,"OPEN","Y");
		config_save(md);
		char** bloques = config_get_array_value(md, "BLOCKS");
		config_destroy(md);

		agregarPokemonAUnBloque(bloques, newPokemon);

		md = config_create(path);
		config_set_value(md,"OPEN","N");
		config_save(md);
		config_destroy(md);
		liberar_lista(bloques);
	}
	actualizarTamanioPokemon(newPokemon->newPokemon->nombre);
	sleep(tiempoRetardo);
	enviarMensajeAppeared(newPokemon->IDmensaje, newPokemon->newPokemon->nombre, newPokemon->newPokemon->coordenadas);
	printf("Envie un appeared\n\n");
	free(path);
}

void agregarPokemonAUnBloque(char** bloques, NewPokemonConIDs* newPokemon){
	int i = 0;
	int carga = 0;
	char** subLeido;
	char* escritura;

	int tamanioMax = cantidadDeBloques(bloques);
	tamanioMax = tamanioBloque * tamanioMax;


	char** leido = lecturaBloques(bloques);

	char* coorX = string_itoa(newPokemon->newPokemon->coordenadas.posicionX);
	char* coorY = string_itoa(newPokemon->newPokemon->coordenadas.posicionY);

	i = 0;

	while(leido[i]!=NULL && !carga){
		escritura = string_new();
		subLeido = string_split(leido[i], "=");
		if(string_starts_with(subLeido[0], coorX) && string_ends_with(subLeido[0], coorY)){
			carga = 1;
			string_append(&escritura, subLeido[0]);
			string_append(&escritura, "=");

			char* cantidadAnterior = string_duplicate(subLeido[1]);
			int cantidadAnt = atoi(cantidadAnterior);
			int nuevaCantidad = cantidadAnt + newPokemon->newPokemon->cantidad;
			char* nuevaCant = string_itoa(nuevaCantidad);
			string_append(&escritura, nuevaCant);

			free(nuevaCant);
			free(cantidadAnterior);
			free(leido[i]); //---------------------------- A ver si salvo esos bytes !!
			leido[i] = string_duplicate(escritura);
		}
		i++;
		free(escritura);
		liberar_lista(subLeido);
	}

	i = 0;
	if(carga){
		escribirBloques(bloques, leido, tamanioMax, newPokemon->newPokemon->nombre);
	} else {
		escritura = string_new();
		string_append(&escritura, coorX);
		string_append(&escritura, "-");
		string_append(&escritura, coorY);
		string_append(&escritura, "=");
		char* cantidad = string_itoa(newPokemon->newPokemon->cantidad);
		string_append(&escritura, cantidad);
		string_append(&escritura, "\n");
		free(cantidad);
		while(bloques[i]!=NULL && !carga){
			if(!esBloqueLleno(bloques[i], escritura)){
				carga = 1;
				generarEscrituraEnBloque(escritura, bloques[i]);
			}
			i++;
		}
		if(!carga){
			agregarBloqueAMetadata(escritura, newPokemon->newPokemon->nombre);
		}
	}
	free(coorX);
	free(coorY);
	liberar_lista(leido);
}

void agregarBloqueAMetadata(char* escritura, char* nombre){
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, nombre);
	string_append(&path, "/Metadata.bin");

	char* nuevoBloques = string_new();
	t_config* md = config_create(path);
	char** bloques = config_get_array_value(md, "BLOCKS");
	int bloqueNum = obtenerYEscribirProximoDisponible();
	char* bloque = string_itoa(bloqueNum);
	int i = 0;
	string_append(&nuevoBloques, "[");
	while(bloques[i] != NULL){
		string_append(&nuevoBloques, bloques[i]);
		string_append(&nuevoBloques, ",");
		i++;
	}
	string_append(&nuevoBloques, bloque);
	string_append(&nuevoBloques, "]");
	config_set_value(md, "BLOCKS", nuevoBloques);
	config_save(md);

	generarBloqueNuevo(escritura, bloque);
	config_destroy(md);
	free(path);
	free(nuevoBloques);
	free(bloque);
	liberar_lista(bloques);
	log_info(logger, "Agregue un bloque al Metadata de %s", nombre);
}

void generarBloqueNuevo(char* escritura, char* bloque){
	char* nombre = string_new();
	string_append(&nombre, puntoMontaje);
	string_append(&nombre, "/TALL_GRASS/Blocks/");
	string_append(&nombre, bloque);
	string_append(&nombre, ".bin");

	FILE* fd = fopen(nombre, "wt");
	fwrite(escritura, strlen(escritura), 1, fd);
	fclose(fd);
	log_info(logger, "Genere un bloque nuevo para %s", nombre);
	free(nombre);
	free(escritura);
}

void generarEscrituraEnBloque(char* escritura, char* bloque){
	char* nombre = string_new();
	string_append(&nombre, puntoMontaje);
	string_append(&nombre, "/TALL_GRASS/Blocks/");
	string_append(&nombre, bloque);
	string_append(&nombre, ".bin");

	int file = open(nombre, O_CREAT | O_RDWR, 0664);
	lseek(file, 0, SEEK_END);
	write(file, escritura, strlen(escritura));
	close(file);

	log_info(logger, "Escribi en el bloque %s la linea %s", bloque, escritura);

	free(nombre);
}

void pisarBloque(char* escritura, char* bloque){
	char* nombre = string_new();
	string_append(&nombre, puntoMontaje);
	string_append(&nombre, "/TALL_GRASS/Blocks/");
	string_append(&nombre, bloque);
	string_append(&nombre, ".bin");

	FILE* fd = fopen(nombre, "wt");
	fwrite(escritura, strlen(escritura), 1, fd);
	fclose(fd);

	log_info(logger, "Pise el bloque %s", bloque);

	free(nombre);
	free(escritura);
}

char** lecturaBloques(char** bloques){
	char* aux;

	int i = cantidadDeBloques(bloques);

	void* lectura = malloc(tamanioBloque*i);

	char* original = lectura;
	i=0;

	int tamanioDeLoLeido = 0;

	while(bloques[i]!=NULL){
		aux = lecturaBloque(bloques[i]);
		memcpy(lectura, aux, strlen(aux));
		tamanioDeLoLeido += strlen(aux);
		i++;
		lectura = lectura + strlen(aux);
		free(aux);
	}

	original = string_substring_until(original, tamanioDeLoLeido);

	char** leido = string_split(original, "\n");

	i=0;

	while(leido[i]!=NULL){
		if(string_starts_with(leido[i], "\025")){
			leido[i] = string_substring_from(leido[i], strlen("\025"));
		}
		i++;
	}

	free(original);
	//free(lectura);

	return leido;
}

char* lecturaBloque(char* bloque){
	char* nombre = string_new();
	string_append(&nombre, puntoMontaje);
	string_append(&nombre, "/TALL_GRASS/Blocks/");
	string_append(&nombre, bloque);
	string_append(&nombre, ".bin");
	char* centinela = string_new();
	int file = open(nombre, O_CREAT | O_RDWR, 0664);

	void* punteroATexto = malloc(tamanioBloque);

	int tamanioLeido = read(file, punteroATexto, tamanioBloque);

	close(file);

	centinela = punteroATexto;

	centinela = string_substring_until(centinela, tamanioLeido);

	free(nombre);

	free(punteroATexto);

	return centinela;
}

int esBloqueLleno(char* numBloque, char* escritura){
	int lleno = tamanioDeBloque(numBloque) +strlen(escritura) >= tamanioBloque;
	return lleno;
}

void escribirBloques(char** bloques,char** leido,int tamanioMax, char* nombre){
	int i = 0;
	int j = 0;
	int tamanioEscritura = 0;
	int tamanioTotal = 0;
	char* escritura;
	while(bloques[i]!=NULL && tamanioEscritura < tamanioMax){
		escritura = string_new();
		if(leido[j]!=NULL && leido[j+1] == NULL){
			if(!strcmp(leido[j],"")){
				string_append(&escritura, "");
				j++;
			} else {
				string_append(&escritura, leido[j]);
				string_append(&escritura, "\n");
				j++;
			}
		} else {
			if(leido[j]!= NULL){
				if(!strcmp(leido[j],"")){ j++;}
				tamanioEscritura += strlen(leido[j])+1;
				tamanioTotal += strlen(leido[j])+1;
			}
		while(leido[j] != NULL && tamanioEscritura < tamanioBloque && tamanioTotal < tamanioMax){
			string_append(&escritura, leido[j]);
			string_append(&escritura, "\n");
			j++;
			if(leido[j]!= NULL){
				if(!strcmp(leido[j],"")){ j++;}
				tamanioEscritura += strlen(leido[j])+1;
				tamanioTotal += strlen(leido[j])+1;
			}
		}
	}
		pisarBloque(escritura, bloques[i]);
		tamanioEscritura = 0;
		i++;
	}
	while(leido[j] != NULL){
			escritura = string_new();
			string_append(&escritura, leido[j]);
			string_append(&escritura, "\n");
			agregarBloqueAMetadata(escritura, nombre);
			j++;
	}
}

// --------------------- ELIMINAR - CATCH --------------------- //

void eliminarPokemon(CatchPokemonConIDs* pokemon){
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, pokemon->catchPokemon->nombre);

	char* pathCarpeta= string_duplicate(path);

	string_append(&path, "/Metadata.bin");
	int encontrado = 0;

	if(!existePokemon(pokemon->catchPokemon->nombre)){
		log_info(logger,"No existe el pokemon %s", pokemon->catchPokemon->nombre);
	} else {
		t_config* md = config_create(path);
		while(archivoAbierto(path)){
			sleep(tiempoReintento);
		}
		config_set_value(md,"OPEN","Y");
		config_save(md);
		char** bloques = config_get_array_value(md, "BLOCKS");
		config_destroy(md);

		int i = 0;
		char* centinela = string_new();
		char* coorX = string_itoa(pokemon->catchPokemon->coordenadas.posicionX);
		char* coorY = string_itoa(pokemon->catchPokemon->coordenadas.posicionY);
		string_append(&centinela,coorX);
		string_append(&centinela,"-");
		string_append(&centinela,coorY);

		char** leido = lecturaBloques(bloques);
		char** subleido;
		int lineaVacia = 0;

		while(leido[i] !=NULL&& !encontrado){
			subleido = string_split(leido[i], "=");
			if(!strcmp(subleido[0], centinela)){
				encontrado = 1;
				int nuevaCantidad = atof(subleido[1]) - 1;
				char* escritura = string_new();
				if(nuevaCantidad == 0){
					lineaVacia = 1;
					log_info(logger,"Ya no hay mas %s en la posicion %d %d", pokemon->catchPokemon->nombre, pokemon->catchPokemon->coordenadas.posicionX, pokemon->catchPokemon->coordenadas.posicionY);
					string_append(&escritura, "");
					free(leido[i]);
					leido[i] = string_duplicate(escritura);
					free(escritura);
				} else {
					string_append(&escritura, subleido[0]);
					string_append(&escritura, "=");
					char* nuevaCant = string_itoa(nuevaCantidad);
					string_append(&escritura, nuevaCant);
					free(leido[i]);
					leido[i] = string_duplicate(escritura);
					free(escritura);
					free(nuevaCant);
				}
			}
			liberar_lista(subleido);
			i++;
		}
		if(!encontrado){
			log_info(logger,"No existe %s en esa posición", pokemon->catchPokemon->nombre);
			md = config_create(path);
			config_set_value(md,"OPEN","N");
			config_save(md);
			config_destroy(md);
		} else {
			int cantDeBloques = cantidadDeBloques(bloques);
			escribirBloques(bloques, leido, cantDeBloques*tamanioBloque, pokemon->catchPokemon->nombre);
			if(lineaVacia){
				md = config_create(path);
				config_set_value(md,"OPEN","N");
				config_save(md);
				config_destroy(md);
				actualizarTamanioPokemon(pokemon->catchPokemon->nombre);
				if(recorrerParaEliminarArchivo(bloques, path)){
					eliminarSiEsCarpetaVacia(path, pathCarpeta);
				}
			} else {
				md = config_create(path);
				config_set_value(md,"OPEN","N");
				config_save(md);
				config_destroy(md);
				actualizarTamanioPokemon(pokemon->catchPokemon->nombre);
			}
		}
		liberar_lista(bloques);
		liberar_lista(leido);
		free(centinela);
		free(coorX);
		free(coorY);

	}
	sleep(tiempoRetardo);
	enviarMensajeCaught(pokemon->IDmensaje, encontrado);
	printf("Envie un Caught\n\n");
	free(path);
	free(pathCarpeta);
}

int recorrerParaEliminarArchivo(char** bloques, char* path){
	int i = 0;
	int seBorra = 0;
	while(bloques[i] != NULL && !seBorra){
		if(eliminarSiEsArchivoVacio(bloques[i])){
			seBorra=1;
			eliminarBloqueDeMetadata(path, bloques[i]);
		}
	i++;
	}
	return seBorra;
}

int eliminarSiEsArchivoVacio(char* bloque){
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Blocks/");
	string_append(&path, bloque);
	string_append(&path, ".bin");

	if(tamanioDeBloque(bloque) == 0){
		remove(path);
		log_info(logger, "Elimine el archivo %s", path);
		free(path);
		return 1;
	}
	free(path);
	return 0;
}

void eliminarBloqueDeMetadata(char* path, char* bloqueAEliminar){

	char* nuevoBloques = string_new();
	t_config* md = config_create(path);
	char** bloques = config_get_array_value(md, "BLOCKS");
	int i = 0;
	string_append(&nuevoBloques, "[");
	while(bloques[i+1] != NULL){
		if(strcmp(bloques[i], bloqueAEliminar)){
			string_append(&nuevoBloques, bloques[i]);
			string_append(&nuevoBloques, ",");
		}
		i++;
	}
	if(strcmp(bloques[i],bloqueAEliminar)){
		string_append(&nuevoBloques, bloques[i]);
	} else {
		int length = string_length(nuevoBloques);
		if(length != 1){
			nuevoBloques = string_substring(nuevoBloques, 0, length-1);
		}
	}
	string_append(&nuevoBloques, "]");
	config_set_value(md, "BLOCKS", nuevoBloques);
	config_save(md);
	config_destroy(md);
	liberar_lista(bloques);
	free(nuevoBloques);
	log_info(logger, "Elimine el bloque %s", bloqueAEliminar);
	eliminarBit(atoi(bloqueAEliminar));
}

void eliminarSiEsCarpetaVacia(char* path, char* pathCarpeta){
	t_config* md = config_create(path);
	char* bloques = config_get_string_value(md, "BLOCKS");
	if(!strcmp(bloques, "[]")){
		config_destroy(md);
		remove(path);
		log_info(logger, "Elimine el archivo %s", path);
		remove(pathCarpeta);
		log_info(logger, "Elimine la carpeta %s", pathCarpeta);
	}
}


//--------------------- BUSCAR - GET --------------------- //

void obtenerCantidadYPosiciones(GetPokemonConIDs* pokemon){

	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, pokemon->getPokemon->nombre);
	string_append(&path, "/Metadata.bin");
	char** bloques;

	char* error = string_new();
	if(!existePokemon(pokemon->getPokemon->nombre)){
		log_info(logger,"No existe el pokemon %s",pokemon->getPokemon->nombre);
		t_list* paresOrdenados = list_create();

		sleep(tiempoRetardo);
		enviarMensajeLocalized(pokemon->IDmensaje, pokemon->getPokemon->nombre, paresOrdenados);
		printf("\nEnvie un localized\n\n");

		list_destroy(paresOrdenados);
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
			config_destroy(md);

			int i = 0;
			t_list* coordenadas = list_create();
			t_list* posiciones;
			while(bloques[i]!=NULL){
				posiciones = obtenerPosiciones(bloques[i]);
				list_add_all(coordenadas ,posiciones);
				i++;
				list_destroy(posiciones);
			}

			enviarMensajeLocalized(pokemon->IDmensaje, pokemon->getPokemon->nombre, coordenadas);
			printf("\nEnvie un localized\n\n");
			list_destroy(coordenadas);
			liberar_lista(bloques);
	}
	free(path);
	free(error);
}

t_list* obtenerPosiciones(char* bloque){
	t_list* posiciones = list_create();
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Blocks/");
	string_append(&path, bloque);
	string_append(&path, ".bin");

	char* leido = lecturaBloque(bloque);
	char** pokemones;
	char** subleido;
	char** posicion;
	CoordenadasXY* coordenadas;
	pokemones = string_split(leido, "\n");
	int i = 0;
	while(pokemones[i] != NULL){
		coordenadas = malloc(sizeof(CoordenadasXY));
		subleido = string_split(pokemones[i], "=");
		posicion = string_split(subleido[0], "-");
		coordenadas->posicionX = atoi(posicion[0]);
		coordenadas->posicionY = atoi(posicion[1]);
		for(int j=0; j < atoi(subleido[1]); j++){
			list_add(posiciones, coordenadas);
		}
		liberar_lista(subleido);
		liberar_lista(posicion);
		i++;
	}
	free(path);
	free(leido);
	liberar_lista(pokemones);
	return posiciones;
}


// --------------------- ENVIAR MENSAJES --------------------- //

void enviarMensajeAppeared(int IDmensaje, char* pokemon, CoordenadasXY coordenadas){
	sem_wait(&mensajeABroker);
	int socket_suscriptor = crear_conexion(ip, puerto);
	if(socket_suscriptor!=-1){
		AppearedPokemon* nuevo = malloc(sizeof(AppearedPokemon));
		nuevo->coordenadas = coordenadas;
		nuevo->nombre = pokemon;
		nuevo->tamanioNombrePokemon = strlen(pokemon) + 1;
		printf("Nombre: %s\n", nuevo->nombre);
		printf("Tamanio Nombre: %d\n", nuevo->tamanioNombrePokemon);
		printf("Coordenada X: %d\n", nuevo->coordenadas.posicionX);
		printf("Coordenada Y: %d\n", nuevo->coordenadas.posicionY);
		printf("ID Mensaje: %d\n", IDmensaje);
		enviarAppearedPokemon(nuevo, socket_suscriptor, 0, IDmensaje);
		liberar_conexion(socket_suscriptor);
		free(nuevo);
	}
	sem_post(&mensajeABroker);
}

void enviarMensajeCaught(int IDmensaje, int resultado){
	sem_wait(&mensajeABroker);
	int socket_suscriptor = crear_conexion(ip, puerto);
	if(socket_suscriptor!=-1){
		CaughtPokemon* nuevo = malloc(sizeof(CaughtPokemon));
		nuevo->atrapar = resultado;
		enviarCaughtPokemon(nuevo, socket_suscriptor, 0, IDmensaje);
		liberar_conexion(socket_suscriptor);
		free(nuevo);
	}
	sem_post(&mensajeABroker);
}

void enviarMensajeLocalized(int IDmensaje, char* pokemon, t_list* coordenadas){
	sem_wait(&mensajeABroker);
	int socket_suscriptor = crear_conexion(ip, puerto);
	if(socket_suscriptor!=-1){
		LocalizedPokemon* nuevo = malloc(sizeof(LocalizedPokemon));
		nuevo->cantidadParesOrdenados = list_size(coordenadas);
		nuevo->tamanioNombrePokemon = strlen(pokemon)+1;
		nuevo->nombre = pokemon;
		nuevo->paresOrdenados = coordenadas;
		enviarLocalizedPokemon(nuevo, socket_suscriptor, 0, IDmensaje);
		liberar_conexion(socket_suscriptor);
		free(nuevo);
	}
	sem_post(&mensajeABroker);
}

// --------------------- BITMAP --------------------- //


int obtenerYEscribirProximoDisponible(){
	char* path = string_new();

	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Metadata/Bitmap.bin");

	sem_wait(&bitmapSem);
	int bitmapFile = open(path, O_CREAT | O_RDWR, 0664);

	void* punteroABitmap = mmap(NULL, cantidadBloques/8, PROT_READ | PROT_WRITE, MAP_SHARED, bitmapFile, 0);

	t_bitarray* bitmap = bitarray_create_with_mode((char*)punteroABitmap, cantidadBloques/8, MSB_FIRST);

	for(int i=0; i<cantidadBloques; i++){
		if(bitarray_test_bit(bitmap, i) == 0){
			bitarray_set_bit(bitmap ,i);
			printf("Posicion %d\n", i);
			msync(punteroABitmap ,cantidadBloques/8 ,0);
			//write(bitmapFile, punteroABitmap, cantidadBloques/8);
			close(bitmapFile);
			sem_post(&bitmapSem);
			free(path);
			bitarray_destroy(bitmap);
			return i;
		}
	}
	close(bitmapFile);
	sem_post(&bitmapSem);
	bitarray_destroy(bitmap);
	free(path);
	printf("Espacio lleno\n");
	return 0;
}

void eliminarBit(int index){
	char* path = string_new();
	char* cantBloques;

	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Metadata");

	cantBloques = string_duplicate(path);
	string_append(&cantBloques, "/Metadata.bin");
	t_config* md = config_create(cantBloques);

	int cantidadDeBloques = config_get_int_value(md, "BLOCKS");

	sem_wait(&bitmapSem);
	string_append(&path, "/Bitmap.bin");
	int bitmapFile = open(path, O_CREAT | O_RDWR, 0664);

	void* punteroABitmap = mmap(NULL, cantidadDeBloques/8, PROT_READ | PROT_WRITE, MAP_SHARED, bitmapFile, 0);

	t_bitarray* bitmap = bitarray_create_with_mode((char*)punteroABitmap, cantidadDeBloques/8, MSB_FIRST);

	bitarray_clean_bit(bitmap, index);

	msync(punteroABitmap ,cantidadDeBloques/8 ,0);

	close(bitmapFile);
	sem_post(&bitmapSem);
	bitarray_destroy(bitmap);

	config_destroy(md);
	free(cantBloques);
	free(path);
}

// --------------------- BLOCKS --------------------- //

int cantidadDeBloques(char** bloques){
	int i = 0;
	while(bloques[i]!=NULL){
		i++;
	}
	return i;
}

int tamanioDeBloque(char* bloque){
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Blocks/");
	string_append(&path, bloque);
	string_append(&path, ".bin");

	FILE* fd = fopen(path, "rb");
	fseek(fd, 0, SEEK_END);
	int tamanio = ftell(fd);

	fclose(fd);
	free(path);
	return tamanio;
}

// --------------------- AUX --------------------- //

void liberar_lista(char** lista){
	int contador = 0;
	while(lista[contador] != NULL){
			free(lista[contador]);
			contador++;
	}
	free(lista);
}

