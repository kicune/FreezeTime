#include "runningState.h"
#include "freezeTimer.h"


RunningState::RunningState(FreezeTimer* fTimer) {
	freezeTimer = fTimer;
};

void RunningState::turnLeft() {
    //NOP
};


void RunningState::turnRight() {
    //NOP
};

void RunningState::click() {
    freezeTimer->pauseTimer();
};

void RunningState::longClick() {
   freezeTimer->setTimerState(&(freezeTimer->stoppedState));
};

void RunningState::draw() {
    char buffer[20];
    formatTime(buffer, freezeTimer->getRemainingMillis());
    
    U8GLIB *disp = freezeTimer->getDisplay();
    
    //active timer has black text, inactive white text
    disp->setColorIndex(!freezeTimer->getActive());
    
    //show timer time
    freezeTimer->displayPresetName();
    //print the remaining time
    freezeTimer->displayTimerTime(buffer);    

}
