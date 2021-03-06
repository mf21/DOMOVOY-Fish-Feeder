/*
  Fish Feeder DOMOVOY v.4.0 firmware
  Specs: DIP board, uln2003, ds1307, HC-05, battery, analog keys
  rev. 4.0.2

  The Fish Feeder is part of the DOMOVOY project which provides a smart home technology.
  More information about the project can be found here: http://www.mf21.ru

  created 2017
  by KWL
*/

/*
  Memory mapping
  00 01 - Alarm 1 (hour, min)
  02 03 - Alarm 2 (hour, min)
  04 05 - Alarm 3 (hour, min)
  06 07 - Alarm 4 (hour, min)
  08    - Retraction (on, off)
  10    - motorNumTurns
*/
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <MD_DS1307.h>
#include <SoftwareSerial.h>

//MOTOR section of ULN2003A chip
const byte motorSteps          = 16; // 8 is older value
const int  motorSpeed          = 1024;
const int  motorTurn           = 2048;
const byte motorNumTurnsMax    = 100;
const byte ROMMotorOffset      = 10; // offset in EPROM
const byte ROMRetractionOffset = 8;

const byte motorPin1 = 13;
const byte motorPin2 = 12;
const byte motorPin3 = 11;
const byte motorPin4 = 10;

const int motorRetractionBefore = 1; // number of turnover
const int motorRetractionAfter  = 1; // number of turnover

int       motorNumTurns      = 5; 


Stepper motor(motorSteps, motorPin1, motorPin3, motorPin2, motorPin4); // 6 7 8 9

//KEYS section
const int keyPort = A0;
int keysBTN1State = LOW;
int keysBTN2State = LOW;
int keysBTN3State = LOW;

//HC-05
// Ch:
// 00000001 - Button 1 pressed
// 00000010 - Button 2 pressed
// 00000011 - Button 3 pressed
const int hcPowerPIN = 8;      // HC-05 power control pin 
const int gRxPin = 4;
const int gTxPin = 3;

String hcBuf = "";
String hcStatus = "";

SoftwareSerial hcSerial(gRxPin, gTxPin);

//DISPLAY
const int displayPowerPIN = 9;      // display power control pin 

bool   displayBackLight = false;
String displayString = "";
int    displayBackLightTime = 150; // 15 sec
int    displayBackLightCount = 0;

byte s1[8] = {6, 10, 10, 10, 31, 17, 17, 0};  // Д
byte s2[8] = {14, 1, 1, 6, 1, 1, 14, 0};      // З
byte s3[8] = {17, 17, 17, 19, 21, 25, 17, 0}; // И
byte s4[8] = {21, 21, 17, 19, 21, 25, 17, 0}; // Й
byte s5[8] = {15, 9, 9, 9, 9, 9, 17, 0};      // Л
byte s6[8] = {17, 17, 17, 15, 1, 17, 15, 0};  // У
byte s7[8] = {15, 17, 17, 15, 3, 5, 25, 0};   // Я
byte s8[8] = {17, 17, 17, 25, 21, 21, 25, 0}; // Ы

LiquidCrystal_I2C lcd(0x3F,16,2);

int menuState = 1;

void setup() {
  
  displayInit();
  motorInit();
  keysInit();
  timeInit();
  hcInit();
    
  displayPrintLn("   \7 DOMOBO\4!    ", 0, 0);
  displayPrintLn("     ", 11, 0);
  
}

void loop(){
  
  while(1) {
    keysLoop();
    hcLoop();
    menuLoop(); 
    displayBackLightLoop();
  }
}

