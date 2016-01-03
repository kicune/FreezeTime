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
    virtual void process(); //processes specific state-based actions
    virtual void init(); //things to do when entering the state
};


#endif

