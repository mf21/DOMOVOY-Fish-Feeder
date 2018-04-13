/*
  Fish Feeder DOMOVOY v.4.0 firmware
  Specs: DIP board, uln2003, ds1307, HC-05, battery, analog keys
  rev. 4.0.2
  
  This file contains RTC procedures
  More information about the project can be found here: http://www.mf21.ru

  created 2017
  by KWL  
*/ 

void timeInit() {
  RTC.control(DS1307_CLOCK_HALT, DS1307_OFF);
  RTC.control(DS1307_12H, DS1307_OFF);
//  RTC.s = 1;
//  RTC.writeTime();
}

void timeSetHour(int Hour) {
  RTC.readTime();
  RTC.h = Hour;
  RTC.writeTime();
}

void timeSetMinute(int Minute) {
  RTC.readTime();
  RTC.m = Minute;
  RTC.writeTime();
}

String timeFormatZero(int Param) {

  if (Param < 10) {
    return "0" + String(Param);
  } else {
    return String(Param);
  }
  
}

String timeFormatHMS(int Hour, int Min, int Sec) {
  return timeFormatZero(Hour) + ":" + timeFormatZero(Min) + ":" + timeFormatZero(Sec);
}

String timeFormatHM(int Hour, int Min) {
  return timeFormatZero(Hour) + ":" + timeFormatZero(Min);
}

