#ifndef TIMER_STATE_H
#define TIMER_STATE_H

class FreezeTimer;

class TimerState {
  protected: 
    FreezeTimer *freezeTimer;
  
  public: 
    virtual void turnLeft();
    virtual void turnRight();
    virtual void click();
    virtual void longClick();
    virtual void draw();
};


#endif

