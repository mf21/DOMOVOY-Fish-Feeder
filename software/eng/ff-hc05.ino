/*
  Fish Feeder DOMOVOY v.4.0 firmware
  Specs: DIP board, uln2003, ds1307, HC-05, battery, analog keys
  rev. 4.0.2
  
  This file contains HC-05 module procedures
  More information about the project can be found here: http://www.mf21.ru

  created 2017
  by KWL  
*/ 

void hcInit() {
  pinMode (hcPowerPIN, OUTPUT);  
  digitalWrite(hcPowerPIN, HIGH);
  hcSerial.begin(9600);
  delay(500);
}

void hcCommand() {

  if (hcBuf.substring(0, 8) == "settime:") {
    int timeHour = hcBuf.substring(8, 10).toInt();
    int timeMin = hcBuf.substring(11, 13).toInt();
    
    if (timeHour < 0 || timeHour > 23) {
      hcStatus = "Error:timeHour is not valid";
      Serial.println(hcStatus);
      hcSerial.println(hcStatus);
      hcBuf = "";
      return;
    }
    
    if (timeMin < 0 || timeMin > 59) {
      hcStatus = "Error:timeMin is not valid";
      Serial.println(hcStatus);
      hcSerial.println(hcStatus);
      hcBuf = "";
      return;
    }

    hcStatus = "OK:time set";
    Serial.println(hcStatus);
    hcSerial.println(hcStatus);
    
    timeSetHour(timeHour);
    timeSetMinute(timeMin);
    displayBackLightOn();
    menuTimeCurrent();
    hcBuf = "";
    return;
  }

  if (hcBuf.substring(0, 8) == "setalarm") {
    int alNum = hcBuf.substring(8, 9).toInt();
    int alHour = hcBuf.substring(10, 12).toInt();
    int alMin  = hcBuf.substring(13, 15).toInt();

    if (alNum < 1 || alNum > 4) {
      hcStatus = "Error:alNum is not valid";
      Serial.println(hcStatus);
      hcSerial.println(hcStatus);
      hcBuf = "";
      return;
    }

    if (alHour < 0 || alHour > 23) {
      hcStatus = "Error:alHour is not valid";
      Serial.println(hcStatus);
      hcSerial.println(hcStatus);
      hcBuf = "";
      return;
    }
    
    if (alMin < 0 || alMin > 59) {
      hcStatus = "Error:alMin is not valid";
      Serial.println(hcStatus);
      hcSerial.println(hcStatus);
      hcBuf = "";
      return;
    }

    hcStatus = "OK:alarm set";
    Serial.println(hcStatus);
    hcSerial.println(hcStatus);

    EEPROM.write(2 * alNum - 2, alHour);
    EEPROM.write(2 * alNum - 1, alMin);
    
    displayBackLightOn();
    menuState = 2 + alNum;
    menuAlarmSet(alNum);
    hcBuf = "";
    return;
  }


  if (hcBuf.substring(0, 12) == "setmotorturn") {
    int Turns = hcBuf.substring(13).toInt();

    if (Turns <= 0) {
      hcStatus = "Error:Turn less or equal 0";
      Serial.println(hcStatus);
      hcSerial.println(hcStatus);
      hcBuf = "";
      return;
    }
  
    if (Turns > motorNumTurnsMax) {
      hcStatus = "Error:Turn is too large";
      Serial.println(hcStatus);
      hcSerial.println(hcStatus);
      hcBuf = "";
      return;
    }

    hcStatus = "OK:motorturn set";
    Serial.println(hcStatus);
    hcSerial.println(hcStatus);

    motorNumTurns = Turns;
    EEPROM.write(ROMMotorOffset, motorNumTurns);
    
    displayBackLightOn();
    menuState = 7;
    menuMotorAngleSet();    
    hcBuf = "";
    return;
  }

  if (hcBuf.substring(0, 13) == "setretraction") {
    byte Retract = hcBuf.substring(14).toInt();

    if ((Retract < 0)||(Retract > 1) ) {
      hcStatus = "Error:Retract can be 0 or 1";
      Serial.println(hcStatus);
      hcSerial.println(hcStatus);
      hcBuf = "";
      return;
    }
  
    EEPROM.write(ROMRetractionOffset, Retract);
    
    hcStatus = "z";
    Serial.println(hcStatus);
    hcSerial.println(hcStatus);

    displayBackLightOn();
    menuState = 8;
    menuRetractionSet();    
    hcBuf = "";
    return;
  }

  if (hcBuf.substring(0, 9) == "motortest") {
    hcStatus = "OK:Test feeding";
    Serial.println(hcStatus);
    hcSerial.println(hcStatus);
    menuState = 9;
    keysBTN2State = HIGH;
    hcBuf = "";
    return;
  }  
  
  if (hcBuf.substring(0, 7) == "gettime") {
    RTC.readTime();
    hcStatus = "OK:time:" + timeFormatZero(RTC.h) + ":" + timeFormatZero(RTC.m) + ":" + timeFormatZero(RTC.s);
    Serial.println(hcStatus);
    hcSerial.println(hcStatus);
    hcBuf = "";
    return;
  }

  if (hcBuf.substring(0, 8) == "getalarm") {
    int alNum = hcBuf.substring(8, 9).toInt();

    if (alNum < 1 || alNum > 4) {
      hcStatus = "Error:alNum is not valid";
      Serial.println(hcStatus);
      hcSerial.println(hcStatus);
      hcBuf = "";
      return;
    }

    int alHour = EEPROM.read(2 * alNum - 2);
    int alMin  = EEPROM.read(2 * alNum - 1);

    hcStatus = "OK:alarm" + String(alNum) + ":" + timeFormatZero(alHour) + ":" + timeFormatZero(alMin);
    Serial.println(hcStatus);
    hcSerial.println(hcStatus);

    hcBuf = "";
    return;
  }

  if (hcBuf.substring(0, 13) == "getretraction") {

    byte Retract = EEPROM.read(ROMRetractionOffset);
    hcStatus = "OK:retraction:" + String(Retract);
    Serial.println(hcStatus);
    hcSerial.println(hcStatus);

    hcBuf = "";
    return;
  }
    
  if (hcBuf.substring(0, 12) == "getmotorturn") {

    int Turns = EEPROM.read(ROMMotorOffset);
    hcStatus = "OK:motorturn:" + String(Turns);
    Serial.println(hcStatus);
    hcSerial.println(hcStatus);

    hcBuf = "";
    return;
  }
  
  hcStatus = "Error: Command is not recognized";
  Serial.println(hcStatus);
  hcSerial.println(hcStatus);
  hcBuf = "";
}

void hcLoop() {

char Ch;

  if (hcSerial.available()) {
    Ch = hcSerial.read();
    if (byte(Ch) == 10) {
      hcCommand();        
    } else {
      hcBuf = hcBuf + Ch;
    }
  }
  
}

