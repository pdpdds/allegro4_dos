/**
 * Mouse Listener
 * Allan Legemaate
 * Listens for mouse buttons JUST pressed or JUST released
 *   since the last tick
 * 18/01/2017
**/

//#include <iostream>

#ifndef MOUSELISTENER_H
#define MOUSELISTENER_H

#define MAX_MOUSE_BUTTONS 3

class mouseListener{
  public:
    mouseListener();
    virtual ~mouseListener();

    void update();

    static unsigned char mouse_button;
    static unsigned char mouse_pressed;
    static unsigned char mouse_released;
  protected:

  private:
    static unsigned char mouse_old;
};

#endif // MOUSELISTENER_H
