/*
 * main.cc
 *
 *  Este programa contiene la comunicacion desde la Raspberry
 *  hacia el MBed, asi como el control de envio y llegada de
 *  mensajes desde el modulo Wifi
 *
 *  Created on: 15/6/2015
 *      Author: Nestor
 */

#include "Librerias/libraries.h"
#include "MBed.h"


//----------------------------------------------------------------------------------------
/**
 *  Ejecutamos la inicializacion y la comunicacion
 *  entre la Raspberry y el MBed
 */
//----------------------------------------------------------------------------------------
int main(int argc, char** argv){

	//Mientras la conexion serial con el MBED no este establecida lo seguimos intentando
	while(!initMBed());

	//Permanecemos el bucle escuchando lo que nos diga el MBed y actuando en consecuencia
	loop();

    return 0;
}//Final metodo main()
