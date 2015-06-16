#ifndef __WIFI_ESP8266_H__
#define __WIFI_ESP8266_H__

#include "libraries.h"

#define RUTA_WIFI "/dev/ttyAMA0"

//********************METODOS************************
bool initWifi();
void enviarMensajeWifi(string msg);
string recibirMensajeWifi(double delay);
void cerrarSerialWifi();
void esperar(double seg);
//***************************************************

#endif /* __WIFI_ESP8266_H__ */
