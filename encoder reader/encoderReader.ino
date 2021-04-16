/* Encoder Library - TwoKnobs Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <CrcLib.h>
#include <Encoder.h>
#include <Motor.h>
using namespace CrcLib;
// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

Motor motor1(CRC_ENCO_A, CRC_ENCO_B, CRC_PWM_1, 50);
int dir = 1;
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
