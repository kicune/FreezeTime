#include "alarmState.h"
#include "freezeTimer.h"


AlarmState::AlarmState(FreezeTimer* fTimer) {
	freezeTimer = fTimer;
        show = 0;
        lastBlink = 0;
};

void AlarmState::turnLeft() {
    //NOP
};


void AlarmState::turnRight() {
    //NOP
};

void AlarmState::click() {
    freezeTimer->muteAlarm();
};

void AlarmState::longClick() {
    freezeTimer->muteAlarm();
};

void AlarmState::draw() {        
    U8GLIB *disp = freezeTimer->getDisplay();   
    disp->setColorIndex(!freezeTimer->getActive());
    char strOk[] = "<Ok>";

    //blink the timer
    if(millis() > lastBlink + 500) {
        show = !show;
        lastBlink = millis();
    }    

    if(show) {
        byte pX = freezeTimer->getX() + TIME_X_OFFSET;
        byte pY = freezeTimer->getY() + TEXT_Y_OFFSET;
        byte w = disp->getStrWidth(strOk);
        
        //print the preset name
        freezeTimer->displayPresetName();    
        disp->drawStr(pX - w, pY, strOk);
    }    
}

