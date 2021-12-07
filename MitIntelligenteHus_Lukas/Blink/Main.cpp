#include <Arduino.h>


#include "Headder.h"

unsigned long DELAY_TIME = 2000; // 1 sec
unsigned long DELAY_LED = 10000;
unsigned long delayStart = 0; // the time the delay started
unsigned long delayLed = 0;
bool delayRunning = false; // true if still waiting for delay to finish

bool switchLED = false;

Servo myservo;
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
void setup()
{
	Serial.begin(9600);		// Initiate a serial communication
	SPI.begin();			// Initiate  SPI bus
	mfrc522.PCD_Init();		// Initiate MFRC522
	Timer1.initialize(5000000);  // Initiate Timer1
	Timer1.attachInterrupt(DoorLight);
	
	//Pin setup
	myservo.attach(10);
	pinMode(RED, OUTPUT);
	pinMode(Green, OUTPUT);
}

void loop()
{
	FrontDoor();
	DoorLight();
}

void DoorLight()
{
	if (millis() - delayLed >= DELAY_LED)
	{
		
		delayLed = millis();
		switchLED = false;
		myservo.write(90);
	}
	digitalWrite(RED, !switchLED);
	digitalWrite(Green, switchLED);
}

void FrontDoor()
{
	if (millis() - delayStart >= DELAY_TIME)
	{
		delayStart = millis();
		if ( ! mfrc522.PICC_IsNewCardPresent())
		{
			
			return;
		}
		if (mfrc522.PICC_ReadCardSerial())
		{
			Serial.println("Read card!");
			String content= "";
			byte letter;
			for (byte i = 0; i < mfrc522.uid.size; i++)
			{
				content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
				content.concat(String(mfrc522.uid.uidByte[i], HEX));
			}
			content.toUpperCase();
			
			if (content.substring(1) == "E2 B6 DF 1B")
			{
				//Authorized access
				switchLED = true;
				myservo.write(0);
			}
		}
	}
}
