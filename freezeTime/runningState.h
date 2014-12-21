#ifndef RUNNING_STATE_H
#define RUNNING_STATE_H
#include "timerState.h"

class FreezeTimer;

class RunningState : public TimerState {
  public: 
    RunningState(FreezeTimer* fTimer);
    
    virtual void turnLeft();
    virtual void turnRight();
    virtual void click();
    virtual void longClick();
    virtual void draw();
 
};

#endif

