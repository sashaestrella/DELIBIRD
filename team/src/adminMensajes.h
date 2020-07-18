#ifndef ADMINMENSAJES_H_
#define ADMINMENSAJES_H_

#include "team.h"

char* IP_TEAM;
char* PUERTO_TEAM;

void generarConexiones();
void abrirEscuchas();
void empezarAnalizadoresDeMensajes();

void enviar_getPokemon(GetPokemon*);
CatchPokemonConIDs* enviar_catchPokemon(CatchPokemon*);
bool descartar_localized_no_deseados(LocalizedPokemonConIDs*);
bool descartar_caught_no_deseados(CaughtPokemonConIDs*);
bool descartar_appeared_no_deseados(AppearedPokemonConIDs*);

// ----- Auxiliares ----- //



void noHayBroker();
void suscribirseAColaAppeared();
void suscribirseAColaLocalized();
void suscribirseAColaCaught();
void administradorMensajesColas(int, int, int);
void enviarACK(int, int, int, int);

void recibirMensajesAppeared();
void recibirMensajesLocalized();
void recibirMensajesCaught();
void adminMensajeAppeared(AppearedPokemonConIDs*);
void adminMensajeLocalized(LocalizedPokemonConIDs*);
void adminMensajeCaught(CaughtPokemonConIDs*);


pthread_t hiloServidorTeam;
void iniciar_servidor_team(void);
void esperar_cliente_team(int);
void serve_client_team(int*);
void process_request_team(int, int);



#endif /* ADMINMENSAJES_H_ */
