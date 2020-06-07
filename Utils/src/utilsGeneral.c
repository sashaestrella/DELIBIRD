#include "utilsGeneral.h"

void* serializarSuscripcion(int idSuscriptor,int bytes){
		void* buffer = malloc(bytes);
		int desplazamiento = 0;

		memcpy(buffer + desplazamiento,&(idSuscriptor),sizeof(int));
		desplazamiento+= sizeof(int);

		return buffer;
}


void enviarSuscripcion(int idSuscriptor,int socket_servidor, int cod_op){
		t_buffer* buffer = malloc(sizeof(t_buffer));

		void* stream;
		buffer->size = sizeof(int);
		stream = serializarSuscripcion(idSuscriptor,buffer->size);
		buffer->stream = stream;

		t_paquete* paquete = malloc(sizeof(t_paquete));
		paquete->codigo_operacion = cod_op;
		paquete->buffer = buffer;
		int bytes = buffer->size + sizeof(int) + sizeof(op_code);

		void* a_enviar = serializar_paquete(paquete,bytes);

		send(socket_servidor, a_enviar, bytes, 0);

		free(a_enviar);
		free(buffer->stream);
		free(paquete->buffer);
		free(paquete);
}


void* recibir_mensaje(int socket_cliente, int* size)
{
	void * buffer;
		recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
		buffer = malloc(*size);
		recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}


