#ifndef PAUSED_STATE_H
#define PAUSED_STATE_H
#include "timerState.h"

class FreezeTimer;

class PausedState : public TimerState {
  public: 
    PausedState(FreezeTimer* fTimer);
    
    virtual void turnLeft();
    virtual void turnRight();
    virtual void click();
    virtual void longClick();
    virtual void draw();
  
};

#endif

