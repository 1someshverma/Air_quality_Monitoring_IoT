#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

int Gas = 7;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Print a message to the LCD.

  lcd.setCursor(0,0);
  lcd.print("Gas Detected :");
  
  lcd.setCursor(1,2);
  lcd.print("www.TheEngineering");
  lcd.setCursor(4,3);
  lcd.print("Projects.com");
  pinMode(Gas , INPUT);
}

void loop() {
  
  if(digitalRead(Gas) == HIGH){lcd.setCursor(14,0);lcd.print(" Yes");}
  if(digitalRead(Gas) == LOW){lcd.setCursor(14,0);lcd.print(" No ");}
 
}

