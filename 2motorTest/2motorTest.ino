

#include <CrcLib.h>
#include <Encoder.h>

using namespace Crc;

Encoder enc (CRC_ENCO_A, CRC_ENCO_B);
int encPin = 1;
const int Rot1 = 5281;
const int MaxSpeed;
int target = 2;

void setup() {
  // put your setup code here, to run once:

    CrcLib::Initialize(); 
    Serial.begin(9600);

    CrcLib::InitializePwmOutput(encPin);
   
    CrcLib::InitializePwmOutput(CRC_PWM_5);

}

void loop() {
  // put your main code here, to run repeatedly:
  CrcLib::Update();

  if (enc.read() < target*Rot1)
    {
      int speed = MaxSpeed;
      CrcLib::SetPwmOutput (encPin, speed);
    }
    else if (enc.read() > target*Rot1)
    {
      speed = -1 * MaxSpeed;
      CrcLib::SetPwmOutput (encPin, speed);
    }
    else
    {
      speed = 0;
      CrcLib::SetPwmOutput (encPin, speed);
    }
    
  CrcLib::SetPwmOutput(CRC_PWM_5,127);
  
}
