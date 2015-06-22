#ifndef __WATCHDOG_HH__
#define __WATCHDOG_HH__

#include "libraries.h"
#include "wifi_esp8266.h"


#define RUTA_WATCHDOG "/dev/watchdog"
#define SERIAL_ERROR_WATCHDOG "ERROR. Conexion con el Watchdog no establecida, intentelo de nuevo"

bool initWatchDog();
bool initWatchDog(int seconds);
void setWaitTimeWatchDog(int seconds);
void keepAliveWatchDog();
void disableWatchDog();
void cerrarConexionWatchDog();


#endif
