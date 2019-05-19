/**
 * Key Listener
 *   Allan Legemaate
 * Listens for keys JUST pressed or JUST released
 *   since the last tick
 * 18/01/2017
**/

#ifndef KEYLISTENER_H
#define KEYLISTENER_H

//#include <iostream>

class keyListener
{
  public:
    keyListener();
    virtual ~keyListener();

    static bool anyKeyPressed;

    void update();

    static bool keyPressed[127];
    static bool keyReleased[127];

    static int lastKeyPressed;
    static int lastKeyReleased;
  protected:
  private:
    static bool lastTicksKey[127];
};

#endif // KEYLISTENER_H
