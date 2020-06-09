#include "adminMensajes.h"

void generarConexiones(int tipoSuscriptor){

	// -- Hilo de suscripcion a cola appeared -- //
	ParametrosSuscripcion* appeared = malloc(sizeof(ParametrosSuscripcion));
	appeared->conexionCola = &conexionAppeared;
	appeared->colaASuscribirse = SUSCRIPTOR_APPEAREDPOKEMON;
	appeared->nuevoExistente = tipoSuscriptor;
	appeared->IDSuscripcion = &IDsuscripcionAppeared;

	pthread_t hiloAppeared;
	pthread_create(&hiloAppeared, NULL, suscribirseACola, appeared); //Vamos a ver como sacar esto


	// -- Hilo de suscripcion a cola caught -- //
	ParametrosSuscripcion* caught = malloc(sizeof(ParametrosSuscripcion));
	caught->conexionCola = conexionCaught;
	caught->colaASuscribirse = SUSCRIPTOR_CAUGHTPOKEMON;
	caught->nuevoExistente = tipoSuscriptor;
	caught->IDSuscripcion = IDsuscripcionCaught;

	pthread_t hiloCaught;
	pthread_create(&hiloCaught, NULL, suscribirseACola, caught);


	// -- Hilo de suscripcion a cola localized -- //
	ParametrosSuscripcion* localized = malloc(sizeof(ParametrosSuscripcion));
	localized->conexionCola = conexionLocalized;
	localized->colaASuscribirse = SUSCRIPTOR_LOCALIZEDPOKEMON;
	localized->nuevoExistente = tipoSuscriptor;
	localized->IDSuscripcion = IDsuscripcionLocalized;

	pthread_t hiloLocalized;
	//pthread_create(&hiloLocalized, NULL, suscribirseACola, localized);


	//pthread_join(hiloAppeared,NULL);
	//pthread_join(hiloCaught,NULL);
	//pthread_join(hiloLocalized,NULL);
}

void abrirEscuchas(){
	pthread_t escuchaAppeared;
	pthread_create(&escuchaAppeared, NULL, recibirMensajesAppeared, (conexionAppeared, IDsuscripcionAppeared));
	pthread_detach(escuchaAppeared);

	pthread_t escuchaLocalized;
	pthread_create(&escuchaLocalized, NULL, recibirMensajesLocalized, (conexionLocalized, IDsuscripcionLocalized));
	pthread_detach(escuchaLocalized);

	pthread_t escuchaCaught;
	pthread_create(&escuchaCaught, NULL, recibirMensajesCaught, (conexionCaught, IDsuscripcionCaught));
	pthread_detach(escuchaCaught);
}



