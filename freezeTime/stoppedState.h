#ifndef STOPPED_STATE_H
#define STOPPED_STATE_H
#include "timerState.h"

class FreezeTimer;

class StoppedState : public TimerState {
  public: 
    StoppedState(FreezeTimer* fTimer);
    
    virtual void turnLeft();
    virtual void turnRight();
    virtual void click();
    virtual void longClick();
    virtual void draw();
  
};

#endif

