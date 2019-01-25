#include <U8glib.h>
#include <Encoder.h>
#include <OneButton.h>
//#include <SoftwareSerial.h>
#include "freezeTimer.h"
#include "pitches.h"
#include "Ports.h"

/*
*    Zapojeni:
 *    
 *    Display: VSS:GND, VDD:+5V , RS:10, R/W:11, E:13, PSB:GND, A:+5V, K:GND
 *    Encoder: A:2, B:3, COM:GND, N0:7
 *    Beeper: -:GND, S:12
 *    Prepinac timeru: -:GND, S:8
 *    
 */

#define ENC_PIN_A    2
#define ENC_PIN_B    3
#define ENC_PIN_SWITCH    7
#define BEEP_PIN     6
#define SWITCH_PIN   5

#define SCK_PIN      13
#define LCD_LED_PIN  4

//U8GLIB_ST7920_128X64_1X u8g(SCK_PIN);

//5110 hw SPI: U8GLIB_PCD8544(sck, mosi, cs, a0 [, reset])
//5110 sw SPI: U8GLIB_PCD8544(cs, a0 [, reset])
//led sck mosi d/c(a0) rst sce(c/s) gnd vcc
//U8GLIB_PCD8544 u8g(12, 11, 8, 10, 9);
U8GLIB_PCD8544 u8g(0, 1, 8, 10, 9); 

#define NUM_OF_TIMERS    2
FreezeTimer timers[] = {            //an array of timers
    FreezeTimer("Timer 1", 0, 0, 0, &u8g),
    FreezeTimer("Timer 2", 0, 0, TIMER_HEIGHT, &u8g)
 };


//variables and defines to process encoder clicks
#define ENC_CLICKED  1
#define ENC_PRESSED  2
//#define ENC_PRESS_DELAY    500    // how long must the long press be, in ms.

//how long timer stays on when nothing is happening (in ms.)
#define TIMER_OFF_DELAY    10000

//pause between two alarm beeps, in millis.
#define ALARM_PAUSE    500

byte encoderClicked = 0;
byte risingFalling = 0;  
boolean isDisplayOn = false;

FreezeTimer *activeTimer = NULL;     //a pointer to the timer that currently receives an input
byte activeTimerIdx;

char buf[24];

Encoder encoder(ENC_PIN_A, ENC_PIN_B);
long encPrevValue = 0;
long encValue = 0;

OneButton btnSwitch(SWITCH_PIN, true);      //Switch timer button
OneButton btnEncoder(ENC_PIN_SWITCH, true); //encoder button

//timestamp of the last press
unsigned long lastActivityTimestamp = 0;
//sleeping flag
boolean isSleeping = false;

void draw(void) {
    u8g.setColorIndex(1);
    u8g.drawFrame(0,0,84, 48);
//    u8g.setFont(u8g_font_unifont);  
    u8g.setFont(u8g_font_7x13B);
    u8g.setFontRefHeightExtendedText();
    u8g.setFontPosTop();    
}

//put the FreezeTimer to sleep
void sleep() {
     //turn the lcd light off
    displayOff();
    //put the display to sleep
    u8g.sleepOff();
    //put ATMEL to sleep
    Sleepy::powerDown();    
}

void wakeUp() {
    //ATMEL will wake up itself with an interrupt
    //wake the display
    u8g.sleepOn();
    displayOn();
    
}

void displayOn() {
    digitalWrite(LCD_LED_PIN, 1);
    isDisplayOn = true;
}

void displayOff() {
    digitalWrite(LCD_LED_PIN, 0);
    isDisplayOn = false;
}

void timerSwitch() {
    lastActivityTimestamp = millis();
    //if the lcd backlight is off, turn it on
    if(!isDisplayOn) displayOn();
    
    //deactivate current timer
    activeTimer->setActive(false);
    
    //switch to the next timer
    if(++activeTimerIdx > NUM_OF_TIMERS - 1) {
        activeTimerIdx = 0;
    }   
    
    //activate the next timer
    activeTimer = &timers[activeTimerIdx];
    activeTimer->setActive(true);
}

void encoderClick() {
    //confirm click by beeping
    tone(BEEP_PIN, NOTE_C4, 70);
    //if the lcd backlight is off, turn it on
    if(!isDisplayOn) displayOn();
    
    lastActivityTimestamp = millis();
    
    activeTimer->click();    
}

void encoderPress() {
    //confirm click by long beep
    tone(BEEP_PIN, NOTE_C4, 210);
    lastActivityTimestamp = millis();
    activeTimer->longClick();
}

void soundAlarm() {
    static unsigned long lastAlarmMillis = 0; //timestamp of the last alarm sounding

    //sound the alarm - verify the delay since the last alarm sound and if it's bigger than the ALARM_PAUSE, sound the beeper
    if(millis() - lastAlarmMillis > ALARM_PAUSE) {
        tone(BEEP_PIN, NOTE_C5, 100);
        lastAlarmMillis = millis();
        //delay(100);
    }
}

