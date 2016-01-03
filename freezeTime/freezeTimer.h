#ifndef FREEZE_TIMER_H
#define FREEZE_TIMER_H
#include "Arduino.h"
#include <U8glib.h>
#include "stoppedState.h"
#include "runningState.h"
#include "pausedState.h"
#include "alarmState.h"
#include "minuteSettingState.h"
#include "secondSettingState.h"
#include "pitches.h"

typedef struct Preset {
  const char* name;
  unsigned int time;
} Preset;

#define TIMER_WIDTH    84
#define TIMER_HEIGHT    24
#define TIME_X_OFFSET   83    //X position where time is displayed, justified to the right

#define TEXT_X_OFFSET   2
#define TEXT_Y_OFFSET   6

#define MANUAL_SETTING_TIMEOUT  15000 //number of miliseconds of inactivity in minute/secondSettingState after which the timer starts automaticaly 

char* formatTime(char *buffer, long milliTime);

class TimerState;

class FreezeTimer {
public:
    static Preset presets[];
    StoppedState stoppedState;
    RunningState runningState;
    PausedState pausedState;
    AlarmState alarmState;
    MinuteSettingState minuteSettingState;
    SecondSettingState secondSettingState;

  
private:
  const char *timerName;   //Name of the timer
  boolean isActive; //TRUE if the timer is an active timer
  unsigned int xPos;  //x-position of the timer display
  unsigned int yPos;   //y-position of the timer display
  unsigned long initialTime;  //time length of the timer in millis (to how many seconds*1000 the timer was originaly set)
  unsigned long lastMillis; //timestamp of the last timer update
  long remainingMillis;
  int selPresetIdx; //idx of a selected preset;
  TimerState *timerState;    //actual state of the timer
  char strTimeBuffer[20]; //buffer for string representation of remaining time
  
  U8GLIB *p_u8g; //pointer to the display handler class


public:   
  FreezeTimer(const char* name, unsigned long time, unsigned int x, unsigned int y, U8GLIB *p_display);  //Constructor
  void draw();                   //redraw the timer on the screen
  const char* getName();
  Preset* getSelectedPreset();
  void setNextPreset();
  void setPreviousPreset();
  TimerState* getTimerState();
  void setTimerState(TimerState *state);
  void startTimer();    //Duh!
  void pauseTimer();    //Duh! #2
  void unpauseTimer();    //Duh! #3
  void stopTimer();       //Duh! #4
  void updateTimer();  //updates the remaining time based on the time spent running
  void fireAlarm();    //honk the alarm
  void muteAlarm();    //unhonk the alarm
  U8GLIB *getDisplay(); //retrieve display handle
  unsigned int getX();
  unsigned int getY();
  void setActive(boolean active); //set / unset active flag
  boolean getActive(); //get active flag
  void click(); //click handler
  void longClick(); //long click handler
  long getRemainingMillis(); 
  void setInitialTime(unsigned long actual);
  unsigned long getInitialTime();
  char* getActualStrTime();
  void displayPresetName();
  void displayTimerTime(char* strTime);
  
  //draw functions
  void highlight();
};

#endif


