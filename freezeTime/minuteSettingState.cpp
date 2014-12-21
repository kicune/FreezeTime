#include "minuteSettingState.h"
#include "freezeTimer.h"


MinuteSettingState::MinuteSettingState(FreezeTimer* fTimer) {
	freezeTimer = fTimer;
};

void MinuteSettingState::turnLeft() {
    Serial.println("Substracting 60 seconds");
    //fetch actual timer setting
    long iTime = freezeTimer->getInitialTime();
    //substract 1 minute (60s * 1000):
    iTime -= 60000L;
    
    //ignore clicks that would put us below 0
    if(iTime >= 0) {
        freezeTimer->setInitialTime(iTime);
    }
};


void MinuteSettingState::turnRight() {
    Serial.println("Adding 60 seconds");    
    //fetch actual timer setting
    long iTime = freezeTimer->getInitialTime();
    //add 1 minute (60s * 1000):
    iTime += 60000L;
    
    //ignore clicks that would put us over 999 minutes
    if(iTime < 59940000L) {
        freezeTimer->setInitialTime(iTime);
    }
    
};

void MinuteSettingState::click() {
   freezeTimer->setTimerState(&(freezeTimer->secondSettingState));
};

void MinuteSettingState::longClick() {
   //reset - go back to preset selection
   freezeTimer->setTimerState(&(freezeTimer->stoppedState));
};

void MinuteSettingState::draw() {
    char buffer[20];
    char buffer2[4];
    unsigned long iniTime;

    
    U8GLIB *disp = freezeTimer->getDisplay();
    
    //active timer has black text (2x highlighted), inactive white text
    byte color = freezeTimer->getActive();
    disp->setColorIndex(!color);

    //Print preset name
    freezeTimer->displayPresetName();

    //find minute box size (first two or 3 chars)
    iniTime = freezeTimer->getInitialTime();
    
    formatTime(buffer, iniTime);
    //more than 99 minutes -> 3 chars, otherwise 2 chars
    buffer2[0] = buffer[0]; buffer2[1]=buffer[1]; 
    if(iniTime / 60000 > 99) {
        buffer2[2] = buffer[2]; buffer2[3] = 0;
    } else {
        buffer2[2] = 0;
    }
        
    byte h = disp->getFontAscent() - disp->getFontDescent();  
    byte w = disp->getStrWidth(buffer);
    byte wMinutes = disp->getStrWidth(buffer2);
    byte pX = freezeTimer->getX() + TIME_X_OFFSET;
    byte pY = freezeTimer->getY() + TEXT_Y_OFFSET;

    //print a whole time string first...    
    disp->drawStr(pX - w, pY, buffer);
    
    //...then print a box around minutes...
    disp->drawBox(pX - w - 1, pY - 1, wMinutes + 1, h + 2) ;
    
    //...and then print only minutes in a reverted color
    disp->setColorIndex(color);    
    disp->drawStr(pX - w, pY, buffer2);
}

