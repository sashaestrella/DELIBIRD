#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<src/utilsGeneral.h>

t_log* iniciar_logger(void);
t_config* leer_config(void);
void suscribirNewPokemon();
void suscribirLocalizedPokemon();
void suscribirGetPokemon();
void suscribirAppearedPokemon();
void suscribirCatchPokemon();
void suscribirCaughtPokemon();
void terminar_programa(int, t_log*, t_config*);

#endif /* GAMEBOY_H_ */
