#include "freezeTimer.h"
#include "stoppedState.h"


//******* Static members ************
#define COUNT_PRESETS   5

Preset FreezeTimer::presets[] = {
  {"Custom", 0},    
  {"Kafe", 210},
  {"Chleb1", 600},
  {"Chleb2", 2100},
  {"Pasta", 180}
};

char* formatTime(char *buffer, long milliTime) {
    unsigned int minutes = milliTime / 60000L;
    unsigned int seconds = (milliTime / 1000L) % 60L;
    sprintf(buffer, "%02d:%02d", minutes, seconds);
    return buffer;
}

/************************************/

FreezeTimer::FreezeTimer(const char* name, unsigned long time, unsigned int x, unsigned int y, U8GLIB *display) : 
  xPos(x), yPos(y), timerName(name), initialTime(time), p_u8g(display), isActive(false),
  stoppedState(this), runningState(this), pausedState(this), alarmState(this), minuteSettingState(this), secondSettingState(this)
{
    
    timerState = &stoppedState;
    selPresetIdx = 0;
}

void FreezeTimer::draw() {
  
  //highlight timer if active   
  if(isActive) {
    p_u8g->setColorIndex(1);
    p_u8g->drawBox(xPos, yPos, TIMER_WIDTH, TIMER_HEIGHT);
  }
  
  //let active state do its drawing
  timerState->draw();  
}

Preset* FreezeTimer::getSelectedPreset()
{
    return &FreezeTimer::presets[selPresetIdx];
}

const char* FreezeTimer::getName()
{
  return timerName;
}

void FreezeTimer::setNextPreset()
{
  if(selPresetIdx < COUNT_PRESETS -  1)
    ++selPresetIdx;
  else
    selPresetIdx = 0;
}

void FreezeTimer::setPreviousPreset()
{
  if(selPresetIdx > 0)
    --selPresetIdx;
  else
    selPresetIdx = COUNT_PRESETS - 1;  
}

TimerState* FreezeTimer::getTimerState()
{
    return timerState;
}

void FreezeTimer::setTimerState(TimerState *state)
{
    timerState = state;
}

void FreezeTimer::startTimer()
{
    setTimerState(&runningState);
    remainingMillis = initialTime;
    lastMillis = millis();
    Serial.print("Timer started: ");
}

void FreezeTimer::pauseTimer()
{
    setTimerState(&pausedState);
}

void FreezeTimer::unpauseTimer()
{
    setTimerState(&runningState);
    //reset lastMillis so the time spent on pause is not counted against remaining timer time
    lastMillis = millis();
}

void FreezeTimer::stopTimer()
{
    setTimerState(&stoppedState);
   
}

void FreezeTimer::updateTimer()
{
    unsigned long actualMillis = millis();    
    //timer is updated only if the timer is running
    if(timerState == &runningState) {
        remainingMillis = remainingMillis - (actualMillis - lastMillis);
        //Serial.print("Remaining millis: "); Serial.println(remainingMillis);            
        if(remainingMillis < 0) {
            //Serial.print("Start millis: "); Serial.println(startMillis);            
            //Serial.print("Actual millis: "); Serial.println(actualMillis);
            fireAlarm();
        }
        lastMillis = actualMillis;
    }
    

}

long FreezeTimer::getRemainingMillis()
{
    return remainingMillis;
}

void FreezeTimer::fireAlarm() 
{
    setTimerState( &alarmState );
}

void FreezeTimer::muteAlarm()
{
    //by switching the state to stoppedState the alarm will be muted
    stopTimer();
}

U8GLIB* FreezeTimer::getDisplay()
{
    return p_u8g;
}

unsigned int FreezeTimer::getX() {
    return xPos;
}

unsigned int FreezeTimer::getY() {
    return yPos;
}

void FreezeTimer::setActive(boolean active) {
    isActive = active;
}

boolean FreezeTimer::getActive() {
    return isActive;
}

unsigned long FreezeTimer::getInitialTime() {
    return initialTime;
}

void FreezeTimer::setInitialTime(unsigned long initial) {
    initialTime = initial;
}

char* FreezeTimer::getActualStrTime() {
    unsigned long remainingSeconds = getRemainingMillis() / 1000;
    unsigned int minutes = remainingSeconds / 60;
    unsigned int seconds = remainingSeconds % 60;
    sprintf(strTimeBuffer, "%02d:%02d", minutes, seconds);
    return strTimeBuffer;
}

void FreezeTimer::click() {
    //just pass the click to the active state
    if(timerState != NULL)
        timerState->click();
}

void FreezeTimer::longClick() { 
    //just pass the click to the active state
    if(timerState != NULL)
        timerState->longClick();
}


void FreezeTimer::displayPresetName() {
    const char nullName[] = "No preset";
    const char *presName;
    
    Preset *_pres = getSelectedPreset();
    if(_pres == NULL) {
        presName = nullName;
    } else {
        presName = getSelectedPreset()->name;
    }

    p_u8g->drawStr(getX() + TEXT_X_OFFSET, getY() + TEXT_Y_OFFSET, presName);
}

void FreezeTimer::displayTimerTime(char* strTime) {

    
    //time is justified to the right
    byte h = p_u8g->getFontAscent() - p_u8g->getFontDescent();  
    byte w = p_u8g->getStrWidth(strTime);
    byte pX = getX() + TIME_X_OFFSET;
    byte pY = getY() + TEXT_Y_OFFSET;    
    
    
    //print the remaining time 
    p_u8g->drawStr(pX - w, pY, strTime);    

}
