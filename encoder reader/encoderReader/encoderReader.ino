/* Encoder Library - TwoKnobs Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <CrcLib.h>
#include <Encoder.h>

using namespace Crc;

class EnMotor
{
public:
	int target;
	int speed;
	int MAXSPEED;
	int pinPWM;
	const int TURN = 5281;
	EnMotor(int pinA, int pinB, int pwm, int maxspeed)
	{
		Encoder enc(pinA, pinB);
		MAXSPEED = maxspeed;
		CrcLib::InitializePwmOutput(pwm);
		pinPWM = pwm;
		enc.write(0);
	}
	long read()
	{
		return enc.read();
	}
	void setSpeed()
	{
		if (enc.read() < target - 2 * MAXSPEED)
		{
			speed = MAXSPEED;
		}
		else if (enc.read() > target + 2 * MAXSPEED)
		{
			speed = -1 * MAXSPEED;
		}
		else
		{
			speed = 0;
		}
	}
	void turn(int count, int dir)
	{
		long encVal;
		encVal = enc.read();
		int d = (dir > 0) ? 1 : -1;
		target = encVal + d * count * TURN;
	}
	void update()
	{
		setSpeed();
		CrcLib::SetPwmOutput(pinPWM, speed);
		// Serial.print("Encoder Value: ");
		// Serial.print(enc.read());
		// int speed = setSpeed();
		// Serial.print("; Motor Speed: ");
		// Serial.print(speed);
	}
};

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

EnMotor motor1(CRC_ENCO_A, CRC_ENCO_B, CRC_PWM_1, 50);
int dir = 1;
void setup()
{
	Serial.begin(9600);
	Serial.println("Encoder Test:");
}
long positionLeft = 0;
void loop()
{
	CrcLib::Update();
	motor1.update();
	Serial.println();
	if (Serial.available())
	{
		Serial.println("Reset both knobs to zero");
		// motor1Enco.write(0);
		motor1.turn(2, dir);
		dir = -1 * dir;
	}
}
