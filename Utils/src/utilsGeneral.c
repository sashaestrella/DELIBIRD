#include "utilsGeneral.h"
#include "utilsEstructuras.h"


// int mutex = 0;

void iniciar_servidor(void)
{
	int socket_servidor;

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, PUERTO, &hints, &servinfo);

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
    	esperar_cliente(socket_servidor);
}

void esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
	pthread_detach(thread);

}

void serve_client(int* socket)
{
	int cod_op;
	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
		cod_op = -1;
	process_request(cod_op, *socket);
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

void process_request(int cod_op, int cliente_fd) {
	int size;

	suscriptores = list_create();
	void* algoARecibir;

		switch (cod_op) {
				case MENSAJE:
					algoARecibir = recibir_mensaje(cliente_fd, &size);
					devolver_mensaje(algoARecibir, size, cliente_fd);
					free(algoARecibir);
					break;
				case 0:
					pthread_exit(NULL);
				case -1:
					pthread_exit(NULL);
				case NEW_POKEMON:
					algoARecibir = recibir_NEW_POKEMON(cliente_fd, &size);
					enviarNewPokemon(algoARecibir,size,cliente_fd);
					free(algoARecibir);
					break;
				case LOCALIZED_POKEMON:
					algoARecibir = recibir_LOCALIZED_POKEMON(cliente_fd,&size);
					enviarLocalizedPokemon(algoARecibir,size,cliente_fd);
					free(algoARecibir);
					break;
				case GET_POKEMON:
					algoARecibir = recibir_GET_POKEMON(cliente_fd,&size);
					enviarGetPokemon(algoARecibir,size,cliente_fd);
					free(algoARecibir);
					break;
				case APPEARED_POKEMON:
					algoARecibir = recibir_APPEARED_POKEMON(cliente_fd,&size);
					enviarAppearedPokemon(algoARecibir,size,cliente_fd);
					free(algoARecibir);
					break;
				case CATCH_POKEMON:
					algoARecibir = recibir_CATCH_POKEMON(cliente_fd,&size);
					enviarCatchPokemon(algoARecibir,size,cliente_fd);
					free(algoARecibir);
					break;
				case CAUGHT_POKEMON:
					algoARecibir = recibir_CAUGHT_POKEMON(cliente_fd,&size);
					enviarCaughtPokemon(algoARecibir,size,cliente_fd);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_NEWPOKEMON:
					list_add(suscriptores,&cliente_fd);
					//algoARecibir = solicMensajeNewPokemon(cliente_fd);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_LOCALIZEDPOKEMON:
					list_add(suscriptores,&cliente_fd);
					//algoARecibir = solicMensajeLocalizedPokemon(cliente_fd);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_GETPOKEMON:
					list_add(suscriptores,&cliente_fd);
					//algoARecibir = solicMensajeGetPokemon(cliente_fd);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_APPEAREDPOKEMON:
					list_add(suscriptores,&cliente_fd);
					//algoARecibir = solicMensajeAppearedPokemon(cliente_fd);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_CATCHPOKEMON:
					list_add(suscriptores,&cliente_fd);
					//algoARecibir = solicMensajeCatchPokemon(cliente_fd);
					free(algoARecibir);
					break;
				case SUSCRIPTOR_CAUGHTPOKEMON:
					list_add(suscriptores,&cliente_fd);
					//algoARecibir = solicMensajeCaughtPokemon(cliente_fd);
					free(algoARecibir);
					break;

	}
}


void* recibir_NEW_POKEMON(int cliente_fd,int* size){
	int tamanioNombrePokemon;

	recv(cliente_fd,&tamanioNombrePokemon, sizeof(int),0);
	NewPokemon* unNewPokemon = malloc(sizeof(NewPokemon));

	char* nombrePokemon = malloc(tamanioNombrePokemon);
	recv(cliente_fd,nombrePokemon,tamanioNombrePokemon,0);
	unNewPokemon->nombre = nombrePokemon;
	recv(cliente_fd,&(unNewPokemon->coordenadas.posicionX),sizeof(uint32_t),0);
	recv(cliente_fd,&(unNewPokemon->coordenadas.posicionY),sizeof(uint32_t),0);
	recv(cliente_fd,&(unNewPokemon->cantidad),sizeof(uint32_t),0);

	list_add(New_Pokemon,unNewPokemon); //Ver si tiene que ir el & o no


	return unNewPokemon;
}



void* recibir_LOCALIZED_POKEMON(int cliente_fd,int* size){
	int tamanioNombrePokemon;
	LocalizedPokemon* localizedPokemon = malloc(sizeof(LocalizedPokemon));

	recv(cliente_fd,&tamanioNombrePokemon,sizeof(int),0);
	char* nombrePokemon = malloc(tamanioNombrePokemon);
	recv(cliente_fd,nombrePokemon,tamanioNombrePokemon,0);
	localizedPokemon->nombre = nombrePokemon;
	recv(cliente_fd,&(localizedPokemon->cantidadParesOrdenados),sizeof(uint32_t),0);

	for(int i=0; i<=localizedPokemon->cantidadParesOrdenados;i++){
		recv(cliente_fd,&(localizedPokemon->paresOrdenados),sizeof(t_list),0);
	}

	list_add(Localized_Pokemon,localizedPokemon);

	free(localizedPokemon);


	return localizedPokemon;
}


void* recibir_GET_POKEMON(int cliente_fd, int* size){
	int tamanioNombrePokemon;

	recv(cliente_fd,&tamanioNombrePokemon,sizeof(int),0);
	GetPokemon* unGetPokemon = malloc(sizeof(GetPokemon));
	char* nombrePokemon = malloc(tamanioNombrePokemon);
	recv(cliente_fd,nombrePokemon,tamanioNombrePokemon,0);
	unGetPokemon->nombre = nombrePokemon;

	list_add(Get_Pokemon,unGetPokemon);

	return unGetPokemon;
}


void* recibir_APPEARED_POKEMON(int cliente_fd,int* size){
	int tamanioNombrePokemon;

	recv(cliente_fd,&tamanioNombrePokemon, sizeof(int),0);
	AppearedPokemon* unAppearedPokemon = malloc(sizeof(AppearedPokemon));
	char* nombrePokemon = malloc(tamanioNombrePokemon);
	recv(cliente_fd,nombrePokemon,tamanioNombrePokemon,0);
	unAppearedPokemon->nombre = nombrePokemon;
	recv(cliente_fd,&(unAppearedPokemon->coordenadas.posicionX),sizeof(uint32_t),0);
	recv(cliente_fd,&(unAppearedPokemon->coordenadas.posicionY),sizeof(uint32_t),0);

	list_add(Appeared_Pokemon,unAppearedPokemon);

	return unAppearedPokemon;
}


void* recibir_CATCH_POKEMON(int cliente_fd,int*size){
	int tamanioNombrePokemon;

	recv(cliente_fd,&tamanioNombrePokemon, sizeof(int),0);
	CatchPokemon* catchPokemon = malloc(sizeof(CatchPokemon));
	char* nombrePokemon = malloc(tamanioNombrePokemon);
	recv(cliente_fd,nombrePokemon,tamanioNombrePokemon,0);
	catchPokemon->nombre = nombrePokemon;
	recv(cliente_fd,&(catchPokemon->coordenadas.posicionX),sizeof(uint32_t),0);
	recv(cliente_fd,&(catchPokemon->coordenadas.posicionY),sizeof(uint32_t),0);

	list_add(Catch_Pokemon,catchPokemon);

	return catchPokemon;
}

void* recibir_CAUGHT_POKEMON(int cliente_fd,int* size){

	CaughtPokemon* caughtPokemon = malloc(sizeof(CaughtPokemon));
	recv(cliente_fd,&(caughtPokemon->atrapar),sizeof(uint32_t),0);

	list_add(Caught_Pokemon,caughtPokemon);

	return caughtPokemon;
}

/*
void* solicMensajeNewPokemon(int socket_suscriptor){

	pthread_create(&hiloConexionSusc, NULL,(void*)enviarColaNewPokemon,NULL);
	pthread_join(hiloConexionSusc,NULL);

	return 0;
}

void* solicMensajeLocalizedPokemon(int socket_suscriptor){

	pthread_create(&hiloConexionSusc, NULL,(void*)enviarColaLocalizedPokemon,NULL);
	pthread_join(hiloConexionSusc,NULL);

	return 0;
}

void* solicMensajeGetPokemon(int socket_suscriptor){

	pthread_create(&hiloConexionSusc, NULL,(void*)enviarColaGetPokemon,NULL);
	pthread_join(hiloConexionSusc,NULL);

	return 0;
}

void* solicMensajeAppearedPokemon(int socket_suscriptor){

	pthread_create(&hiloConexionSusc, NULL,(void*)enviarColaAppearedPokemon,NULL);
	pthread_join(hiloConexionSusc,NULL);

	return 0;
}

void* solicMensajeCatchPokemon(int socket_suscriptor){

	pthread_create(&hiloConexionSusc, NULL,(void*)enviarColaCatchPokemon,NULL);
	pthread_join(hiloConexionSusc,NULL);

	return 0;
}

void* solicMensajeCaughtPokemon(int socket_suscriptor){

	pthread_create(&hiloConexionSusc, NULL,(void*)enviarColaCaughtPokemon,NULL);
	pthread_join(hiloConexionSusc,NULL);

	return 0;
}

*/

void* recibir_mensaje(int socket_cliente, int* size)
{
	void * buffer;
		recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
		buffer = malloc(*size);
		recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}


void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void* serializarNewPokemon(NewPokemon* newPokemon,int bytes){

	void* buffer = malloc(bytes);
	int tamanioNombre = strlen(newPokemon->nombre) + 1;
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento,&tamanioNombre,sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(buffer + desplazamiento,newPokemon->nombre,tamanioNombre);
	desplazamiento+= tamanioNombre;
	memcpy(buffer + desplazamiento,&(newPokemon->coordenadas.posicionX),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,&(newPokemon->coordenadas.posicionY),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,&(newPokemon->cantidad),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);


	return buffer;
}

void* serializarLocalizedPokemon(LocalizedPokemon* localizedPokemon,int bytes){

	void* buffer = malloc(bytes);
	int tamanioNombre = strlen(localizedPokemon->nombre) + 1;
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento,&tamanioNombre,sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(buffer + desplazamiento,localizedPokemon->nombre,tamanioNombre);
	desplazamiento+= tamanioNombre;
	memcpy(buffer + desplazamiento,&(localizedPokemon->cantidadParesOrdenados),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	//memcpy(buffer + desplazamiento,&(localizedPokemon->paresOrdenados),sizeof(size_t));
	//desplazamiento+= sizeof(size_t);

	return buffer;
}

void* serializarGetPokemon(GetPokemon* getPokemon,int bytes){

	void* buffer = malloc(bytes);
	int tamanioNombre = strlen(getPokemon->nombre) + 1;
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento,&tamanioNombre,sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(buffer + desplazamiento,getPokemon->nombre,tamanioNombre);
	desplazamiento+= tamanioNombre;

	return buffer;
}

void* serializarAppearedPokemon(AppearedPokemon* appearedPokemon,int bytes){

	void* buffer = malloc(bytes);
	int tamanioNombre = strlen(appearedPokemon->nombre) + 1;
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento,&tamanioNombre,sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(buffer + desplazamiento,appearedPokemon->nombre,tamanioNombre);
	desplazamiento+= tamanioNombre;
	memcpy(buffer + desplazamiento,&(appearedPokemon->coordenadas.posicionX),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,&(appearedPokemon->coordenadas.posicionY),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	return buffer;
}

void* serializarCatchPokemon(CatchPokemon* catchPokemon,int bytes){

	void* buffer = malloc(bytes);
	int tamanioNombre = strlen(catchPokemon->nombre) + 1;
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento,&tamanioNombre,sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(buffer + desplazamiento,catchPokemon->nombre,tamanioNombre);
	desplazamiento+= tamanioNombre;
	memcpy(buffer + desplazamiento,&(catchPokemon->coordenadas.posicionX),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,&(catchPokemon->coordenadas.posicionY),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	return buffer;
}

void* serializarCaughtPokemon(CaughtPokemon* caughtPokemon,int bytes){

	void* buffer = malloc(bytes);
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento,&(caughtPokemon->atrapar),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	return buffer;
}



void enviarNewPokemon(NewPokemon* new_pokemon,int size,int socket_suscriptor){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = NEW_POKEMON;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(sizeof(NewPokemon));
	memcpy(paquete->buffer->stream,new_pokemon,paquete->buffer->size);

	int bytes = 2* sizeof(int)+strlen(new_pokemon->nombre)+sizeof(new_pokemon->coordenadas.posicionX)+sizeof(new_pokemon->coordenadas.posicionY)+sizeof(new_pokemon->cantidad);

	void* a_enviar = serializarNewPokemon(new_pokemon,bytes);

	//pthread_mutex_lock(&mutex);

	send(socket_suscriptor,a_enviar,bytes,0);

	//pthread_mutex_unlock(&mutex);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(new_pokemon);
	free(paquete);
}

void enviarLocalizedPokemon(LocalizedPokemon* localized_pokemon,int size,int socket_suscriptor){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = LOCALIZED_POKEMON;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(sizeof(LocalizedPokemon));
	memcpy(paquete->buffer->stream,localized_pokemon,paquete->buffer->size);

	int bytes = 2* sizeof(int)+strlen(localized_pokemon->nombre)+sizeof(localized_pokemon->cantidadParesOrdenados)+sizeof(localized_pokemon->paresOrdenados);

	void* a_enviar = serializarLocalizedPokemon(localized_pokemon,bytes);

	//pthread_mutex_lock(&mutex);

	send(socket_suscriptor,a_enviar,bytes,0);

	//pthread_mutex_unlock(&mutex);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(localized_pokemon);
	free(paquete);

}

void enviarGetPokemon(GetPokemon* get_pokemon,int size,int socket_suscriptor){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = GET_POKEMON;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(sizeof(GetPokemon));
	memcpy(paquete->buffer->stream,get_pokemon,paquete->buffer->size);

	int bytes = 2* sizeof(int)+strlen(get_pokemon->nombre);

	void* a_enviar = serializarGetPokemon(get_pokemon,bytes);

	//pthread_mutex_lock(&mutex);

	send(socket_suscriptor,a_enviar,bytes,0);

	//pthread_mutex_unlock(&mutex);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(get_pokemon);
	free(paquete);
}

void enviarAppearedPokemon(AppearedPokemon* appeared_pokemon,int size,int socket_suscriptor){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = APPEARED_POKEMON;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(sizeof(AppearedPokemon));
	memcpy(paquete->buffer->stream,appeared_pokemon,paquete->buffer->size);

	int bytes = 2* sizeof(int)+strlen(appeared_pokemon->nombre)+sizeof(appeared_pokemon->coordenadas.posicionX)+sizeof(appeared_pokemon->coordenadas.posicionY);

	void* a_enviar = serializarAppearedPokemon(appeared_pokemon,bytes);

	//pthread_mutex_lock(&mutex);

	send(socket_suscriptor,a_enviar,bytes,0);

	//pthread_mutex_unlock(&mutex);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(appeared_pokemon);
	free(paquete);
}

void enviarCatchPokemon(CatchPokemon* catch_pokemon,int size,int socket_suscriptor){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = CATCH_POKEMON;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(sizeof(CatchPokemon));
	memcpy(paquete->buffer->stream,catch_pokemon,paquete->buffer->size);

	int bytes = 2* sizeof(int)+strlen(catch_pokemon->nombre)+sizeof(catch_pokemon->coordenadas.posicionX)+sizeof(catch_pokemon->coordenadas.posicionY);

	void* a_enviar = serializarCatchPokemon(catch_pokemon,bytes);

	//pthread_mutex_lock(&mutex);

	send(socket_suscriptor,a_enviar,bytes,0);

	//pthread_mutex_unlock(&mutex);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(catch_pokemon);
	free(paquete);

}

void enviarCaughtPokemon(CaughtPokemon* caught_pokemon,int size,int socket_suscriptor){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = CAUGHT_POKEMON;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(sizeof(CaughtPokemon));
	memcpy(paquete->buffer->stream,caught_pokemon,paquete->buffer->size);

	int bytes = sizeof(uint32_t);

	void* a_enviar = serializarCaughtPokemon(caught_pokemon,bytes);

	//pthread_mutex_lock(&mutex);

	send(socket_suscriptor,a_enviar,bytes,0);

	//pthread_mutex_unlock(&mutex);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(caught_pokemon);
	free(paquete);
}

void devolver_mensaje(void* payload, int size, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, payload, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes); //bytes string mensaje

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}


void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}

