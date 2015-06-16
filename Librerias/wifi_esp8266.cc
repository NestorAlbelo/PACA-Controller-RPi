#include "wifi_esp8266.h"

//*****************VARIABLES*****************//
int fd_Wifi = -1;			//Contiene el ID de la conexion del Wifi

//******************METODOS******************//

//----------------------------------------------------------------------------------------
/**
 *  Inicializamos la conexion con el Wifi ESP8266
 */
//----------------------------------------------------------------------------------------
bool initWifi(){
	//Abrimos la conexion con el Wifi en modo Lectura/Escritura y
	//comprobamos que la conexion este bien establecida
	if ((fd_Wifi = open(RUTA_WIFI, O_RDWR | O_NOCTTY | O_NDELAY)) == -1){
		cout << "ERROR. La conexion Serial con el Wifi ESP8266 ha fallado, asegurese de que no este siendo utilizada por otro programa" << endl;
		return false;
	}

	//Configuramos la conexion UART
	struct termios options;
	tcgetattr(fd_Wifi, &options);
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//Establecemos el BAUD a 9600
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(fd_Wifi, TCIFLUSH);
	tcsetattr(fd_Wifi, TCSANOW, &options);

	cout << "Conexion con el Wifi ESP8266 establecida con exito" << endl;
	return true;
}//Final metodo initWifi();


//----------------------------------------------------------------------------------------
/**
 *  Enviamos un mensaje al Wifi
 *
 *  @param msg Contiene el mensaje a enviar
 */
//----------------------------------------------------------------------------------------
void enviarMensajeWifi(string msg){
	int length = msg.length();
	esperar(0.1);
	//Inicializamos el mensaje a enviar
	//----- TX BYTES -----
	unsigned char tx_buffer[length+2];
	unsigned char *p_tx_buffer;
	p_tx_buffer = &tx_buffer[0];

	//Recorremos el mensaje y lo copiamos en el buffer a enviar
	for(int i=0; i < length; i++)
		*p_tx_buffer++ = msg[i];

	//Concatenamos el \r\n
	*p_tx_buffer++ = 13; 	// 	\r
	*p_tx_buffer++ = 10;	//	\n
	//Final inicializacion mensaje

	//Comprobamos que la conexion este abierta y si es asi enviamos el mensaje
	if (fd_Wifi != -1){
		if ((write(fd_Wifi, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]))) < 0)
			cout << "ERROR. No se ha podido enviar el mensaje correctamente" << endl;
	}
	else{
		cout << "ERROR.No se puede enviar ningun mensaje porque la conexion no esta establecida, por favor abra una conexion con el modulo Wifi ESP8266" << endl;
	}
	p_tx_buffer = NULL;
	delete p_tx_buffer;
}//Final metodo enviarMensajeWifi(string mensaje)


//----------------------------------------------------------------------------------------
/**
 *  Comprobamos que exista un mensaje que leer y si es asi lo mostramos por pantalla
 *
 *  @param delay Establece el tiempo en segundos a esperar para recibir el mensaje
 */
//----------------------------------------------------------------------------------------
string recibirMensajeWifi(double delay){
	esperar(delay);					//Esperamos un tiempo determinado
	//Comprobamos que este la conexion abierta
	if (fd_Wifi != -1){
		// Read up to 255 characters from the port if they are there
		unsigned char rx_buffer[256];
		int rx_length = read(fd_Wifi, (void*)rx_buffer, 255);		//Filestream, buffer to store in, number of bytes to read (max)
		if (rx_length < 0){
			//Ocurre un error si no existen bytes de lectura
			cout << "ERROR.No existen bytes de lectura" << endl;
			return string("No existen bytes de lectura");
		}
		else if (rx_length == 0){
			//No hay datos que leer (su longitud es 0)
			return string("No hay datos que leer");
		}
		else{
			//Recibimos el mensaje y lo mostramos
			rx_buffer[rx_length] = '\0';							//Asignamos NULL al ultimo elemento de la cadena
			cout << rx_length << " bytes leido: " << rx_buffer << endl;

			//Devolvemos el mensaje recogido
			return (string ((char*) rx_buffer));
		}
	}
	else{
		cout << "ERROR.No se puede recibir ningun mensaje porque no existe conexion con el modulo Wifi ESP8266" << endl;
		return string("Error");
	}
}//Final metodo recibirMensajeWifi(double delay)


//----------------------------------------------------------------------------------------
/**
 *  Cerramos la conexion serial con el modulo Wifi ESP8266
 */
//----------------------------------------------------------------------------------------
void cerrarSerialWifi(){
	close(fd_Wifi);
	cout << "Conexion Serial con Wifi cerrada correctamente" << endl;
}//Final metodo cerrarSerialWifi()


//----------------------------------------------------------------------------------------
/**
 *  No hacemos nada durante un periodo de tiempo, solo esperar
 *
 *  @param segundos Establece el tiempo en segundos a esperar
 */
//----------------------------------------------------------------------------------------
void esperar(double segundos){
	timeval time;
	gettimeofday(&time,NULL);
	long millis = (time.tv_sec*1000) + (time.tv_usec / 1000);
	long total = millis + (segundos*1000);
	while(millis < total){
		gettimeofday(&time,NULL);
		millis = (time.tv_sec*1000) + (time.tv_usec / 1000);
	}
}//Final metodo esperar(double segundos)
