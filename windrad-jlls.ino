#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>

#define WHEEL_PIN A1 
#define PEC_PIN 2
#define LED 13

LiquidCrystal_I2C lcd(0x27, 16, 2);

int period_duration = 0;
int interval, wheel, counter;
unsigned long previousMicros, usInterval, calc;

void setup() {
  counter = 0;  // setting counter to 0
  interval = 1;  // 5 second interval
  wheel = 20; // number of encoder disc holes

  calc = 60 / interval;  // calculate interval to one minute
  usInterval = interval * 1000000;  // convert interval to micro

  wheel = wheel * 2;  // number of encoder disc wholes times 2 

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
  lcd.setCursor(0, 0);
  printVoltage(getVoltage());
  output();

  Serial.println(period_duration);
  
  delay(100);
}

float getVoltage() {
  float Messurement = analogRead(WHEEL_PIN);
  float Voltage = map(Messurement, 0, 1023, 0, 255);
  return Voltage/10;
}

void printVoltage(float voltage) {
  Serial.print(F("Voltage: "));
  Serial.println(voltage, 2);
  lcd.print(F("Voltage: "));
  lcd.print(voltage, 2);
}

//bool checkHall() {
//  int sensorStatus = digitalRead(HallSensor);
//  if (sensorStatus == 1) {
//    digitalWrite(LED, HIGH);
//    Serial.println("YEEEES");
//    return true;
//
//  } else  {
//    digitalWrite(LED, LOW); 
//    Serial.println("Nooooo");
//    return false;
//  }
//}

void count(){
  if (micros() - previousMicros >= 700) {
    counter++;
    previousMicros = micros();
  }
}

void output(){
  Timer1.detachInterrupt(); // interrupts the timer
  Serial.print("Drehzahl pro Minute: ");
  int speed = ((counter)*calc) / wheel;
  // calculate round per minute
  
  Serial.println(speed);
  counter = 0;  // resetting the counter
  Timer1.attachInterrupt(output);  // restarts the timer for output
}
