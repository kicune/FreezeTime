#ifndef MINUTESETTING_STATE_H
#define MINUTESETTING_STATE_H
#include "timerState.h"

class FreezeTimer;

class MinuteSettingState : public TimerState {
    
  public: 
    MinuteSettingState(FreezeTimer* fTimer);
    
    virtual void turnLeft();
    virtual void turnRight();
    virtual void click();
    virtual void longClick();
    virtual void draw();
  
};

#endif

