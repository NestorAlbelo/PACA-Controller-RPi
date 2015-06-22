#include "watchdog.h"


//*****************VARIABLES*****************//
int fd_watchdog = -1;							//Contiene el ID del watchdog
//*******************************************//


//******************METODOS******************//

//----------------------------------------------------------------------------------------
/**
 *	Abrimos la conexion con el WatchDog para poder usarlo
 */
//----------------------------------------------------------------------------------------
bool initWatchDog(){
	if ((fd_watchdog = open(RUTA_WATCHDOG, O_RDWR | O_NOCTTY)) < 0) {
		enviarMensajeWifi(SERIAL_ERROR_WATCHDOG);
		cerrarConexionWatchDog();
		return ERROR_RETURN;
	}

	cout << "WatchDog inicializado correctamente" << endl;
	return RETURN_OK;
}//Final metodo initWatchDog()

//----------------------------------------------------------------------------------------
/**
 *	Abrimos la conexion con el WatchDog para poder usarlo
 *	y establecemos el tiempo de espera
 */
//----------------------------------------------------------------------------------------
bool initWatchDog(int seconds){
	if ((fd_watchdog = open(RUTA_WATCHDOG, O_RDWR | O_NOCTTY)) < 0) {
		enviarMensajeWifi(SERIAL_ERROR_WATCHDOG);
		cerrarConexionWatchDog();
		return ERROR_RETURN;
	}

	//Establecemos el tiempo de espera pasado por parametro
	setWaitTimeWatchDog(seconds);

	//Mandamos retroalimentacion al WatchDog
	keepAliveWatchDog();

	cout << "Conexion con WatchDog inicializada correctamente con T: " << seconds << "sg" << endl;
	return RETURN_OK;

}//Final metodo initWatchDog(int seconds)


//----------------------------------------------------------------------------------------
/**
 *	Establecemos el tiempo de espera hasta que el
 *	WatchDog reinicie el sistema
 */
//----------------------------------------------------------------------------------------
void setWaitTimeWatchDog(int seconds){
	ioctl(fd_watchdog, WDIOC_SETTIMEOUT, &seconds);
	ioctl(fd_watchdog, WDIOC_GETTIMEOUT, &seconds);
}//Final metodo setWaitTimeWatchDog(int seconds)


//----------------------------------------------------------------------------------------
/**
 *	Enviamos retroalimentacion al WatchDog para que
 *	no reinicie el sistema
 */
//----------------------------------------------------------------------------------------
void keepAliveWatchDog(){
	ioctl(fd_watchdog, WDIOC_KEEPALIVE, 0);
}//Final metodo keepAliveWatchDog()


//----------------------------------------------------------------------------------------
/**
 *	Deshabilitamos el contador del Watchdog para que no
 *	apague el sistema a partir de ahora
 */
//----------------------------------------------------------------------------------------
void disableWatchDog(){
	write(fd_watchdog, "V", 1);
	cout << "Watchdog deshabilitado correctamente" << endl;
}//Final metodo disableWatchDog()


//----------------------------------------------------------------------------------------
/**
 *	Cerramos la conexion con el WatchDog de forma segura
 *	y deshabilitamos el contador
 */
//----------------------------------------------------------------------------------------
void cerrarConexionWatchDog(){
	disableWatchDog();
	close(fd_watchdog);
	cout << "Conexion con WatchDog cerrada correctamente" << endl;
}//Final metodo cerrarConexionWatchDog()

