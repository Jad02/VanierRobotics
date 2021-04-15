/* Encoder Library - TwoKnobs Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */
#define MC_1 CRC_PWM_1
#include <CrcLib.h>
#include <Encoder.h>
using namespace CrcLib;
// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder motor1Enco(CRC_ENCO_A, CRC_ENCO_B);

bool motorRotating = LOW;
const int STOP_POSITION = 1994;
const int MOTOR_SPEED = 50; //127*0.10=13
void setup()
{
	Serial.begin(9600);
	Serial.println("Encoder Test:");
	CrcLib::InitializePwmOutput(MC_1);
	motor1Enco.write(0);
}

long positionLeft = 0;

void loop()
{
	CrcLib::Update();
	long enc;
	enc = motor1Enco.read();
	Serial.print("Encoder Value: ")
	Serial.print(enc);
	int speed;
	// if a character is sent from the serial monitor,
	// reset both back to zero.
	if (motor1Enco.read() < STOP_POSITION - MOTOR_SPEED)
	{
		speed = MOTOR_SPEED;
		
	}
	else if (motor1Enco.read() > STOP_POSITION + MOTOR_SPEED)
	{
		speed = -1 * MOTOR_SPEED;
	}
	else
	{
		speed = 0;
	}
	Serial.print("; Motor Speed: ")
	Serial.print(speed);
	CrcLib::SetPwmOutput(MC_1, speed);
	
	Serial.println();
	if (Serial.available())
	{
		Serial.read();
		
		Serial.println("Reset both knobs to zero");
		motor1Enco.write(0);
	}
}
