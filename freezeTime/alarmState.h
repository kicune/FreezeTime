#ifndef ALARM_STATE_H
#define ALARM_STATE_H
#include "Arduino.h"
#include "timerState.h"

class FreezeTimer;

class AlarmState : public TimerState {
  private:
    //for blinking
    byte show;
    unsigned long lastBlink;

    
  public: 
    AlarmState(FreezeTimer* fTimer);    
    
    virtual void turnLeft();
    virtual void turnRight();
    virtual void click();
    virtual void longClick();
    virtual void draw();
  
};

#endif

