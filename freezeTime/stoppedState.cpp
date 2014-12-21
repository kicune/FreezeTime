#include "stoppedState.h"
#include "freezeTimer.h"


StoppedState::StoppedState(FreezeTimer* fTimer) {
	freezeTimer = fTimer;
};

void StoppedState::turnLeft() {
  freezeTimer->setPreviousPreset();
};


void StoppedState::turnRight() {
  freezeTimer->setNextPreset();
};

void StoppedState::click() {
    //if a custom time interval is selected, then go to the period setting
    if(freezeTimer->getSelectedPreset()->time == 0) {
        freezeTimer->setTimerState(&(freezeTimer->minuteSettingState));
    } else {
    //if a normal preset is selected, set its time into the actual timer's time:
        freezeTimer->setInitialTime(freezeTimer->getSelectedPreset()->time*1000L);
        
        //and then start the timer
        freezeTimer->setTimerState( &(freezeTimer->runningState) );
        freezeTimer->startTimer();
    }
};

void StoppedState::longClick() {
    //NOP
};

void StoppedState::draw()
{   
    U8GLIB *disp = freezeTimer->getDisplay();
    char buffer[20];
    
    formatTime(buffer, freezeTimer->getSelectedPreset()->time*1000L);
    
    //active timer has black text, inactive white text
    disp->setColorIndex(!freezeTimer->getActive());
    freezeTimer->displayPresetName();
    freezeTimer->displayTimerTime(buffer);
}

