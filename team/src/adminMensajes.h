#ifndef ADMINMENSAJES_H_
#define ADMINMENSAJES_H_

#include "team.h"

char* msgAppeared = "8 0"; //11
char* msgLocalized = "9 0";
char* msgCaught = "13 0";
char* msgGameBoy; //Dsp vemo

void generarConexiones();
void generarConexion(int, char*);
void* recibirMensaje(int);
void* adminMensajes(char*);


#endif /* ADMINMENSAJES_H_ */
