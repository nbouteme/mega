#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  pinMode(7, OUTPUT);
  pinMode(8, INPUT);
}

float getDist() {
  digitalWrite(7, LOW);
  delayMicroseconds(2);
  digitalWrite(7, HIGH);
  delayMicroseconds(10);
  digitalWrite(7, LOW);
  int duration = pulseIn(8, HIGH);
  return duration * 0.01666f;
}

int state = 1;
void loop() {
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print(String("d: ") + getDist());
  delay(200);               
}
