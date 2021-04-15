//**************************************************************************************************************************
// CrcDuino Tutorial 4 - Basic power motor control using an encoder
// Inspired by: https://www.norwegiancreations.com/2017/09/arduino-tutorial-using-millis-instead-of-delay/ 
//      and     https://www.norwegiancreations.com/2018/10/arduino-tutorial-avoiding-the-overflow-issue-when-using-millis-and-micros/
// The code below uses the millis() native Arduino function, a command that returns the number of milliseconds since the board started running its current sketch, to measure elapsed time.
// NOTE:  To simplify code and focus on the encoder use, this version of the code does not cover the case where the BTN_RUN is still pressed when the encoder reaches its position.
//**************************************************************************************************************************

#include <CrcLib.h>

//"Encoder" library by Paul Stoffregen must first be installed via "Arduino Library Manager")
//  How-to-use https://www.pjrc.com/teensy/td_libs_Encoder.html
//#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

//Renaming pins with significant names
#define MC_1 CRC_PWM_1

using namespace CrcLib;

//Motor1 encoder variable: "Encoder" library pre-requisite
Encoder motor1Enco(CRC_ENCO_A, CRC_ENCO_B); //This encoder is plugged into these 2 pins on the board

//Program execution variable
bool motorRotating = LOW; // will allow Motor_1 rotation

//Program constants
const int STOP_POSITION = 1994; //Found by trial and error to correspond to a 360deg rotation on my setup. Some motor (like the Bilda) have this information in their specs sheets
const int MOTOR_SPEED = 13; //127*0.10=13
//**************************************************************************************************************************

void setup() {
  CrcLib::Initialize();
  Serial.begin(9600);  //High baud rate for faster printing on screen

  CrcLib::InitializePwmOutput(MC_1); //Use CrcLib::InitializePwmOutput(MC_1,1); to rapidly reverse this motor rotation direction all over the code
  
  //At power-up, establish a known encoder position value
  // Best practice would be to instead assign a physical limit switch indicating the zero position, and have the robot search
  // that position itself at sequence start in the void loop() part. This is called "homing".
  motor1Enco.write(0);
}

void loop() {
  CrcLib::Update();
  Serial.println(motor1Enco.read()); //Continuously print on screen the motor position
  
  if(CrcLib::GetDigitalInput(true) == 0)  //Start rotation when button is pressed
  {
    motorRotating = HIGH;
  }
  
  if(motorRotating == HIGH && (motor1Enco.read() <= STOP_POSITION) ) //Encoder position not yet reached
  {
    CrcLib::SetPwmOutput(MC_1,MOTOR_SPEED);
  }
  else
  {
    CrcLib::SetPwmOutput(MC_1,0);
  }

}
 
