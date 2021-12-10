/*
 * Headder.h
 *
 * Created: 06-12-2021 14:26:23
 *  Author: luka0591
 */ 


#ifndef HEADDER_H_
#define HEADDER_H_

//Include libs----------
#include <SPI.h>
#include <MFRC522.h>
#include <TimerOne.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
//----------------------

//Door-----------------
#define SS_PIN 53
#define RST_PIN 52
#define RED_Door 12
#define Green_Door 13

Servo servoDoor;
unsigned long DELAY_TIME = 2000; // 1 sec
unsigned long DELAY_LED = 10000;
unsigned long delayStart = 0; // the time the delay started
unsigned long delayLed = 0;
bool delayRunning = false; // true if still waiting for delay to finish
bool switchLED = false;

void FrontDoor();
void DoorLight();
//---------------------

//OLED display---------------------
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
//---------------------------------

//DHT11 sensor---------------
#define DHTPIN 2
#define DHTTYPE DHT11
//---------------------------

//Temperature Control------------
#define CoolerPin 3
#define HeaterPin 30
#define WindowPin 6

Servo servoWindow;
float targetTemp = 25.5;
long targetHumidity = 30;
String keyValue = "";
float newValue = 0;
int windowVal = 0;

void TemperaturControl();
void ShowDisplay();
//------------------------------

//Ultrasonic Sensor Module--------------------------------
#define echoPin 7
#define trigPin 4

long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

void MeasureDistance();
//--------------------------------------------------------

//Keypad----------------------------
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};


byte rowPins[ROWS] = {49, 48, 47, 46};
byte colPins[COLS] = {45, 44, 43, 42};
//-------------------------------------

const int rs = 8, en = 6, d4 = 4, d5 = 3, d6 = 7, d7 = 6;
#endif /* HEADDER_H_ */