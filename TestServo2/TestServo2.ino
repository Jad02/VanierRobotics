#include <CrcLib.h>
using namespace Crc;

float voltageValue; 

void setup() {
  CrcLib::Initialize(); 

  Serial.begin(9600);
  
  CrcLib::InitializePwmOutput(CRC_PWM_8);
  CrcLib::InitializePwmOutput(CRC_PWM_5);
 }
 
void loop() { 
  CrcLib::Update(); //Refresh
  
  CrcLib::SetPwmOutput(CRC_PWM_8, -10); 
  CrcLib::SetPwmOutput(CRC_PWM_5, 127);

  voltageValue=CrcLib::GetBatteryVoltage();
  Serial.println(voltageValue);



}
