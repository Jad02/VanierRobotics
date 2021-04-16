#include <Encoder.h>
class Motor
{
public:
	Encoder enc;
	int target;
	int speed;
	int MAXSPEED;
	int pinPWM;
	const int TURN = 5281;
	Motor(int pinA, int pinB, int pwm, int maxspeed)
	{
		enc = Encoder(pinA, pinB);
		MAXSPEED = maxspeed;
		CrcLib::InitializePwmOutput(pwm);
		pinPWM = pwn;
		enc.write(0);
	}
	long read(){
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
		encVal = motor1Enco.read();
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
}