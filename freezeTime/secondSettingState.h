#ifndef SECONDSETTING_STATE_H
#define SECONDSETTING_STATE_H
#include "timerState.h"

class FreezeTimer;

class SecondSettingState : public TimerState {
    
  public: 
    SecondSettingState(FreezeTimer* fTimer);
    
    virtual void turnLeft();
    virtual void turnRight();
    virtual void click();
    virtual void longClick();
    virtual void draw();
    virtual void process();
    virtual void init();

   private:
    long inactiveTimer;
  
};

#endif

