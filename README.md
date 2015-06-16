#**PACA-Controller-RPi**

En este programa nos encargamos del control de comunicación que existirá entre la Raspberry Pi y el MBed para el proyecto Finca Las Lucanas, en el que se pretende controlar un carro de carga _(PACA)_ mediante un mando PS3 a control remoto o un Joystick manual situado en el propio carro. Para ello mediante la Raspberry controlaremos la conexion serial existente entre el MBed y esta, asi como el Bluetooth del mando y la comunicación Wifi para retransmitir el estado actual de PACA a un móvil desde el que monitorizar todo.

##**Metodos importantes del programa**

**- initMBed():** Inicializamos las conexiones tanto con el MBed como con el modulo Wifi ESP8266.
	
**- loop():** Contiene el groso del programa ya que la Raspeberry permanecera aqui la mayor parte del tiempo en bucle escuchando al MBed y actuando en consecuencia.
	
**- obtenerDatosPS3():** Permite comprobar si existe una conexion con el mando PS3 y obtener los datos de los eventos del mando PS3 si existiesen.
	
**- enviarMensajeWifi(string msg):** Es el encargado de comunicarse con el telefono movil mediante la conexion Wifi para mantenerlo informado.
	
	