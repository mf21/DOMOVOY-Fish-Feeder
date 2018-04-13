/*
  Fish Feeder DOMOVOY v.4.0 firmware
  Specs: DIP board, uln2003, ds1307, HC-05, battery, analog keys
  rev. 4.0.2

  This file contains menu procedures
  More information about the project can be found here: http://www.mf21.ru

  Button 1 - Change mode
  Button 2 - Set first value
  Button 3 - Set second value

  created 2017
  by KWL  
*/ 

 void menuLoop() {
      
  if (menuAlarmCheck(1)) {
    displayPrintLightOn("   KOPM\5EH\3E 1   ", 0, 0);
    displayPrintLn("    H\7M-H\7M!    ", 0 ,1);
    motorRotate(motorNumTurns);
  } else if (menuAlarmCheck(2)) {
    displayPrintLightOn("   KOPM\5EH\3E 2   ",  0, 0);
    displayPrintLn("    H\7M-H\7M!    ", 0 ,1);
    motorRotate(motorNumTurns);
  } else if (menuAlarmCheck(3)) {
    displayPrintLightOn("   KOPM\5EH\3E 3   ", 0 ,0);
    displayPrintLn("    H\7M-H\7M!    ", 0 ,1);
    motorRotate(motorNumTurns);
  } else if (menuAlarmCheck(4)) {
    displayPrintLightOn("   KOPM\5EH\3E 4   ", 0, 0);
    displayPrintLn("    H\7M-H\7M!    ", 0 ,1);
    motorRotate(motorNumTurns);
  }

  if (menuState == 1) {
    menuTimeCurrent();
  } else if (menuState == 2) {
    menuTimeSet();
  } else if (menuState == 3) {
    menuAlarmSet(1);
  } else if (menuState == 4) {
    menuAlarmSet(2);
  } else if (menuState == 5) {
    menuAlarmSet(3);
  } else if (menuState == 6) {
    menuAlarmSet(4);
  } else if (menuState == 7) {
    menuMotorAngleSet();
  } else if (menuState == 8) {
    menuRetractionSet();
  } else if (menuState == 9) {
    menuMotorTest();
  }
 
}

void menuTimeCurrent() {

   if (keysBTN1State == HIGH && menuState == 1 && !displayBackLight) {
     displayBackLightOn();
     keysBTN1State = LOW;
     delay(400);
     return;
   }

   if (keysBTN2State == HIGH && menuState == 1 && !displayBackLight) {
     displayBackLightOn();
     keysBTN2State = LOW;
     delay(400);
     return;
   }

   if (keysBTN3State == HIGH && menuState == 1 && !displayBackLight) {
     displayBackLightOn();
     keysBTN3State = LOW;
     delay(400);
     return;
   }

   if (keysBTN1State == HIGH && menuState == 1) {
     menuState = 2;
     displayBackLightCount = 0;
     delay(400);
   }

  RTC.readTime();
  displayPrintLn("   \7 DOMOBO\4!    ", 0, 0);
  displayPrintLn("    " + timeFormatHMS(RTC.h, RTC.m, RTC.s) + "    ", 0, 1);
}

void menuTimeSet() {

int curHour;
int curMin;

    RTC.readTime();
    curHour = RTC.h;
    curMin = RTC.m;

  if (keysBTN2State == HIGH) {
     curHour++;
     displayBackLightCount = 0;
     
     if (curHour > 23) {
      curHour = 0;
     }

     timeSetHour(curHour);
     delay(250);
  }

  if (keysBTN3State == HIGH) {
     curMin++;
     displayBackLightCount = 0;

     if (curMin > 59) {
      curMin = 0;
    }

     timeSetMinute(curMin);
     delay(250);
   }

   if (keysBTN1State == HIGH && menuState == 2) {
     keysBTN1State = LOW;
     displayBackLightCount = 0;
     menuState = 3;
     delay(400);
   }

  displayPrintLn("\6CT. BPEMEH\3    ", 0, 0);
  displayPrintLn("     " + timeFormatHM(curHour, curMin) + "    ", 0, 1);

}

