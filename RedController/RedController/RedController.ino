/*
 Name:		RedController.ino
 Created:	3/7/2016 5:42:43 PM
 Author:	Piotr
*/
#include <Wire.h>
#include "L3G.h"
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



#define LED 13

#define COUNTOF(x) (sizeof(x)/sizeof(*x))

byte Buttons[] = { BT_GREEN, BT_BLUE, BT_RED, BT_YELLOW, BT_RIGHT, BT_LEFT };


float gyroScale[] = { 0.0001, 0.0001, 0.0001 };
int gyroOffest[] = { 88, 108, 12 };
float angularRotation[] = { 0, 0, 0 };

bool sw = true;

L3G gyro;

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
	
	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);

	
	Joystick.begin(false);
	Serial.begin(9600);
	Wire.begin();
	//Mouse.begin();


	if (!gyro.init(L3G::device_auto, L3G::sa0_auto));
	{
		Serial.println("Gyro ERROR!!!");
		while (1);
	}

	gyro.enableDefault();
	gyro.writeReg(L3G::CTRL_REG4, 0x20); //full scale 2000/s
	


	Joystick.setRudder(128);
	Joystick.setThrottle(128);
	Joystick.setXAxisRotation(180);
	Joystick.setYAxisRotation(180);
}
const uint8_t pinToButtonMap = 4;
uint8_t lastButtonState[4] = { 0,0,0,0 };



// the loop function runs over and over again until power down or reset
void loop() 
{
	for (uint8_t i = 0; i < 4; i++)
	{
		uint8_t currentButtonState = !digitalRead(i + pinToButtonMap);
		if (currentButtonState != lastButtonState[i])
		{
			Joystick.setButton(i, currentButtonState);
			lastButtonState[i] = currentButtonState;			
		}	
	}
	
#pragma region HatSwith

	if (!digitalRead(BT_UP) && (!digitalRead(BT_RIGHT))) Joystick.setHatSwitch(0, 315);
	else if (!digitalRead(BT_UP) && (!digitalRead(BT_LEFT))) Joystick.setHatSwitch(0, 45);
	else if (!digitalRead(BT_DOWN) && (!digitalRead(BT_RIGHT))) Joystick.setHatSwitch(0, 225);
	else if (!digitalRead(BT_DOWN) && (!digitalRead(BT_LEFT))) Joystick.setHatSwitch(0, 135);
	else if (!digitalRead(BT_DOWN)) Joystick.setHatSwitch(0,180);
	else if (!digitalRead(BT_LEFT)) Joystick.setHatSwitch(0, 90);
	else if (!digitalRead(BT_UP)) Joystick.setHatSwitch(0, 360);
	else if (!digitalRead(BT_RIGHT)) Joystick.setHatSwitch(0, 270);
	
	else Joystick.setHatSwitch(0, -1);

#pragma endregion	
	
	if (!digitalRead(BT_START)) Joystick.setButton(6, true);
	else Joystick.setButton(6, false);

	if (!digitalRead(BT_SELECT)) Joystick.setButton(7, 1);
	else Joystick.setButton(7, 0);

	if (!digitalRead(BT_START) && !digitalRead(BT_SELECT))
	{		

		sw = !sw;
		digitalWrite(LED, sw);
		for (size_t i = 0; i < 3; i++) angularRotation[i] = 0;
		
		delay(200);
	}

#pragma region Analogs

	//Joystick.setXAxis(map(analogRead(LA_X), 16, 1023, -127, 127));
	if(angularRotation[2] >= -90 && angularRotation[2] <= 90 ) Joystick.setXAxis(map(angularRotation[2], -90, 90, 127, -127));
	Joystick.setYAxis(map(analogRead(LA_Y), 16, 1023, 127, -127));
	Joystick.setZAxis(map(analogRead(RA_X), 16, 1023, -127, 127));
	Joystick.setRudder(map(analogRead(RA_Y), 16, 1023, 0, 255));
	//Joystick.setYAxisRotation(map(analogRead(RA_Y), 0, 1023, 0, 360));
	//Joystick.setZAxisRotation(a);
	//Joystick.setThrottle(a);

#pragma endregion
	
	Joystick.sendState();
	PrintGyro_details();
	
	delay(4);

}

void MouseMove()
{


}

int X, Y, Z;
int32_t varX=0, varY=0, varZ=0;
uint32_t index = 2;

void PrintGyro_simple()
{
	gyro.read();
	
	Serial.print((int)gyro.g.x);
	Serial.print(",");
	Serial.print((int)gyro.g.y);
	Serial.print(",");
	Serial.print((int)gyro.g.z);

	




}
void PrintGyro_details()
{
	gyro.read();


	Serial.print((int)gyro.g.x);
	Serial.print("\t");
	Serial.print((int)gyro.g.y);
	Serial.print("\t");
	Serial.print((int)gyro.g.z);
	Serial.print("\t");

	Serial.print(((int)gyro.g.x  + gyroOffest[0])*gyroScale[0]);
	Serial.print(",");
	Serial.print(((int)gyro.g.y + gyroOffest[1])*gyroScale[1]);
	Serial.print(",");
	Serial.print(((int)gyro.g.z + gyroOffest[2])*gyroScale[2]);

	/*Serial.print((int)gyro.g.x * gyroScale[0] + gyroOffest[0]);
	Serial.print(",");
	Serial.print((int)gyro.g.y * gyroScale[1] + gyroOffest[1]);
	Serial.print(",");
	Serial.println((int)gyro.g.z * gyroScale[2] + gyroOffest[2*/;

		
	varX += (int)gyro.g.x;
	varY += (int)gyro.g.y;
	varZ += (int)gyro.g.z;


	Serial.print((int)gyro.g.x);
	Serial.print("\t");
	Serial.print((int)gyro.g.y);
	Serial.print("\t");
	Serial.print((int)gyro.g.z);

	Serial.print("\t");
	
	Serial.print((float)varX/(float)index);
	Serial.print("\t");
	Serial.print((float)varY/(float)index);
	Serial.print("\t");
	Serial.print((float)varZ/(float)index);
	


	angularRotation[0] += (gyro.g.x - gyroOffest[0])*gyroScale[0];
	angularRotation[1] += (gyro.g.y - gyroOffest[1])*gyroScale[1];
	angularRotation[2] += (gyro.g.z - gyroOffest[2])*gyroScale[2];

	Serial.print("\t");
	Serial.print((X-gyroOffest[0])*gyroScale[0]);
	Serial.print("\t");
	Serial.print((Y-gyroOffest[1])*gyroScale[1]);
	Serial.print("\t");
	Serial.print((Z-gyroOffest[2])*gyroScale[2]);
	

	angularRotation[0] += (X - gyroOffest[0])*gyroScale[0];
	angularRotation[1] += (Y - gyroOffest[1])*gyroScale[1];
	angularRotation[2] += (Z - gyroOffest[2])*gyroScale[2];

	Serial.print("\t");
	Serial.print(angularRotation[0]);
	Serial.print("\t");
	Serial.print(angularRotation[1]);
	Serial.print("\t");
	Serial.print(angularRotation[2]);
	

	Serial.print("\t");
	Serial.print((String)index);
	Serial.print("\t");
	Serial.print(millis());
	Serial.println();

	index++;

}

void Blink(int8_t pin, int time)
{
	digitalWrite(pin, HIGH);
	delay(time);
	digitalWrite(pin, LOW);
}