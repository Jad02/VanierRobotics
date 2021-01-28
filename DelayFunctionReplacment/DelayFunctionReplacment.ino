#include <CrcLib.h>  //CrcLib program requirement
//Rename IOs with meaningful name (good practice)
#define SERVO_STAND_1 CRC_PWM_6
#define BTN_RUN CRC_DIG_1

//Declare program variables (value will change during execution)
// *Generally, you should use "unsigned long" for variables that hold time
// *The value will quickly become too large for an int to store
unsigned long time1Capture = 0;  //will store last time the time was recorded
bool btnHasBeenPressed = LOW; //tells us we pressed the button or not

//Declare program constants (value can't change during execution)
const unsigned long delay1 = 5000;           // time for which we send the servo command (milliseconds)

using namespace Crc; //CrcLib program requirement
//**************************************************************************************************************************
void setup() {
  // put your setup code here, to run once at the beginning:
  CrcLib::Initialize(); //Sets up the CrcDuino
  
  CrcLib::InitializePwmOutput(SERVO_STAND_1, 500, 2500); //Sets up the motor output, we specify the min and max pulse lenght as mentioned by the servo manufacturer
  CrcLib::SetDigitalPinMode(BTN_RUN, INPUT); //Sets up the digital port as an Input

  Serial.begin(9600);
}
//**************************************************************************************************************************
void loop() {
  // put your main code here, to run repeatedly:
  CrcLib::Update(); //Refreshes the CrcDuino

  if ( !(CrcLib::GetDigitalInput(BTN_RUN) == HIGH ) ) //When the button is pressed (GND applied to the input)
  {
    btnHasBeenPressed = HIGH; //we want to remember it happened, so that we go on even if released
  }

  if(btnHasBeenPressed == HIGH )  //If the button has been pressed
  {
    //we don't want to update time1Capture so that our timer loop below is monitored
    CrcLib::SetPwmOutput(SERVO_STAND_1, -64);  //But we want to send the servo to the desired position
  }
  else  //If the button has NOT been pressed
  {
    time1Capture = millis(); //we want to update time1Capture if the button has not been pressed, so that we don't enter the loop below...
    CrcLib::SetPwmOutput(SERVO_STAND_1, 0);  //we reset the position command
  }

  // This loop will be entered when more than "delay1" ms will have elapsed since the last time the "time1Capture" value has been updated
  // If the button has not been pressed, we continuously update time1Capture, so we will never enter the loop...
  // If the button has been pressed, we've stopped continuously updating time1Capture, so we will someday enter the loop!
  if(millis() - time1Capture > delay1)
  {
        CrcLib::SetPwmOutput(SERVO_STAND_1, 0);  //we reset the position command
        btnHasBeenPressed = LOW; //
  }

  
}
//**************************************************************************************************************************
