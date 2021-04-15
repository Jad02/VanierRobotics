#include <Servo.h>

int servopin = 1;
Servo myservo; // Using this is like giving the servo a name

int pos = 0;

void setup()
{
	Serial.begin(9600);
	myservo.attach(servopin); //attaches servo on pin 9 to the seervo object
}

void loop()
{

	for (pos = 0; pos <= 180; pos += 1)
	{ //from 0˚ to 180˚ in steps of 1˚

		myservo.write(pos); //tell the servo to change its position according to the variable pos
		delay(15);			// waits 15ms for the servo to reach the position
	}

	for (pos = 180; pos >= 0; pos -= 1)
	{
		myservo.write(pos);
		delay(15);
	}
}
