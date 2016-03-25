/*
 Name:		RedController.ino
 Created:	3/7/2016 5:42:43 PM
 Author:	Piotr
*/
#include "Joystick.h"
#include "Mouse.h"

#define BT_GREEN 4
#define BT_BLUE 6
#define BT_RED 5
#define BT_YELLOW 7

#define BT_RIGHT 11
#define BT_LEFT 9
#define BT_UP 10
#define BT_DOWN 8

#define BT_START A4
#define BT_SELECT A5

#define LA_X A0
#define LA_Y A1
#define RA_X A2
#define RA_Y A3

#define COUNTOF(x) (sizeof(x)/sizeof(*x))

byte Buttons[] = { BT_GREEN, BT_BLUE, BT_RED, BT_YELLOW, BT_RIGHT, BT_LEFT };
bool sw = true;

// the setup function runs once when you press reset or power the board
void setup() 
{
	pinMode(BT_GREEN, INPUT_PULLUP);
	pinMode(BT_BLUE, INPUT_PULLUP);
	pinMode(BT_RED, INPUT_PULLUP);
	pinMode(BT_YELLOW, INPUT_PULLUP);
	pinMode(BT_RIGHT, INPUT_PULLUP);
	pinMode(BT_LEFT, INPUT_PULLUP);
	pinMode(BT_UP, INPUT_PULLUP);
	pinMode(BT_DOWN, INPUT_PULLUP);
	pinMode(BT_START, INPUT_PULLUP);
	pinMode(BT_SELECT, INPUT_PULLUP);
	pinMode(LA_X, INPUT_PULLUP);
	pinMode(LA_Y, INPUT_PULLUP);
	pinMode(RA_X, INPUT_PULLUP);
	pinMode(RA_Y, INPUT_PULLUP);
	
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	
	Joystick.begin(false);
	
	Joystick.setRudder(128);
	Joystick.setThrottle(128);
	Joystick.setXAxisRotation(180);
	Joystick.setYAxisRotation(180);

	Serial.begin(9600);
	//Mouse.begin();
}
const uint8_t pinToButtonMap = 4;
uint8_t lastButtonState[4] = { 0,0,0,0 };



// the loop function runs over and over again until power down or reset
void loop() 
{
	/*for (size_t i = 0; i < COUNTOF(Buttons) ; i++)
	{
		for each (int Button in Buttons)
		{

		}
	}*/

	for (uint8_t i = 0; i < 4; i++)
	{
		uint8_t currentButtonState = !digitalRead(i + pinToButtonMap);
		if (currentButtonState != lastButtonState[i])
		{
			Joystick.setButton(i, currentButtonState);
			lastButtonState[i] = currentButtonState;
			
		}
		
	}
	
	if (!digitalRead(BT_UP) && (!digitalRead(BT_RIGHT))) Joystick.setHatSwitch(0, 315);
	else if (!digitalRead(BT_UP) && (!digitalRead(BT_LEFT))) Joystick.setHatSwitch(0, 45);
	else if (!digitalRead(BT_DOWN) && (!digitalRead(BT_RIGHT))) Joystick.setHatSwitch(0, 225);
	else if (!digitalRead(BT_DOWN) && (!digitalRead(BT_LEFT))) Joystick.setHatSwitch(0, 135);
	else if (!digitalRead(BT_DOWN)) Joystick.setHatSwitch(0,180);
	else if (!digitalRead(BT_LEFT)) Joystick.setHatSwitch(0, 90);
	else if (!digitalRead(BT_UP)) Joystick.setHatSwitch(0, 360);
	else if (!digitalRead(BT_RIGHT)) Joystick.setHatSwitch(0, 270);
	
	else Joystick.setHatSwitch(0, -1);
	
	
	if (!digitalRead(BT_START)) Joystick.setButton(6, 1);
	else Joystick.setButton(6, 0);

	if (!digitalRead(BT_SELECT)) Joystick.setButton(7, 1);
	else Joystick.setButton(7, 0);

	if (!digitalRead(BT_START) && !digitalRead(BT_SELECT))
	{
		sw = !sw;
		digitalWrite(13, sw);
		delay(200);
	}

	Joystick.setXAxis(map(analogRead(LA_X), 16, 1023, -127, 127));
	Joystick.setYAxis(map(analogRead(LA_Y), 16, 1023, 127, -127));
	Joystick.setZAxis(map(analogRead(RA_X), 16, 1023, -127, 127));
	//Joystick.setYAxisRotation(map(analogRead(RA_Y), 0, 1023, 0, 360));
	//Joystick.setZAxisRotation(a);
	Joystick.setRudder(map(analogRead(RA_Y), 16, 1023, 0, 255));
	//Joystick.setThrottle(a);
	Joystick.sendState();
	
	

	//Serial.println(analogRead(LA_Y));
	delay(50);
}



void MouseMove()
{


}

void GyroMove()
{


}