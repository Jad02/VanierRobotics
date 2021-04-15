/* Encoder Library - TwoKnobs Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <CrcLib.h>
#include <Encoder.h>
using namespace CrcLib;
// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder motor1Enco(CRC_ENCO_A, CRC_ENCO_B);

void setup()
{
	Serial.begin(9600);
	Serial.println("Encoder Test:");
}

long positionLeft = 0;

void loop()
{
	long enc;
	enc = motor1Enco.read();
	Serial.print(enc);
	Serial.println();
	// if a character is sent from the serial monitor,
	// reset both back to zero.
	if (Serial.available())
	{
		Serial.read();
		Serial.println("Reset both knobs to zero");
		motor1Enco.write(0);
	}
}
