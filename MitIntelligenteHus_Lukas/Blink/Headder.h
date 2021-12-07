/*
 * Headder.h
 *
 * Created: 06-12-2021 14:26:23
 *  Author: luka0591
 */ 


#ifndef HEADDER_H_
#define HEADDER_H_

#include <SPI.h>
#include <MFRC522.h>
#include <TimerOne.h>
#include <Servo.h>

#define SS_PIN 53
#define RST_PIN 52
#define RED 12
#define Green 13

void FrontDoor();
void DoorLight();



#endif /* HEADDER_H_ */