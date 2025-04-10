#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>

#define WHEEL_PIN A1 // motor pin
#define PEC_PIN 2 // photoelectric counter pin
#define LED 13 // int led

LiquidCrystal_I2C lcd(0x27, 16, 2);

int interval, wheel, counter;
unsigned long previousMicros, usInterval, calc;
volatile int speed;
volatile bool updateFlag = false;

void setup() {
  counter = 0;  // setting counter to 0
  interval = 1;  // 1 second interval
  wheel = 20; // number of encoder disc holes

  calc = 60 / interval;  // calculate interval to one minute
  usInterval = interval * 1000000;  // convert interval to micro

  wheel = wheel * 2;  // number of encoder disc holes times 2 

  Timer1.initialize(usInterval);  // initialize timer with interval time
  attachInterrupt(digitalPinToInterrupt(PEC_PIN), count, CHANGE);

  Timer1.attachInterrupt(output);  // executes output after interval time
  
  lcd.init();
  Serial.begin(9600);
  pinMode(WHEEL_PIN, INPUT);
  pinMode(PEC_PIN, INPUT);
  pinMode(LED, OUTPUT);
  
  lcd.backlight();
  lcd.clear();
}

void loop() {
  if (updateFlag) {
    lcd.setCursor(0, 0);
    lcd.print("RPM:");
    lcd.print(speed);
    lcd.setCursor(0, 1);
    printVoltage(getVoltage());
    lcd.setCursor(0, 1);
    updateFlag = false;
  }
}

float getVoltage() {
  float Measurement = analogRead(WHEEL_PIN);
  Serial.println(Measurement);
  float Voltage = Measurement * 5.0/1023 * 10;
  return Voltage/10;
}

void printVoltage(float voltage) {
  Serial.print(F("Voltage: "));
  Serial.println(voltage, 2);
  lcd.print(F("Voltage: "));
  lcd.print(voltage, 2);
}


void count(){
  if (micros() - previousMicros >= 700) {
    counter++;
    previousMicros = micros();
  }
}

void output(){
  Timer1.detachInterrupt(); // interrupts the timer
  Serial.print("Drehzahl pro Minute: ");
  speed = ((counter)*calc) / wheel; // calculate round per minute
  
  Serial.println(speed);
  updateFlag = true;
  
  counter = 0;  // resetting the counter
  Timer1.attachInterrupt(output);  // restarts the timer for output
}