NewPokemonConIDs* recibir_NEW_POKEMON(int cliente_fd,int* size,int reciboID){

		NewPokemon* unNewPokemon = malloc(sizeof(NewPokemon));
		NewPokemonConIDs* newConIDs = malloc(sizeof(NewPokemonConIDs));
		newConIDs->newPokemon = unNewPokemon;
		t_buffer* buffer = malloc(sizeof(t_buffer));
		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		int id;
		if(reciboID > 0){
			memcpy(&id,stream,sizeof(int));
			stream+= sizeof(int);
			newConIDs->IDmensaje = id;
		}
		memcpy(&(unNewPokemon->tamanioNombrePokemon),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		unNewPokemon->nombre = malloc(unNewPokemon->tamanioNombrePokemon);
		memcpy(unNewPokemon->nombre,stream,unNewPokemon->tamanioNombrePokemon);
		stream+=unNewPokemon->tamanioNombrePokemon;
		memcpy(&(unNewPokemon->coordenadas.posicionX),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		memcpy(&(unNewPokemon->coordenadas.posicionY),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		memcpy(&(unNewPokemon->cantidad),stream,sizeof(uint32_t));

		free(buffer->stream);
		free(buffer);

		printf("\nMe llego el mensaje: %s\n",unNewPokemon->nombre);

		return newConIDs;
}


LocalizedPokemonConIDs* recibir_LOCALIZED_POKEMON(int cliente_fd,int* size,int reciboID){

		LocalizedPokemon* unLocalizedPokemon = malloc(sizeof(LocalizedPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));
		LocalizedPokemonConIDs* localizedConIdCorrelativo = malloc(sizeof(LocalizedPokemonConIDs));
		localizedConIdCorrelativo->localizedPokemon = unLocalizedPokemon;

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		int idCorrelativo,id;
		if(reciboID > 0){
			memcpy(&id,stream,sizeof(int));
			stream+=sizeof(int);
			localizedConIdCorrelativo->IDmensaje = id;
		}
		memcpy(&idCorrelativo,stream,sizeof(int));
		stream+=sizeof(int);
		localizedConIdCorrelativo->IDcorrelativo = idCorrelativo;

		memcpy(&(unLocalizedPokemon->tamanioNombrePokemon),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		unLocalizedPokemon->nombre = malloc(unLocalizedPokemon->tamanioNombrePokemon);
		memcpy(unLocalizedPokemon->nombre,stream,unLocalizedPokemon->tamanioNombrePokemon);
		stream+=unLocalizedPokemon->tamanioNombrePokemon;
		memcpy(&(unLocalizedPokemon->cantidadParesOrdenados),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);

		int cantidadDePares = (unLocalizedPokemon->cantidadParesOrdenados);
		int coordenadasXY = cantidadDePares * 2;
		t_list* unasCoordenadas = list_create();
		for(int i=0;i<coordenadasXY;i++){
			CoordenadasXY* coordenadas = malloc(sizeof(CoordenadasXY));
			memcpy(&(coordenadas->posicionX),stream,sizeof(uint32_t));
			stream+=sizeof(uint32_t);
			memcpy(&(coordenadas->posicionY),stream,sizeof(uint32_t));
			stream+=sizeof(uint32_t);
			list_add(unasCoordenadas,coordenadas);

		}

		unasCoordenadas = unLocalizedPokemon->paresOrdenados;

		printf("\nMe llego el mensaje: %s\n",localizedConIdCorrelativo->localizedPokemon->nombre);

		free(buffer->stream);
		free(buffer);

		return localizedConIdCorrelativo;
}



GetPokemonConIDs* recibir_GET_POKEMON(int cliente_fd, int* size,int reciboID){

		GetPokemon* unGetPokemon = malloc(sizeof(GetPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));
		GetPokemonConIDs* getConIDs = malloc(sizeof(GetPokemonConIDs));
		getConIDs->getPokemon = unGetPokemon;

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		int id;
		if(reciboID > 0){
			memcpy(&id,stream,sizeof(int));
			stream+=sizeof(int);
			getConIDs->IDmensaje = id;
		}

		memcpy(&(unGetPokemon->tamanioNombrePokemon),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		unGetPokemon->nombre = malloc(unGetPokemon->tamanioNombrePokemon);
		memcpy(unGetPokemon->nombre,stream,unGetPokemon->tamanioNombrePokemon);
		stream+=unGetPokemon->tamanioNombrePokemon;

		printf("\nMe llego el mensaje: %s\n",unGetPokemon->nombre);

		free(buffer->stream);
		free(buffer);

		return getConIDs;
}

AppearedPokemonConIDs* recibir_APPEARED_POKEMON(int cliente_fd,int* size,int reciboID,int reciboIDCorrelativo){

		AppearedPokemon* unAppearedPokemon = malloc(sizeof(AppearedPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));
		AppearedPokemonConIDs* appearedConIDs = malloc(sizeof(AppearedPokemonConIDs));
		appearedConIDs->appearedPokemon = unAppearedPokemon;

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		int idCorrelativo,id;
		if(reciboID > 0){
			memcpy(&id,stream,sizeof(int));
			stream+=sizeof(int);
			appearedConIDs->IDmensaje = id;
		}

		if(reciboIDCorrelativo){
			memcpy(&idCorrelativo,stream,sizeof(int));
			stream+= sizeof(int);
			appearedConIDs->IDcorrelativo = idCorrelativo;
		}
		memcpy(&(unAppearedPokemon->tamanioNombrePokemon),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		unAppearedPokemon->nombre = malloc(unAppearedPokemon->tamanioNombrePokemon);
		memcpy(unAppearedPokemon->nombre,stream,unAppearedPokemon->tamanioNombrePokemon);
		stream+=unAppearedPokemon->tamanioNombrePokemon;
		memcpy(&(unAppearedPokemon->coordenadas.posicionX),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		memcpy(&(unAppearedPokemon->coordenadas.posicionY),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);

		printf("\nMe llego el mensaje: %s\n",unAppearedPokemon->nombre);

		free(buffer->stream);
		free(buffer);

		return appearedConIDs;
}


CatchPokemonConIDs* recibir_CATCH_POKEMON(int cliente_fd,int*size,int reciboID){

		CatchPokemon* unCatchPokemon = malloc(sizeof(CatchPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));
		CatchPokemonConIDs* catchConIDs = malloc(sizeof(CatchPokemonConIDs));
		catchConIDs->catchPokemon = unCatchPokemon;

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		int id;
		if(reciboID){
			memcpy(&id,stream,sizeof(int));
			stream+= sizeof(int);
			catchConIDs->IDmensaje = id;
		}

		memcpy(&(unCatchPokemon->tamanioNombrePokemon),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		unCatchPokemon->nombre = malloc(unCatchPokemon->tamanioNombrePokemon);
		memcpy(unCatchPokemon->nombre,stream,unCatchPokemon->tamanioNombrePokemon);
		stream+=unCatchPokemon->tamanioNombrePokemon;
		memcpy(&(unCatchPokemon->coordenadas.posicionX),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);
		memcpy(&(unCatchPokemon->coordenadas.posicionY),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);

		printf("\nMe llego el mensaje: %s\n",unCatchPokemon->nombre);

		free(buffer->stream);
		free(buffer);

		return catchConIDs;
}

CaughtPokemonConIDs* recibir_CAUGHT_POKEMON(int cliente_fd,int* size,int reciboID){

		CaughtPokemon* unCaughtPokemon = malloc(sizeof(CaughtPokemon));
		t_buffer* buffer = malloc(sizeof(t_buffer));
		CaughtPokemonConIDs* unCaughtPokemonConIDCorrelativo = malloc(sizeof(CaughtPokemonConIDs));
		unCaughtPokemonConIDCorrelativo->caughtPokemon = unCaughtPokemon;

		recv(cliente_fd,&(buffer->size),sizeof(int),MSG_WAITALL);
		void* stream = malloc(buffer->size);
		buffer->stream = stream;
		recv(cliente_fd,buffer->stream,buffer->size,MSG_WAITALL);

		int idCorrelativo,id;
		if(reciboID > 0){
			memcpy(&id,stream,sizeof(int));
			stream+=sizeof(int);
			unCaughtPokemonConIDCorrelativo->IDmensaje = id;
		}

		memcpy(&idCorrelativo,stream,sizeof(int));
		stream+=sizeof(int);
		unCaughtPokemonConIDCorrelativo->IDCorrelativo = idCorrelativo;

		memcpy(&(unCaughtPokemon->atrapar),stream,sizeof(uint32_t));
		stream+=sizeof(uint32_t);

		printf("\nEl pokemon fue atrapado?: %d\n",unCaughtPokemon->atrapar);

		free(buffer->stream);
		free(buffer);

		return unCaughtPokemonConIDCorrelativo;
}



void* serializar_paquete(t_paquete* paquete, int bytes)
{

	void * a_enviar = malloc(bytes);
	int desplazamiento = 0;

	memcpy(a_enviar + desplazamiento, &(paquete->codigo_operacion), sizeof(op_code));
	desplazamiento+= sizeof(op_code);
	memcpy(a_enviar + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(a_enviar + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;
	return a_enviar;
}





void* serializarLocalizedPokemon(LocalizedPokemon* localizedPokemon,int bytes, int id,int idCorrelativo){

	void* buffer = malloc(bytes);
	uint32_t tamanioNombre = strlen(localizedPokemon->nombre) + 1;
	int desplazamiento = 0;

	if(id > 0){
		memcpy(buffer + desplazamiento,&id,sizeof(int));
		desplazamiento += sizeof(int);
	}
	memcpy(buffer + desplazamiento,&idCorrelativo,sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(buffer + desplazamiento,&tamanioNombre,sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,localizedPokemon->nombre,tamanioNombre);
	desplazamiento+= tamanioNombre;
	memcpy(buffer + desplazamiento,&(localizedPokemon->cantidadParesOrdenados),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	for(int i=0;i<localizedPokemon->cantidadParesOrdenados;i++){
		CoordenadasXY* coordenadas = list_get(localizedPokemon->paresOrdenados,i);
		memcpy(buffer + desplazamiento,&(coordenadas->posicionX),sizeof(uint32_t));
		desplazamiento+= sizeof(uint32_t);
		memcpy(buffer + desplazamiento,&(coordenadas->posicionY),sizeof(uint32_t));
		desplazamiento+= sizeof(uint32_t);
	}

	return buffer;
}

void* serializarGetPokemon(GetPokemon* getPokemon,int bytes,int id){

	void* buffer = malloc(bytes);
	uint32_t tamanioNombre = strlen(getPokemon->nombre) + 1;
	int desplazamiento = 0;

	if(id > 0){
		memcpy(buffer + desplazamiento,&id,sizeof(int));
		desplazamiento+= sizeof(int);
	}

	memcpy(buffer + desplazamiento,&tamanioNombre,sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,getPokemon->nombre,tamanioNombre);
	desplazamiento+= tamanioNombre;

	return buffer;
}

void* serializarAppearedPokemon(AppearedPokemon* appearedPokemon,int bytes,int id,int idCorrelativo){

	void* buffer = malloc(bytes);
	uint32_t tamanioNombre = strlen(appearedPokemon->nombre) + 1;
	int desplazamiento = 0;

	if(id > 0){
		memcpy(buffer + desplazamiento,&id,sizeof(int));
		desplazamiento+= sizeof(int);
	}
	if(idCorrelativo > 0){
		memcpy(buffer + desplazamiento,&idCorrelativo,sizeof(int));
		desplazamiento+= sizeof(int);
	}
	memcpy(buffer + desplazamiento,&tamanioNombre,sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,appearedPokemon->nombre,tamanioNombre);
	desplazamiento+= tamanioNombre;
	memcpy(buffer + desplazamiento,&(appearedPokemon->coordenadas.posicionX),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,&(appearedPokemon->coordenadas.posicionY),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	return buffer;
}

void* serializarCatchPokemon(CatchPokemon* catchPokemon,int bytes,int id){

	void* buffer = malloc(bytes);
	uint32_t tamanioNombre = strlen(catchPokemon->nombre) + 1;
	int desplazamiento = 0;

	if(id > 0){
		memcpy(buffer + desplazamiento,&id,sizeof(int));
		desplazamiento+= sizeof(int);
	}

	memcpy(buffer + desplazamiento,&tamanioNombre,sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,catchPokemon->nombre,tamanioNombre);
	desplazamiento+= tamanioNombre;
	memcpy(buffer + desplazamiento,&(catchPokemon->coordenadas.posicionX),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento,&(catchPokemon->coordenadas.posicionY),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	return buffer;
}

void* serializarCaughtPokemon(CaughtPokemon* caughtPokemon,int bytes,int id,int idCorrelativo){

	void* buffer = malloc(bytes);
	int desplazamiento = 0;

	if(id > 0){
		memcpy(buffer + desplazamiento,&id,sizeof(int));
		desplazamiento+= sizeof(int);
	}
	memcpy(buffer + desplazamiento,&idCorrelativo,sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(buffer + desplazamiento,&(caughtPokemon->atrapar),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	return buffer;
}

void* serializarNewPokemon(NewPokemon* newPokemon,int bytes,int id){

	void* stream = malloc(bytes);
	uint32_t tamanioNombre = strlen(newPokemon->nombre) + 1;
	int desplazamiento = 0;

	if(id > 0){
		memcpy(stream + desplazamiento,&id,sizeof(int));
		desplazamiento+= sizeof(int);
	}
	memcpy(stream + desplazamiento,&tamanioNombre,sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(stream + desplazamiento,newPokemon->nombre,tamanioNombre);
	desplazamiento+= tamanioNombre;
	memcpy(stream + desplazamiento,&(newPokemon->coordenadas.posicionX),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(stream + desplazamiento,&(newPokemon->coordenadas.posicionY),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(stream + desplazamiento,&(newPokemon->cantidad),sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);


	return stream;
}

void enviarNewPokemon(NewPokemon* unNewPokemon, int socket_suscriptor,int id){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	uint32_t tamanioNombrePokemon = strlen(unNewPokemon->nombre) +1;
	void* stream;
	if(id > 0){
		buffer->size = sizeof(uint32_t) * 4 + tamanioNombrePokemon + sizeof(int);
	} else {
		buffer->size = sizeof(uint32_t) * 4 + tamanioNombrePokemon;
	}
	stream = serializarNewPokemon(unNewPokemon,buffer->size,id);
	buffer->stream = stream;

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = NEW_POKEMON;
	paquete->buffer = buffer;
	int bytes = buffer->size + sizeof(int) + sizeof(op_code);

	void* a_enviar = serializar_paquete(paquete,bytes);

	send(socket_suscriptor, a_enviar, bytes, 0);

	free(a_enviar);
	free(buffer->stream);
	free(paquete->buffer);
	free(paquete);


}

void enviarLocalizedPokemon(LocalizedPokemon* localized_pokemon,int socket_suscriptor,int id, int idCorrelativo){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	int tamanioNombrePokemon = strlen(localized_pokemon->nombre) +1;
	int coordenadas = localized_pokemon->cantidadParesOrdenados * 2;
	if(id > 0){
		buffer->size = sizeof(uint32_t) * 2 * coordenadas + tamanioNombrePokemon + sizeof(int)*2;
	}else {
		buffer->size = sizeof(uint32_t) * 2 * coordenadas + tamanioNombrePokemon + sizeof(int);
	}
	void* stream = serializarLocalizedPokemon(localized_pokemon,buffer->size,id,idCorrelativo);
	buffer->stream = stream;
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = LOCALIZED_POKEMON;
	paquete->buffer = buffer;
	int bytes = buffer->size + sizeof(int) + sizeof(op_code);

	void* a_enviar = serializar_paquete(paquete,bytes);

	send(socket_suscriptor, a_enviar, bytes, 0);

	free(a_enviar);
	free(buffer->stream);
	free(paquete->buffer);
	free(paquete);

}

void enviarGetPokemon(GetPokemon* get_pokemon,int socket_suscriptor,int id){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	uint32_t tamanioNombrePokemon = strlen(get_pokemon->nombre) +1;

	if(id > 0){
		buffer->size = sizeof(uint32_t) + tamanioNombrePokemon + sizeof(int);
	}else {
		buffer->size = sizeof(uint32_t) + tamanioNombrePokemon;
	}
	void* stream = serializarGetPokemon(get_pokemon,buffer->size,id);
	buffer->stream = stream;
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = GET_POKEMON;
	paquete->buffer = buffer;
	int bytes = buffer->size + sizeof(int) + sizeof(op_code);

	void* a_enviar = serializar_paquete(paquete,bytes);

	send(socket_suscriptor, a_enviar, bytes, 0);

	free(a_enviar);
	free(buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviarAppearedPokemon(AppearedPokemon* appeared_pokemon,int socket_suscriptor,int id,int idCorrelativo){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	uint32_t tamanioNombrePokemon = strlen(appeared_pokemon->nombre) +1;
	buffer->size = sizeof(uint32_t) * 3 + tamanioNombrePokemon + sizeof(int);

	void* stream = serializarAppearedPokemon(appeared_pokemon,buffer->size,id,idCorrelativo);
	buffer->stream = stream;
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = APPEARED_POKEMON;
	paquete->buffer = buffer;
	int bytes = buffer->size + sizeof(int) + sizeof(op_code);

	void* a_enviar = serializar_paquete(paquete,bytes);

	send(socket_suscriptor, a_enviar, bytes, 0);

	free(a_enviar);
	free(buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviarCatchPokemon(CatchPokemon* catch_pokemon,int socket_suscriptor,int id){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	uint32_t tamanioNombrePokemon = strlen(catch_pokemon->nombre) +1;

	if(id > 0){
		buffer->size = sizeof(uint32_t)*3 + tamanioNombrePokemon + sizeof(int);
	}else {
		buffer->size = sizeof(uint32_t)*3 + tamanioNombrePokemon;
	}
	void* stream = serializarCatchPokemon(catch_pokemon,buffer->size,id);
	buffer->stream = stream;
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = CATCH_POKEMON;
	paquete->buffer = buffer;
	int bytes = buffer->size + sizeof(int) + sizeof(op_code);

	void* a_enviar = serializar_paquete(paquete,bytes);

	send(socket_suscriptor, a_enviar, bytes, 0);

	free(a_enviar);
	free(buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviarCaughtPokemon(CaughtPokemon* caught_pokemon,int socket_suscriptor,int id,int idCorrelativo){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	if(id > 0){
		buffer->size = sizeof(uint32_t) + sizeof(int)*2;
	}else {
		buffer->size = sizeof(uint32_t) + sizeof(int);
	}
	void* stream = serializarCaughtPokemon(caught_pokemon,buffer->size,id,idCorrelativo);
	buffer->stream = stream;
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = CAUGHT_POKEMON;
	paquete->buffer = buffer;
	int bytes = buffer->size + sizeof(int) + sizeof(op_code);

	void* a_enviar = serializar_paquete(paquete,bytes);

	send(socket_suscriptor, a_enviar, bytes, 0);

	free(a_enviar);
	free(buffer->stream);
	free(paquete->buffer);
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
