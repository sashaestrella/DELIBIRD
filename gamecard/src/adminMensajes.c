#include "adminMensajes.h"

// ------------------------------------ GAMEBOY ------------------------------------ //

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
	//char* loQueVoyALoguear = "Se creó una conexión. El código de operación recibido es: %d.";
	//log_info(logger, loQueVoyALoguear,cod_op);
	process_request_gamecard(cod_op, *socket);
}

void process_request_gamecard(int cod_op, int cliente_fd) {
	int size;
	void* algoARecibir;

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
					//atenderMensajesNew(newConIDs);
					agregarPokemon(newConIDs);
					free(newConIDs);
					break;
				case GET_POKEMON:
					getConIDs = recibir_GET_POKEMON(cliente_fd,&size,1);
					//atenderMensajesGet(getConIDs);
					obtenerCantidadYPosiciones(getConIDs);
					free(getConIDs);
					break;
				case CATCH_POKEMON:
					catchConIDs = recibir_CATCH_POKEMON(cliente_fd,&size,1);
					//atenderMensajesCatch(catchConIDs);
					eliminarPokemon(catchConIDs);
					free(catchConIDs);
					break;
	}
}


// ------------------------------------ BROKER ------------------------------------ //

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

// ------------------------------------ SUSCRIPCIONES ------------------------------------ //


