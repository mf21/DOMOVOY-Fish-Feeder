/*
  Fish Feeder DOMOVOY v.4.0 firmware
  Specs: DIP board, uln2003, ds1307, HC-05, battery, analog keys
  rev. 4.0.2
  
  This file contains motor procedures
  More information about the project can be found here: http://www.mf21.ru

  created 2017
  by KWL  
*/ 

void motorInit() {

  byte RetractionState;
  
  motorNumTurns = EEPROM.read(ROMMotorOffset);
  RetractionState = EEPROM.read(ROMRetractionOffset);

  if (RetractionState == 255) {
    RetractionState = 1;
    EEPROM.write(ROMRetractionOffset, RetractionState);
  }
  
}

void motorRotate(long Turns) {

unsigned long Angle;
byte RetractionState;

  RetractionState = EEPROM.read(ROMRetractionOffset);
  Angle = Turns * motorTurn + motorRetractionBefore * motorTurn * RetractionState;
  
  motor.setSpeed(motorSpeed);
  motor.step(motorRetractionBefore * motorTurn);

  if (Angle < 32767) {
    motor.step(-Angle);  
  } else { 
    for (int i=0; i<int(Angle / 32767); i++) {
      motor.step(-32767);  
    }
    motor.step(-1 * (Angle - int(Angle / 32767)*32767));
  }

  motor.step(motorRetractionAfter * motorTurn);

  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

