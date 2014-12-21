#include "pausedState.h"
#include "freezeTimer.h"


PausedState::PausedState(FreezeTimer* fTimer) {
	freezeTimer = fTimer;
};

void PausedState::turnLeft() {
    //NOP
};


void PausedState::turnRight() {
    //NOP
};

void PausedState::click() {
    freezeTimer->unpauseTimer();
};

void PausedState::longClick() {
    freezeTimer->stopTimer();
};

void PausedState::draw() {
    char buffer[20];
    formatTime(buffer, freezeTimer->getRemainingMillis());
    
    U8GLIB *disp = freezeTimer->getDisplay();
    
    //active timer has black text, inactive white text
    disp->setColorIndex(!freezeTimer->getActive());
    
    disp->drawStr(freezeTimer->getX() + TEXT_X_OFFSET -2, freezeTimer->getY() + TEXT_Y_OFFSET, "|Pause|");
    
    //time is justified to the right
    byte h = disp->getFontAscent() - disp->getFontDescent();  
    byte w = disp->getStrWidth(buffer);
    byte pX = freezeTimer->getX() + TIME_X_OFFSET;
    byte pY = freezeTimer->getY() + TEXT_Y_OFFSET;
    
    //print the remaining time 
    disp->drawStr(pX - w, pY, buffer);    
}