void menuAlarmSet(int AlarmNum) {

int alHour;
int alMin;

  alHour = EEPROM.read(2 * AlarmNum - 2);
  alMin  = EEPROM.read(2 * AlarmNum - 1);

  if (alHour == 255) {
    alHour = 0;
  }

  if (alMin == 255) {
    alMin = 0;
  }

  if (keysBTN2State == HIGH) {
     alHour++;
     displayBackLightCount = 0;

     if (alHour > 23) {
      alHour = 0;
     }

     EEPROM.write(2 * AlarmNum - 2, alHour);
     delay(250);
  }

  if (keysBTN3State == HIGH) {
     alMin++;
     displayBackLightCount = 0;

     if (alMin > 59) {
      alMin = 0;
    }

     EEPROM.write(2 * AlarmNum - 1, alMin);
     delay(250);
   }

   if (keysBTN1State == HIGH && menuState == AlarmNum + 2) {
     keysBTN1State = LOW;
     displayBackLightCount = 0;
     menuState = AlarmNum + 3;
     delay(400);
   }

  displayPrintLn("\6CT. KOPM\5EH\3\7 " + String(AlarmNum), 0, 0);
  displayPrintLn("     " + timeFormatHM(alHour, alMin) + "    ", 0, 1);
}

boolean menuAlarmCheck(int AlarmNum) {

int alHour;
int alMin;

  alHour = EEPROM.read(2 * AlarmNum - 2);
  alMin  = EEPROM.read(2 * AlarmNum - 1);

  if (alHour == 0 && alMin == 0) {
    return false;
  }

  RTC.readTime();
  if (alHour == RTC.h && alMin == RTC.m && RTC.s == 0) {
    delay(1000);
    return true;
  } else {
    return false;
  }

}

void menuMotorAngleSet() {

int tmpMotorNumTurns;

  tmpMotorNumTurns = EEPROM.read(ROMMotorOffset);

  if (tmpMotorNumTurns == 255) {
    tmpMotorNumTurns = motorNumTurnsMax;
  }

  if (keysBTN2State == HIGH) {
     tmpMotorNumTurns = tmpMotorNumTurns + 1;
     displayBackLightCount = 0;

     if (tmpMotorNumTurns > motorNumTurnsMax) {
      tmpMotorNumTurns = 0;
     }

     motorNumTurns = tmpMotorNumTurns;
     EEPROM.write(ROMMotorOffset, motorNumTurns);
     delay(400);
  }

   if (keysBTN1State == HIGH) {
     keysBTN1State = LOW;
     displayBackLightCount = 0;
     menuState = 8;
     delay(400);
   }

  displayPrintLn("\6CT. \1O\2", 0, 0);
  lcd.setCursor(8, 0);
  lcd.write(0);
  displayPrintLn(": " + String(motorNumTurns) + "    ", 9, 0);
  displayPrintLn("                ", 0, 1);
}

void menuRetractionSet() {

  byte RetractionState;

  RetractionState = EEPROM.read(ROMRetractionOffset);

  if (keysBTN2State == HIGH) {
     RetractionState = (RetractionState - 1) * -1;
     displayBackLightCount = 0;
     EEPROM.write(ROMRetractionOffset, RetractionState);
     delay(400);
  }

   if (keysBTN1State == HIGH && menuState == 8) {
     menuState = 9;
     displayBackLightCount = 0;
     delay(400);
   }

  if (RetractionState == 1) {
    displayPrintLn("PETPAKT: \1A     ", 0, 0);
  } else {
    displayPrintLn("PETPAKT: HET    ", 0, 0);
  }
  
  displayPrintLn("                ", 0, 1);  
  
}

void menuMotorTest() {

  if (keysBTN2State == HIGH) {
    displayPrintLightOn("    H\7M-H\7M!    ", 0 ,1);
    motorNumTurns = EEPROM.read(ROMMotorOffset);
    motorRotate(motorNumTurns);
    keysBTN2State = LOW;
    menuState = 1;
  }

   if (keysBTN1State == HIGH && menuState == 9) {
     keysBTN1State = LOW;
     displayBackLightCount = 0;
     menuState = 1;
     delay(400);
   }

  displayPrintLn(" TECT KOPM\5EH\3\7 ", 0, 0);
  displayPrintLn("                ", 0, 1);
  
}


