// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>

#ifndef __JOYSTICK_HH__
#define __JOYSTICK_HH__

#include "libraries.h"

#define JS_EVENT_BUTTON 0x01 // button pressed/released
#define JS_EVENT_AXIS   0x02 // joystick moved
#define JS_EVENT_INIT   0x80 // initial state of device

#define RUTA_PS3 "/dev/input/js0"
#define CENTRO_EJES 0
#define NORMALIZADOR 32767

enum BUTTON : unsigned char{EQUIS=14,CUADRADO=15,CIRCULO=13,TRIANGULO=12,UP=4,DOWN=6,LEFT=7,RIGH=5,L1=10,L2=8,L3=1,R1=11,R2=9,R3=2,START=3,SELECT=0};
enum ESTADOS_BOTONES {PULSADO=1, NO_PULSADO = 0};
enum AXIS : unsigned char{PADL_X=1,PADL_Y=0,PADR_X=3,PADR_Y=2};

/**
 * Encapsulates all data relevant to a sampled joystick event.
 */
class JoystickEvent
{
public:
  /**
   * The timestamp of the event, in milliseconds.
   */
  unsigned int time;
  
  /**
   * The value associated with this joystick event.
   * For buttons this will be either 1 (down) or 0 (up).
   * For axes, this will range between -32768 and 32767.
   */
  short value;
  
  /**
   * The event type.
   */
  unsigned char type;
  
  /**
   * The axis/button number.
   */
  unsigned char number;

  /**
   * Returns true if this event is the result of a button press.
   */
  bool isButton()
  {
    return (type & JS_EVENT_BUTTON) != 0;
  }

  /**
   * Returns true if this event is the result of an axis movement.
   */
  bool isAxis()
  {
    return (type & JS_EVENT_AXIS) != 0;
  }

  /**
   * Returns true if this event is part of the initial state obtained when
   * the joystick is first connected to.
   */
  bool isInitialState()
  {
    return (type & JS_EVENT_INIT) != 0;
  }
};

/**
 * Represents a joystick device. Allows data to be sampled from it.
 */
class Joystick
{
private:
  void openPath(std::string devicePath);
  
  int _fd;
  
public:
  ~Joystick();

  /**
   * Initialises an instance for the first joystick: /dev/input/js0
   */
  Joystick();

  /**
   * Initialises an instance for the joystick with the specified,
   * zero-indexed number.
   */
  Joystick(int joystickNumber);

  /**
   * Initialises an instance for the joystick device specified.
   */
  Joystick(std::string devicePath);
 
  /**
   * Returns true if the joystick was found and may be used, otherwise false.
   */
  bool isFound();
  
  /**
   * Attempts to populate the provided JoystickEvent instance with data
   * from the joystick. Returns true if data is available, otherwise false.
   */
  bool sample(JoystickEvent* event);
};

#endif
