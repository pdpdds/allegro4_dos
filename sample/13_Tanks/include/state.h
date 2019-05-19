/**
 * State machine
 * Allan Legemaate
 * 30/12/2016
 * Compartmentalize program
 */
#ifndef STATE_H
#define STATE_H

//State variables
extern int stateID;
extern int nextState;

// Set next state
extern void set_next_state( int newState );

//Game states
enum programStates{
  STATE_NULL,
  STATE_INIT,
  STATE_INTRO,
  STATE_MENU,
  STATE_GAME,
  STATE_EDIT,
  STATE_EXIT,
};

// State
class state{
  public:
    // Deconstructor
    virtual ~state() {};

    int getStateID(){
      return stateID;
    }

    // Draw to screen
    virtual void draw() = 0;

    // Update logic
    virtual void update() = 0;
};

#endif // STATE_H
