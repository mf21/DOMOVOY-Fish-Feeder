/*
  Fish Feeder DOMOVOY v.4.0 firmware
  Specs: DIP board, uln2003, ds1307, HC-05, battery, analog keys
  rev. 4.0.2
    
  This file contains display procedures
  More information about the project can be found here: http://www.mf21.ru

  created 2017
  by KWL  
*/  
 
void displayInit() {
  pinMode(displayPowerPIN, OUTPUT);
  digitalWrite(displayPowerPIN, HIGH);
  lcd.init();
  displayBackLightOn();
}

void displayClear() {
  if (displayBackLight) {
    lcd.clear(); 
  }
}

void displayPrintLn(String text, int X, int Y) {
  if (text != displayString) {
    displayString = text;
    lcd.setCursor(X, Y);
    lcd.print(displayString);  
  }
}

void displayPrintLightOn(String text, int X, int Y) {
  displayBackLightOn();
  displayBackLightCount = 0;
  displayPrintLn(text, X, Y);
}

void displayBackLightOn() {
  lcd.backlight();  
  displayBackLight = true;
}

void displayBackLightOff() {
  lcd.noBacklight();
  displayBackLight = false;
  displayBackLightCount = 0;
  menuState = 1;
}

void displayBackLightLoop() {

  if (displayBackLight) {
    displayBackLightCount++;
  }

  if (displayBackLightCount >= displayBackLightTime) {
    displayBackLightOff();
  }
  
}