void* suscribirseAColaNew(){
	int IDsuscripcion;
	sem_wait(&suscripciones);
	int conexion = crear_conexion(ip, puerto);
	if(conexion!=-1){

		enviarSuscripcion(IDsuscripcionNew, conexion, SUSCRIPTOR_NEWPOKEMON);
		recv(conexion, &IDsuscripcion, sizeof(int), MSG_WAITALL);
		char* id = string_itoa(IDsuscripcion);
		config_set_value(config, "ID_SUSCRIPCION_NEW", id);
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
		config_set_value(config, "ID_SUSCRIPCION_GET", id);
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
		config_set_value(config, "ID_SUSCRIPCION_CATCH", id);
		config_save(config);
		free(id);
		sem_post(&suscripciones);

		administradorMensajesColas(SUSCRIPTOR_CATCHPOKEMON, conexion, IDsuscripcion);
	} else {
		sem_post(&reintentoCatch);
		sem_post(&suscripciones);
	}
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
				NewPokemonConIDs* nuevoNewPokemonConId;
				for(int i = 0; i<cantidadNewPokemon; i++){
					recv(conexion, &codigo, sizeof(op_code), MSG_WAITALL);
					nuevoNewPokemonConId = recibir_NEW_POKEMON(conexion, 0, 1);
					int ack=1;
					send(conexion, &ack, sizeof(int), 0);
					adminMensajeNewPokemon(nuevoNewPokemonConId);
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
					adminMensajeGetPokemon(nuevoGetPokemonConId);
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
					adminMensajeCatch(nuevoCatchPokemonConId);
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

void* adminMensajeNewPokemon(NewPokemonConIDs* nuevoNewPokemon){
	list_add(mensajesNew, nuevoNewPokemon);
	sem_post(&colaNew);
}

void* adminMensajeGetPokemon(GetPokemonConIDs* nuevoGetPokemon){
	list_add(mensajesGet, nuevoGetPokemon);
	sem_post(&colaGet);

}

void* adminMensajeCatch(CatchPokemonConIDs* nuevoCatch){
	list_add(mensajesCatch, nuevoCatch);
	sem_post(&colaCatch);
}

// --------------------- ATENDER MENSAJES --------------------- //

void* atenderMensajesNew(){
	while(1){

		sem_wait(&colaNew);
		NewPokemonConIDs* mensajeLeido;
		mensajeLeido = (NewPokemonConIDs*)list_remove(mensajesNew ,0);
		agregarPokemon(mensajeLeido);
		free(mensajeLeido->newPokemon->nombre);
		free(mensajeLeido->newPokemon);
		free(mensajeLeido);

	}
}


void* atenderMensajesCatch(){
	while(1){

		sem_wait(&colaCatch);
		CatchPokemonConIDs* mensajeLeido;
		mensajeLeido = (CatchPokemonConIDs*)list_remove(mensajesCatch ,0);
		eliminarPokemon(mensajeLeido);
		free(mensajeLeido->catchPokemon->nombre);
		free(mensajeLeido->catchPokemon);
		free(mensajeLeido);

	}
}


void* atenderMensajesGet(){
	while(1){

		sem_wait(&colaGet);
		GetPokemonConIDs* mensajeLeido;
		mensajeLeido = (GetPokemonConIDs*)list_remove(mensajesGet ,0);
		obtenerCantidadYPosiciones(mensajeLeido);
		free(mensajeLeido->getPokemon->nombre);
		free(mensajeLeido->getPokemon);
		free(mensajeLeido);

	}
}

// --------------------- FILE SYSTEM --------------------- //


void* crearDirectorioTG(){
	char* path = string_new();
	string_append(&path,puntoMontaje);
	string_append(&path, "/TALL_GRASS");
	mkdir(path, 0777);

	log_info(logger, "Genere el directorio para el FL en %s", puntoMontaje);

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
	free(tamanio);
	free(cantidad);

	log_info(logger, "Agrego el Metadata del File System");

	generarBitmap(pathMetadata, md);
	config_destroy(md);
	free(pathMetadata);
	free(path);
}


void* crearFiles(char* pathOrigin){
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


void* crearBlocks(char* path){
	string_append(&path, "/Blocks");
	mkdir(path, 0777);
	free(path);
	log_info(logger, "Agrego la carpeta Blocks");
}


void* generarBitmap(char* pathOrigin, t_config* md){

	char* path = string_duplicate(pathOrigin);
	string_append(&path, "/Bitmap.bin");

	FILE* bitmapFile = fopen(path, "r");

	if(bitmapFile == NULL){
		void* punteroABitmap = malloc(cantidadBloques/8);

		t_bitarray* bitmap = bitarray_create_with_mode(punteroABitmap, cantidadBloques, MSB_FIRST);

		FILE* bitmapFile = fopen(path, "wrb");
		fwrite(punteroABitmap, 1, cantidadBloques/8, bitmapFile);
		fclose(bitmapFile);
		bitarray_destroy(bitmap);
		free(punteroABitmap);
	}

	free(path);

	log_info(logger, "Genere el bitmap");
}


void* armarFolderPara(char* nombre){
	char* path = string_new();
	string_append(&path,puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, nombre);
	mkdir(path, 0777);
	free(path);
	log_info(logger, "Genere la carpeta para %s", nombre);
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
	config_destroy(md);
	free(path);
	log_info(logger, "Cree el Metadata para %s", nombre);
}

// --------------------- INTERACCION CON ARCHIVOS --------------------- //

int archivoAbierto(char* path){
	t_config* md = config_create(path);
	int abierto = !strcmp(config_get_string_value(md, "OPEN"), "Y");
	config_destroy(md);
	return abierto;
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
		free(nombre);
		free(path);
	    return 0;
	} else {
	    fclose(archivo);
	    free(nombre);
	    free(path);
	    return 1;
	}
}

// --------------------- FUNCIONES PRINCIPALES --------------------- //


void agregarPokemon(NewPokemonConIDs* newPokemon){
	FILE* metadata;
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
		free(escritura);
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

int existePosicionPokemon(char* bloque, CoordenadasXY coordenadas){
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Blocks/");
	string_append(&path, bloque);
	string_append(&path, ".bin");

	char* centinela = string_new();
	char* coorX = string_itoa(coordenadas.posicionX);
	char* coorY = string_itoa(coordenadas.posicionY);
	string_append(&centinela,coorX);
	string_append(&centinela,"-");
	string_append(&centinela,coorY);

	char* leido;
	char** subLeido;
	void* punteroATexto;
	int file;

	int encontrado = 0;
	int cursor = 0;

	file = open(path, O_CREAT | O_RDWR, 0664);
	punteroATexto = mmap(NULL, tamanioBloque, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);

	while(strcmp(punteroATexto, "") && !encontrado){

		leido = punteroATexto;
		subLeido = string_split(leido, "=");
		if(!strcmp(subLeido[0], centinela)){
				char* nuevaLinea = string_new();
				int cantidad = atoi(subLeido[1]);
			if(cantidad == 1){

				eliminarLinea(cursor ,path);

			} else {
				string_append(&nuevaLinea, subLeido[0]);
				string_append(&nuevaLinea, "=");
				char* nuevaCantidad = string_itoa(cantidad - 1);
				string_append(&nuevaLinea, nuevaCantidad);
				free(nuevaCantidad);
			}
			memcpy(punteroATexto, nuevaLinea, strlen(nuevaLinea));
			msync(NULL, tamanioBloque, 0);
			free(nuevaLinea);
			encontrado = 1;
		}
		liberar_lista(subLeido);
		punteroATexto = punteroATexto + strlen(leido);
		cursor++;
	}

	close(file);
	free(path);
	free(centinela);
	free(coorX);
	free(coorY);
	return encontrado;
}

int eliminarSiEsArchivoVacio(char* bloque){
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Blocks/");
	string_append(&path, bloque);
	string_append(&path, ".bin");
	int seElimina = 0;

	if(tamanioDeBloque(bloque) == 0){
	  seElimina = 1;
	}
	if(seElimina){
		remove(path);
		log_info(logger, "Elimine el archivo %s", path);
	}
	free(path);
	return seElimina;
}

void eliminarLinea(int lno, char* path){
	int ctr = 0;
	FILE *fptr1, *fptr2;
	char* str;
	char* temp = string_duplicate("temp.bin");

	fptr1 = fopen(path, "r");
	fptr2 = fopen(temp, "w");

	lno++;

	while (!feof(fptr1))
	{
		str = string_duplicate("\0");
		fgets(str, tamanioBloque, fptr1);
		if (!feof(fptr1))
		{
			ctr++;
			/* skip the line at given line number */
			if (ctr != lno)
			{
				fprintf(fptr2, "%s", str);
			}
		}
	}
	fclose(fptr1);
	fclose(fptr2);
	remove(path);  		// remove the original file
	rename(temp, path); // rename the temporary file to original name
	log_info(logger, "Se elimino la linea %d del archivo %s", lno, path);
	free(temp);
	free(str);

}

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
		while(bloques[i]!=NULL && !existePosicionPokemon(bloques[i], pokemon->catchPokemon->coordenadas)){
			i++;
		}
		if(bloques[i] == NULL){
			log_info(logger,"No existe %s en esa posición", pokemon->catchPokemon->nombre);
			md = config_create(path);
			config_set_value(md,"OPEN","N");
			config_save(md);
			config_destroy(md);
		} else {
			encontrado = 1;
			if(eliminarSiEsArchivoVacio(bloques[i])){
				md = config_create(path);
				config_set_value(md,"OPEN","N");
				config_save(md);
				config_destroy(md);
				eliminarBloqueDeMetadata(path ,bloques[i]);
				eliminarSiEsCarpetaVacia(path, pathCarpeta);
			} else {
				md = config_create(path);
				config_set_value(md,"OPEN","N");
				config_save(md);
				config_destroy(md);
			}
		}
		liberar_lista(bloques);
		actualizarTamanioPokemon(pokemon->catchPokemon->nombre);
	}
	sleep(tiempoRetardo);
	enviarMensajeCaught(pokemon->IDmensaje, encontrado);
	printf("Envie un Caught\n\n");
	free(path);
	free(pathCarpeta);
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
	if(!existePokemon(pokemon->getPokemon->nombre)){
		log_info(logger,"No existe el pokemon %s",pokemon->getPokemon->nombre);
		t_list* paresOrdenados = list_create();

		sleep(tiempoRetardo);
		enviarMensajeLocalized(pokemon->IDmensaje, pokemon->getPokemon->nombre, paresOrdenados);

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
			printf("Envie un localized\n\n");
			CoordenadasXY* coordenadaAux;
			for(int j = 0; j < list_size(coordenadas); j++){
				coordenadaAux = list_get(coordenadas, j);
				free(coordenadaAux);
			}
			list_destroy(coordenadas);
			liberar_lista(bloques);
			actualizarTamanioPokemon(pokemon->getPokemon->nombre);
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

	char* caracterLeido = malloc(1);
	char* caracterAuxiliar;
	char** subleido;
	char** lasCoor;
	char* coorX;
	char* coorY;

	FILE* fd = fopen(path, "rt");
	char finDeLinea = '\n';
	CoordenadasXY* posicion;

	int cantidad;

	char* leido;

	fread(caracterLeido, 1, 1, fd);
	caracterAuxiliar = string_substring_until(caracterLeido, 1);
	while(!feof(fd)){
		leido = string_new();
		while(caracterLeido[0] != finDeLinea){
			string_append(&leido, caracterAuxiliar);
			free(caracterAuxiliar);
			fread(caracterLeido, 1, 1, fd);
			caracterAuxiliar = string_substring_until(caracterLeido, 1);
		}
		subleido = string_split(leido, "=");
		lasCoor = string_split(subleido[0], "-");
		cantidad = atoi(subleido[1]);
		coorX = string_duplicate(lasCoor[0]);
		coorY = string_duplicate(lasCoor[1]);
		for(int i=0; i<cantidad; i++){
			posicion = malloc(sizeof(CoordenadasXY));
			posicion->posicionX = atoi(coorX);
			posicion->posicionY = atoi(coorY);
			list_add(posiciones, posicion);
		}
		free(caracterAuxiliar);
		fread(caracterLeido, 1, 1, fd);
		caracterAuxiliar = string_substring_until(caracterLeido, 1);
		free(leido);
		liberar_lista(subleido);
		liberar_lista(lasCoor);
		free(coorX);
		free(coorY);
	}

	free(caracterAuxiliar);
	free(caracterLeido);
	fclose(fd);
	free(path);

	return posiciones;
}


// --------------------- Enviar Mensajes --------------------- //

void enviarMensajeAppeared(int IDmensaje, char* pokemon, CoordenadasXY coordenadas){
	int socket_suscriptor = crear_conexion(ip, puerto);
	if(socket_suscriptor!=-1){
		AppearedPokemon* nuevo = malloc(sizeof(AppearedPokemon));
		nuevo->coordenadas = coordenadas;
		nuevo->nombre = pokemon;
		nuevo->tamanioNombrePokemon = strlen(pokemon) + 1;
		enviarAppearedPokemon(nuevo, socket_suscriptor, 0, IDmensaje);
		liberar_conexion(socket_suscriptor);
		free(nuevo);
	}

}

void enviarMensajeCaught(int IDmensaje, int resultado){
	int socket_suscriptor = crear_conexion(ip, puerto);
	if(socket_suscriptor!=-1){
		CaughtPokemon* nuevo = malloc(sizeof(CaughtPokemon));
		nuevo->atrapar = resultado;
		enviarCaughtPokemon(nuevo, socket_suscriptor, 0, IDmensaje);
		liberar_conexion(socket_suscriptor);
		free(nuevo);
	}
}

void enviarMensajeLocalized(int IDmensaje, char* pokemon, t_list* coordenadas){
	int socket_suscriptor = crear_conexion(ip, puerto);
	if(socket_suscriptor!=-1){
		LocalizedPokemon* nuevo = malloc(sizeof(LocalizedPokemon));
		nuevo->cantidadParesOrdenados = list_size(coordenadas);
		nuevo->tamanioNombrePokemon = sizeof(pokemon)+1;
		nuevo->nombre = pokemon;
		nuevo->paresOrdenados = coordenadas;
		enviarLocalizedPokemon(nuevo, socket_suscriptor, 0, IDmensaje);
		liberar_conexion(socket_suscriptor);
		free(nuevo);
	}
}

// --------------------- BITMAP --------------------- //


int obtenerYEscribirProximoDisponible(){
	char* path = string_new();

	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Metadata/Bitmap.bin");

	int bitmapFile = open(path, O_CREAT | O_RDWR, 0664);

	void* punteroABitmap = mmap(NULL, cantidadBloques/8, PROT_READ | PROT_WRITE, MAP_SHARED, bitmapFile, 0);

	t_bitarray* bitmap = bitarray_create_with_mode((char*)punteroABitmap, cantidadBloques, MSB_FIRST);

	for(int i=0; i<cantidadBloques; i++){
		if(bitarray_test_bit(bitmap, i) == 0){
			bitarray_set_bit(bitmap ,i);
			msync(NULL ,cantidadBloques/8 ,0);
			write(bitmapFile, punteroABitmap, cantidadBloques/8);
			close(bitmapFile);
			free(path);
			bitarray_destroy(bitmap);
			return i;
		}
	}
	close(bitmapFile);
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

	string_append(&path, "/Bitmap.bin");
	int bitmapFile = open(path, O_CREAT | O_RDWR, 0664);

	void* punteroABitmap = mmap(NULL, cantidadDeBloques/8, PROT_READ | PROT_WRITE, MAP_SHARED, bitmapFile, 0);

	t_bitarray* bitmap = bitarray_create_with_mode((char*)punteroABitmap, cantidadDeBloques, MSB_FIRST);

	bitarray_clean_bit(bitmap, index);

	msync(NULL ,cantidadDeBloques/8 ,0);

	close(bitmapFile);
	bitarray_destroy(bitmap);

	config_destroy(md);
	free(cantBloques);
	free(path);
}

// --------------------- METADATA --------------------- //

void actualizarTamanioPokemon(char* pokemon){
	char* path = string_new();
	string_append(&path, puntoMontaje);
	string_append(&path, "/TALL_GRASS/Files/");
	string_append(&path, pokemon);
	string_append(&path, "/Metadata.bin");

	int tamanio = 0;
	int i = 0;

	t_config* md = config_create(path);
	char** bloques = config_get_array_value(md, "BLOCKS");

	if(!strcmp(bloques[i],"")){
		tamanio = 0;
	} else {
		while(bloques[i]!=NULL){
			tamanio = tamanio + tamanioDeBloque(bloques[i]);
			i++;
		}
	}

	char* tamanioTotal = string_itoa(tamanio);
	config_set_value(md, "SIZE", tamanioTotal);
	free(tamanioTotal);
	config_save(md);
	config_destroy(md);
	free(path);
	liberar_lista(bloques);
	log_info(logger, "Actualiza SIZE de %s", pokemon);
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

// --------------------- AGREGAR --------------------- //

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
					string_append(&escritura, "\n");

					free(nuevaCant);
					free(cantidadAnterior);
					leido[i] = escritura;
			}
			//free(escritura);
			i++;
	}

	i = 0;
	if(carga){
		escribirBloques(bloques, leido, tamanioMax, newPokemon->newPokemon->nombre);
	} else {
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
	//free(escritura);
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
	free(escritura);
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

	while(bloques[i]!=NULL){
		aux = lecturaBloque(bloques[i]);
		memcpy(lectura, aux, strlen(aux));
		i++;
		lectura = lectura + strlen(aux);
	}

	char** leido = string_split(original, "/n");

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

	void* punteroATexto = malloc(sizeof(tamanioBloque));

	read(file, punteroATexto, tamanioBloque);

	close(file);

	centinela = punteroATexto;

	return centinela;
}

int esBloqueLleno(char* numBloque, char* escritura){
	int lleno = tamanioDeBloque(numBloque) +strlen(escritura) >= tamanioBloque;
	return lleno;
}

void escribirBloques(char** bloques,char** leido,int tamanioMax, char* nombre){
	int i = 0;
	int j = 0;
	int cantBloquesAEscribir = cantidadDeBloques(leido);
	int cantBloques = cantidadDeBloques(bloques);
	int tamanioEscritura = 0;
	char* escritura;
	while(bloques[i]!=NULL){
		escritura = string_new();
		tamanioEscritura += strlen(leido[j]);
		while(tamanioEscritura < tamanioMax && leido[j]!=NULL){
			string_append(&escritura, leido[j]);
			j++;
			if(leido[j] != NULL){
				tamanioEscritura += strlen(leido[j]);
			}
		}
		pisarBloque(escritura, bloques[i]);
		//free(escritura);
		tamanioEscritura = 0;
		i++;
	}
	while(leido[j]!=NULL){
		escritura = string_new();
		tamanioEscritura += strlen(leido[j]);
		while(tamanioEscritura < tamanioMax && leido[j]!=NULL){
			string_append(&escritura, leido[j]);
			j++;
			tamanioEscritura += strlen(leido[j]);
		}
		agregarBloqueAMetadata(escritura, nombre);
		tamanioEscritura = 0;
		free(escritura);
	}
}


// --------------------- ELIMINAR --------------------- //

void eliminarBloqueDeMetadata(char* path, char* bloqueAEliminar){

	char* nuevoBloques = string_new();
	t_config* md = config_create(path);
	char** bloques = config_get_array_value(md, "BLOCKS");
	int i = 0;
	string_append(&nuevoBloques, "[");
	while(bloques[i+1] != NULL){
		if(strcmp(bloques[i+1], bloqueAEliminar)){
			string_append(&nuevoBloques, bloques[i+1]);
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

// --------------------- BUSCAR --------------------- //

// --------------------- AUX --------------------- //

void liberar_lista(char** lista){
	int contador = 0;
	while(lista[contador] != NULL){
			free(lista[contador]);
			contador++;
	}
	free(lista);
}