void initialiseInterrupt() {
    // PCICR |= (1 << PCIE2);
    // PCMSK2 |= (1 << PCINT21);
    // PCMSK2 |= (1 << PCINT23);  
    
  cli();		// switch interrupts off while messing with their settings  
  
  //arduino: 5 atmel: 11 - PCINT21
  //arduino: 7 atmel: 13 - PCINT23
  PCICR =0x04;          // Enable PCINT2interrupt
//  PCMSK1 = 0b00101000;  //Pins 11 and 13 (5 and 7 on arduino)
  PCMSK2 = 0b10100000;  //interrupt pins 21 and 23 (5 and 7 on arduino)
  sei();		// turn interrupts back on
}

//ISR(PCINT2_vect) {    // Interrupt service routine. Every single PCINT8..14 (=ADC0..5) change
            // will generate an interrupt: but this will always be the same interrupt routine
//    tone(BEEP_PIN, NOTE_E4, 100);
//}

void setup(void) {
    //Serial.begin(9600);
    tone(BEEP_PIN, NOTE_C4, 100);
    delay(200);    
    tone(BEEP_PIN, NOTE_C4, 100);    
 
    u8g.setColorIndex(1);
    u8g.setRot180();

    //set LCD_LED_PIN to output
    pinMode(LCD_LED_PIN, OUTPUT);
    displayOn();
    
    activeTimerIdx = 0;
    activeTimer = &timers[activeTimerIdx];
    activeTimer->setActive(true);
    
    lastActivityTimestamp = millis();
    
    //attach handler to timer switch button
    btnSwitch.attachClick(timerSwitch);
    //attach handler to encoder button
    btnEncoder.attachClick(encoderClick);
    //btnEncoder.attachDoubleClick(encoderDoubleClick);
    btnEncoder.attachPress(encoderPress);    
    //FIXME: enable internal pullup resistor on unused pins to save the battery
    //initialiseInterrupt();
}

void loop(void) {
    //process encoder change
    encValue = encoder.read();
    
    //ATMEL is waked up by any interrupt, here we wake up also the display
    if(isSleeping) {
        tone(BEEP_PIN, NOTE_C4, 100);
        wakeUp();
        isSleeping = false;
        lastActivityTimestamp = millis();
    }
    
    int clickDiff = (encValue - encPrevValue) / 2; //encoder changes 2 values per click
    if(clickDiff != 0) { 
        lastActivityTimestamp = millis();
        //if the lcd backlight is off, turn it on
        if(!isDisplayOn) displayOn();
        if(clickDiff > 0) {
            //Serial.println("Turned Left");  
            while(clickDiff > 0) {
                activeTimer->getTimerState()->turnLeft();
                clickDiff--;;
            }
        } 
        else {
            //Serial.println("Turned Right");              
            while(clickDiff < 0 ) {
                activeTimer->getTimerState()->turnRight();
                clickDiff++;            
            }
        }  
        encPrevValue = encValue  + (encValue - encPrevValue) % 2;
    }
        
    //process buttons:
    //timer switch
    btnSwitch.tick();
    //encoder button
    btnEncoder.tick();
    
    //update timers
    boolean isAllStopped = true;
    for(int f = 0; f < NUM_OF_TIMERS; f++) {
        //auto-off: check if all timers are in STOPPED state
        if(timers[f].getTimerState() != &timers[f].stoppedState)
            isAllStopped = false;
            
        timers[f].updateTimer();
    }
    
    //if the last activity happened before TIMER_OFF_DELAY msec...
    if(millis() - lastActivityTimestamp > TIMER_OFF_DELAY) {
        if(isAllStopped) {
            //if all timers are stopped, put timer to sleep 
            sleep();
            isSleeping = true;
        } else {
          //if there are some timers running, just turn off the backlight
          displayOff();
        }
    }

    
    
    //is alarm active for any of the timers? if yes, sound the alarm
    for(int f = 0; f < NUM_OF_TIMERS; f++) {
        if(timers[f].getTimerState() == &timers[f].alarmState) {
            //if the focus is on an inactive alarm, switch to the active one
            if(!timers[f].getActive()) {
              timerSwitch();
            }
            soundAlarm();
            //and skip the rest - multiple beeps causes problems
            break;
        }
    }

    //process state-specific actions:
    for(int f = 0; f < NUM_OF_TIMERS; f++) {
      timers[f].getTimerState()->process();
    }

    // picture loop
    u8g.firstPage();  
    do {
        draw();
        for(int f = 0; f < NUM_OF_TIMERS; f++) {
            timers[f].draw();
        }
    } 
    while(u8g.nextPage());

}




