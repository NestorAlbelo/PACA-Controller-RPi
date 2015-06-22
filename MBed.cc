/*
 * MBed.cc
 *
 *  Created on: 15/6/2015
 *      Author: Nestor
 */

#include "MBed.h"

//*******************VARIABLES*********************
string datosAEnviar;					//Contiene el mensaje a enviar al MBED
string caracter;                    	//Un solo caracter del mensaje
string cadena;                      	//Cadena completa o en progreso del mensaje
string mensajeRecibido;					//Es el mensaje que nos ha enviado el MBed
bool salir = false;

int fd_MBed = -1;						//Contiene el ID de la conexion serial del MBed

Joystick joystick(RUTA_PS3);			//Referencia al mando PS3
double ejeX,ejeY;						//Valores de los ejes izquierdos X e Y
int buttonCtrl;							//Valor del boton de control

//*************VECTORES DE MENSAJES****************
string s_modos[] = {"Modo PS3. ", "Modo JOYSTICK. ", "Modo AUTO. ", "Modo OFF. "};
string s_logs[]  = {"Todo esta OK.", "Boton de Control NO pulsado.", "Boton de Control pulsado."};
//*************************************************


//----------------------------------------------------------------------------------------
/**
 *  Inicializamos las conexiones seriales con el MBED.
 *  Si existe algun error lo notificamos, sino
 *  mostramos un mensaje de confirmacion
 *
 *  @return bool Verdadero si to-do ha salido bien y falso si ha habido algun error
 */
//----------------------------------------------------------------------------------------
bool initMBed(){
	//Inicializamos las variables a cero y no pulsadas
	ejeX = ejeY = CENTRO_EJES;
	buttonCtrl = NO_PULSADO;

	//Abrimos la conexion serial con el MBED y comprobamos si hay algun error
	if((fd_MBed = serialOpen(MBED_PORT, BAUD_MBED)) < 0){
		cout << SERIAL_ERROR << endl;
		serialClose(fd_MBed);                        //Cerramos la conexion serial
		enviarMensajeWifi(SERIAL_ERROR);
		exit(-1);
	}

	//Comprobacion de la conexion serial
	serialPuts(fd_MBed, CONNECT);                //Enviamos el mensaje para confirmar la conexion

	//Esperamos la respuesta del MBed
	while(cadena.length() == 0 || caracter != FINAL_CADENA){
		//Leemos la respuesta del MBed
		while(serialDataAvail(fd_MBed)){
			caracter = serialGetchar(fd_MBed);
			cadena += caracter;
			fflush(stdout);
		}
	}

	//Si el mensaje contiene la palabra de conexion le devolvemos el mensaje para confirmarla
	if (cadena.find(CONNECTED) != string::npos) {
		serialPuts(fd_MBed, OK);		//Le enviamos el OK para el comienzo de la comunicacion
	}
	//Si no enviamos un mensaje de error y cerramos la conexion serial
	else{
		cout << MBED_ERROR << endl;
		serialClose(fd_MBed);                        //Cerramos la conexion serial
		enviarMensajeWifi(MBED_ERROR);
		return ERROR_RETURN;
	}
	cadena="";

	//Inicializamos el Wifi ESP8266
	while(!initWifi());

	//Inicializamos el WatchDog
	//while(!initWatchDog(WATCHDOG_TIME));

	//Si todo ha salido bien mostramos el siguiente mensaje
	cout << "La conexion serial con el MBed se establecio correctamente" << endl;
	return RETURN_OK;
}//Final metodo initMBed()

//----------------------------------------------------------------------------------------
/**
 *  Obtenemos los datos de los eventos del mando PS3
 */
//----------------------------------------------------------------------------------------
void obtenerDatosPS3(){
	//Si el mando esta conectado
	if(joystick.isFound()){
		JoystickEvent event;            //Creamos una instancia para un evento del joystick

		//Si existe un evento con el mando
		if (joystick.sample(&event)){
			//Si es un BOTON
			if(event.isButton()){
				//Si es el boton de salida
				if(event.number == START){
					if(event.value == PULSADO)
						salir = true;
				}
				//Si es el boton de control
				else if(event.number == L2){
					if(event.value == PULSADO || event.value == NO_PULSADO)
						buttonCtrl = event.value;				
				}
			}
			//Si es un AXIS
			else if(event.isAxis()){
				//Si movemos el Axis X
				if(event.number == PADL_X){
					ejeX = ((double)(event.value))/NORMALIZADOR;
				}
				//Si movemos el Axis Y
				else if(event.number == PADL_Y){
					ejeY = ((double)(event.value))/NORMALIZADOR;
				}
			}
		}//Final if (joystick.sample(&event))
	}
	//Si el mando no esta conectado
	else{
		//Colocamos los parametros por defecto
		ejeX = ejeY = 0;
		buttonCtrl = NO_PULSADO;
		cout << MANDO_NO_ENCONTRADO << endl;
		enviarMensajeWifi(MANDO_NO_ENCONTRADO);
	}
}//Final metodo obtenerDatosPS3()


