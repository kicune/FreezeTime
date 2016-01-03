#include "timerState.h"
#include "freezeTimer.h"

void TimerState::turnLeft() 
{
}

void TimerState::turnRight() 
{
}

void TimerState::click() 
{
}

void TimerState::longClick() 
{
}

void TimerState::draw()
{
    freezeTimer->getDisplay()->drawStr180(freezeTimer->getX(), freezeTimer->getY(), freezeTimer->getName());
}

void TimerState::process() 
{
}

void TimerState::init() 
{
}


