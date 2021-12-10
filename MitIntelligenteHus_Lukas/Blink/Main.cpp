#include <Arduino.h>

#include "Headder.h"

// Create instances.
#pragma region Instances
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
MFRC522 mfrc522(SS_PIN, RST_PIN);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd = LiquidCrystal(22, 23, 24, 25, 26, 27);
#pragma endregion Instances

void setup()
{
	Serial.begin(9600);		// Initiate a serial communication
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initiate a OLED display
	SPI.begin();			// Initiate  SPI bus
	mfrc522.PCD_Init();		// Initiate MFRC522
	dht.begin();			// Initiate DHT11 Sensor
	lcd.begin(16, 2);		// Initiate LCD Display
	
	//Pin setup------------------------------
	servoDoor.attach(10); //Door Servo
	servoWindow.attach(6); //Window Servo

	pinMode(RED_Door, OUTPUT);
	pinMode(Green_Door, OUTPUT);
	
	pinMode(9, OUTPUT);
	pinMode(2, INPUT);
	
	pinMode(42, INPUT);
	pinMode(43, INPUT);
	pinMode(44, INPUT);
	pinMode(45, INPUT);
	pinMode(46, INPUT);
	pinMode(47, INPUT);
	pinMode(48, INPUT);
	pinMode(49, INPUT);
	
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
	//---------------------------------------
	
	//Cooler--------------------
	pinMode(CoolerPin, OUTPUT);
	//--------------------------
	
	//Heater--------------------
	pinMode(HeaterPin, OUTPUT);
	//--------------------------
	
	//Window--------------------
	pinMode(WindowPin, OUTPUT);
	//--------------------------
}

void loop()
{
	
	FrontDoor();
	DoorLight();
	TemperaturControl();
	MeasureDistance();
}

#pragma region Door
void DoorLight()
{
	if (millis() - delayLed >= DELAY_LED)
	{
		
		delayLed = millis();
		if (switchLED)
		{
			tone(9, 3500, 250);
			delay(400);
			tone(9, 3000, 250);
		}
		switchLED = false;
		
		servoDoor.write(95);
	}
	digitalWrite(RED_Door, !switchLED);
	digitalWrite(Green_Door, switchLED);
}
void FrontDoor()
{
	delayStart = millis();
	if (! mfrc522.PICC_IsNewCardPresent())
	{
		return;
	}
	if (mfrc522.PICC_ReadCardSerial() || ! mfrc522.PICC_IsNewCardPresent())
	{
		String content= "";
		byte letter;
		for (byte i = 0; i < mfrc522.uid.size; i++)
		{
			content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
			content.concat(String(mfrc522.uid.uidByte[i], HEX));
		}
		content.toUpperCase();
		
		if (content.substring(1) == "E2 B6 DF 1B")//Authorized access
		{
			tone(9, 3000, 250);
			delay(400);
			tone(9, 3500, 250);
			switchLED = true;
			servoDoor.write(0);
		}
		else//Unauthorized access
		{
			tone(9, 1000, 250);
			
		}
	}
}
#pragma endregion Door

#pragma region Temp
void TemperaturControl()
{
	ShowDisplay();
	
	if (dht.readTemperature() >= (targetTemp + 0.2))
	{
		analogWrite(CoolerPin, 255);
		digitalWrite(HeaterPin, LOW);
		servoWindow.write(100);
	}
	else if (dht.readTemperature() >= targetTemp && dht.readHumidity() >= targetHumidity)
	{
		servoWindow.write(180);
	}
	else if (dht.readTemperature() <= (targetTemp - 0.2))
	{
		analogWrite(CoolerPin, 0);
		digitalWrite(HeaterPin, HIGH);
		servoWindow.write(180);
	}
	
	if (dht.readHumidity() >= targetHumidity)
	{
		servoWindow.write(40);
	}
	
	char customKey = customKeypad.getKey();
	if (customKey)
	{
		switch (customKey)
		{
			case 'A':
			newValue = keyValue.toFloat();
			targetTemp = newValue;
			keyValue = "";
			analogWrite(6, 255);
			break;
			
			case 'B':
			newValue = keyValue.toFloat();
			targetHumidity = newValue;
			keyValue = "";
			analogWrite(6, 0);
			break;
			
			case 'C':
			keyValue = keyValue.substring(0, keyValue.length()-1);
			break;
			
			case 'D':
			keyValue = "";
			break;
			
			case '*':
			keyValue += '.';
			break;

			default:
			keyValue += customKey;
			break;
		}
	}
}
void ShowDisplay()
{
	//OLED-----------------------------------------------
	display.clearDisplay();

	display.setTextSize(1); // Normal 1:1 pixel scale
	display.setTextColor(WHITE); // Draw white text
	display.setCursor(0,0); // Start at top-left corner
	
	display.println("Smart house Temp sys");
	display.println("---------------------");
	display.print("Target temp:");
	display.print(targetTemp, 1);
	display.println(" C");
	display.print("Current temp:");
	display.print(float(dht.readTemperature()),1);
	display.println(" C");
	display.println("---------------------");
	display.print("Target humidity:");
	display.print(targetHumidity);
	display.println("%");
	display.print("Current humidity:");
	display.print(float(dht.readHumidity()),0);
	display.println("%");
	display.print("---------------------");
	display.display();
	//------------------------------------------------
	
	//LCD---------------------------------------------
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(keyValue);
	lcd.setCursor(12, 0);
	lcd.print("Set:");
	lcd.setCursor(0, 1);
	lcd.println("A =temp B =humid");
	//------------------------------------------------
}
#pragma endregion Temp

#pragma region Ultrasonic Sensor Module
void MeasureDistance()
{
	// Clears the trigPin condition
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	// Sets the trigPin HIGH (ACTIVE) for 10 microseconds
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	// Reads the echoPin, returns the sound wave travel time in microseconds
	duration = pulseIn(echoPin, HIGH);
	// Calculating the distance
	distance = duration / 58; // Speed of sound wave divided by 2 (go and back)

	if (distance <= 30)
	{
		String tempDelay = String(distance);
		tempDelay += "0";
		int distanceDelay = tempDelay.toInt();
		Serial.println(tempDelay);
		tone(9, 3000, distance);
		delay(distanceDelay);
	}
}
#pragma endregion Ultrasonic Sensor Module