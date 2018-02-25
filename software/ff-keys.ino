/*
  Fish Feeder DOMOVOY v.4.0 firmware
  Specs: DIP board, uln2003, ds1307, HC-05, battery, analog keys
  rev. 4.0.2

  This file contains keys procedures
  More information about the project can be found here: http://www.mf21.ru

  created 2017
  by KWL  
*/ 

void keysInit() {
  //
}

void keysLoop() {

  int keyPortData = analogRead(keyPort);

  //Serial.println(keyPortData);
  
  if (keyPortData > 600 && keyPortData < 700) {
    keysBTN1State = HIGH;
  } else {
    keysBTN1State = LOW;
  }
  
  if (keyPortData > 450 && keyPortData < 600) {
    keysBTN2State = HIGH;
  } else {
    keysBTN2State = LOW;
  }

  if (keyPortData < 450) {
    keysBTN3State = HIGH;
  } else {
    keysBTN3State = LOW;
  }


}

