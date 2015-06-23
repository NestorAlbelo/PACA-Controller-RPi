/*
 * libraries.h
 *
 *  Created on: 15/6/2015
 *      Author: nestor
 */

#ifndef LIBRARIES_H_
#define LIBRARIES_H_

#include <string>
#include <iostream>
#include <stdio.h>
#include <unistd.h>			// Used for UART
#include <fcntl.h>			// Used for UART
#include <termios.h>		// Used for UART
#include <cstdlib>
#include <sys/time.h>
#include <errno.h>
#include <wiringSerial.h>
#include <linux/watchdog.h>
#include <sys/ioctl.h>
#include <math.h>

using namespace std;

#define ERROR_RETURN false
#define RETURN_OK	 true


#endif /* LIBRARIES_H_ */
