#include "secondSettingState.h"
#include "freezeTimer.h"


SecondSettingState::SecondSettingState(FreezeTimer* fTimer) {
	freezeTimer = fTimer;
};

void SecondSettingState::turnLeft() {
    //fetch actual timer setting
    long iTime = freezeTimer->getInitialTime();
    //substract 10 seconds (10s * 1000):
    iTime -= 10000L;
    
    //ignore clicks that would put us below 0
    if(iTime > 0) {
        freezeTimer->setInitialTime(iTime);
    }

    //reset inactive timer
    inactiveTimer = millis();
};


void SecondSettingState::turnRight() {
    //FIXME: omezit to jenom na vteriny?
    //fetch actual timer setting
    long iTime = freezeTimer->getInitialTime();
    //add 1 minute (60s * 1000):
    iTime += 10000L;

    //ignore clicks that would put us over 999 minutes
    if(iTime < 59940000L) {
        freezeTimer->setInitialTime(iTime);
    }

    //reset inactive timer
    inactiveTimer = millis();
};

void SecondSettingState::click() {
    freezeTimer->startTimer();
};

void SecondSettingState::longClick() {
   //reset - go back to preset selection
   freezeTimer->setTimerState(&(freezeTimer->stoppedState));
};

void SecondSettingState::draw() {
    char buffer[20];
    char buffer2[3];
    
    formatTime(buffer, freezeTimer->getInitialTime());
    
    U8GLIB *disp = freezeTimer->getDisplay();
    
    //active timer has black text (2x highlighted), inactive white text
    byte color = freezeTimer->getActive();
    disp->setColorIndex(!color);

    //Print preset name
    freezeTimer->displayPresetName();
    
    //find second box size (two last chars)
    byte t_len = strlen(buffer);
    buffer2[0] = buffer[t_len - 2]; buffer2[1]=buffer[t_len - 1]; buffer2[2] = 0;
    byte h = disp->getFontAscent() - disp->getFontDescent();  
    byte w = disp->getStrWidth(buffer);
    byte wSeconds = disp->getStrWidth(buffer2);    
    byte pX = freezeTimer->getX() + TIME_X_OFFSET;
    byte pY = freezeTimer->getY() + TEXT_Y_OFFSET;
    
    //print a whole time string first...    
    disp->drawStr(pX - w, pY, buffer);
    
    //...then print a box around seconds... //FIXME: tady je podvod - spoleham na to, ze minuty jsou stejne siroke jako vteriny!
    disp->drawBox(pX - wSeconds, pY - 1, wSeconds + 1, h + 2) ;
    
    //...and then print only seconds in a reverted color 
    disp->setColorIndex(color);
    disp->drawStr(pX - wSeconds + 1, pY, buffer2);
};

void SecondSettingState::process() {
    if(millis() - inactiveTimer > MANUAL_SETTING_TIMEOUT) {
      //start the timer automaticaly
      freezeTimer->startTimer();
    }
};

void SecondSettingState::init() {
      //resets the timer
      inactiveTimer = millis();
};


