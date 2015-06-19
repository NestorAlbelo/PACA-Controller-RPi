/*
 * MBed.h
 *
 *  Created on: 15/6/2015
 *      Author: Nestor
 */

#ifndef MBED_H_
#define MBED_H_

#include "Librerias/mandoPS3.h"
#include "Librerias/wifi_esp8266.h"
#include "Librerias/watchdog.h"

//CONSTANTES PARA EL CONTROL DEL MBED
#define MBED_PORT "/dev/ttyACM0"
#define BAUD_MBED 115200
#define OK "OK/"
#define CONNECTED "connected/"
#define CONNECT "connect/"
#define SALIR "-1/"
#define FINAL_CADENA "/"
#define PETICION_DATOS_PS3 "DATOS_MANDO_PS3"
#define SEGUNDO 1000
#define WATCHDOG_TIME 5
#define SERIAL_ERROR "ERROR. La conexion serial con el MBed fallo, por favor reinicie el sistema"
#define MBED_ERROR "ERROR. Reinicie el programa del MBed e inicialice este de nuevo"
#define MANDO_NO_ENCONTRADO "Mando PS3 no conectado por favor asegurese de que se encuentra al alcance y con carga"
//FINAL CONSTANTES MBED

//********************METODOS************************
bool initMBed();
void enviarDatosPS3();
void obtenerDatosPS3();
void escucharMBed();
void loop();
//***************************************************

#endif /* MBED_H_ */