//----------------------------------------------------------------------------------------
/**
 *  Formamos la cadena a enviar y se la pasamos al MBED con los datos del mando
 */
//----------------------------------------------------------------------------------------
void enviarDatosPS3(){
	obtenerDatosPS3();				//Comprobamos los eventos del mando

	//Formamos la cadena a enviar
	datosAEnviar = to_string(ejeX) + "," + to_string(ejeY) + "," + to_string(buttonCtrl) + "/";

	cout << "DATOS A ENVIAR: " << datosAEnviar << endl;				//DEJAR SOLO EN TEST!!!
	//Enviamos la cadena formada
	serialPuts(fd_MBed, datosAEnviar.c_str());
	cout << "Enviamos los datos" << endl;							//DEJAR SOLO EN TEST!!!
}//Final metodo enviarDatosPS3()



//----------------------------------------------------------------------------------------
/**
 * Escuchamos lo que nos diga el MBed hasta que nos envie el final de cadena
 */
//----------------------------------------------------------------------------------------
void escucharMBed(){
	mensajeRecibido = "";			//Limpiamos el mensaje
	bool final = false;

	//Mientras no hayamos recogido el final de cadena
	while(!final){
		//cout << "estoy dentro del !final" << endl;
		//esperar(1);
		if(serialDataAvail(fd_MBed)){
			caracter = serialGetchar(fd_MBed);
			if(caracter != FINAL_CADENA){		//Si no es el final de la cadena guardamos el valor
				mensajeRecibido += caracter;
				fflush(stdout);
			}
			else{								//Si es el final de la cadena indicamos el fin
				final = true;
				fflush(stdout);
				//break;
			}
		}
	}

} //Final metodo escucharMBed()



//----------------------------------------------------------------------------------------
/**
 *  Nos mantenemos en un bucle recibiendo lo que nos diga el MBed
 */
//----------------------------------------------------------------------------------------
void loop(){
	int mensaje;
	int modo, log;
	while (!salir){
		cout << "\nVamos a Escuchar al MBED" << endl;
		escucharMBed();	//Modos enviado por los selectores del MBed

		cout << "Recibimos: " << mensajeRecibido << endl;			//DEJAR SOLO EN TEST!!!

		//Decodificamos el mensaje
		mensaje = atoi(mensajeRecibido.c_str());	//Ej: 12 -> 1 - Numero Modo / 2 - Numero Log

		modo = mensaje / 10;						//Obtenemos la parte SUPERIOR del mensaje
		log  = abs(mensaje) % 10;					//Obtenemos la parte INFERIOR del mensaje

		//Si el modo es igual al de PS3 mandamos los datos
		if(modo == M_PS3){
			cout << "Nos piden datos del mando" << endl;			//DEJAR SOLO EN TEST!!!
			enviarDatosPS3();
		}

		//Enviamos los datos al modulo Wifi
		cout << "Enviamos mensaje al Wifi" << endl;				//DEJAR SOLO EN TEST!!!
		enviarMensajeWifi((s_modos[modo]+s_logs[log]));

		/*
		//Si nos estan pidiendo los datos del mando
		if(mensajeRecibido.compare(PETICION_DATOS_PS3) == 0){
			cout << "Nos piden datos del mando" << endl;			//DEJAR SOLO EN TEST!!!
			enviarDatosPS3();
		}
		//Si no enviamos el modo y los logs al wifi
		else{
			cout << "Enviamos mensaje al Wifi" << endl;				//DEJAR SOLO EN TEST!!!
			enviarMensajeWifi(mensajeRecibido);
		}
		*/
		cout << "Estoy al final del bucle loop" << endl; 
	}//Final while

	//cerrarConexionWatchDog();				 //Cerramos la conexion con el Watchdog
	serialClose(fd_MBed);                    //Cerramos la conexion serial
}//Final metodo loop()
