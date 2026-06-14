#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "main.h"

#define MEASUREPIN 4
volatile bool isTriggered = false;

#define TRIGPIN 13
#define ECHOPIN 14
#define MAX_DISTANCE 700
float timeOut = MAX_DISTANCE * 60;  // max waiting time
int soundVelocity = 343;    // 343 m/s | 1234,8 km/h

#define SDA 21  // SDA I2C
#define SCL 22  // SCL I2C

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
    Serial.begin(9600);
    Wire.begin(SDA, SCL);
    lcd.init();                      // initialize the lcd 
    lcd.backlight();                // activate backlight of lcd display
    pinMode(TRIGPIN, OUTPUT);
    pinMode(ECHOPIN, INPUT);
    pinMode(MEASUREPIN, INPUT_PULLUP);  
    bootIntro();
    attachInterrupt(MEASUREPIN, ISR, CHANGE);
    Serial.println("booting successfully");
}

void loop() {
    measureDistance();
}

void bootIntro() {
    lcd.setCursor(0,0);
    lcd.print("starte");
    lcd.setCursor(0,1);
    lcd.print("Entfernungsmesser");
    delay(1000);
    lcd.clear();
    Serial.println("starte Entfernungsmesser... ");
}

void printDistance(float distance) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Distanz:");
    lcd.setCursor(6,1);
    lcd.print("cm |");
    lcd.setCursor(15,1);
    lcd.print("m");
    lcd.setCursor(0,1);
    lcd.print(distance);    // print distance in centimeter
    lcd.setCursor(11,1);
    lcd.print((distance/100));  // print distance in meter
}

float getSonar() {
    unsigned long pingTime;
    float distance;
    // make TRIGPIN output high level lasting for 10 microseconds to trigger HC_SR04
    digitalWrite(TRIGPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGPIN, LOW);
    // Wait HC-SR04 returning to the high level and measure out this waiting time
    pingTime = pulseIn(ECHOPIN, HIGH, timeOut);
    // calculate according to the time
    distance = (float)pingTime * soundVelocity / 2 / 10000;
    return distance;
}

void printDistanceOnSerial(float distance) {
    Serial.print("Gemessene Distanz: ");
    Serial.print(distance);
    Serial.print(" cm");
    Serial.println();
}

void ISR() {
    isTriggered = true;
}

void measureDistance() {
    if (isTriggered) {
        float distance = getSonar();
        printDistance(distance);
        //printDistanceOnSerial(distance);
        isTriggered = false;
    }
}
