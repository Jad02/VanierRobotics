#include <CrcLib.h>
#include <Encoder.h>
#include <Wire.h>
#include <SoftwareSerial.h>


#define DriveR CRC_PWM_1
#define DriveL CRC_PWM_2
#define Elev CRC_PWM_3

#define Car CRC_PWM_4
#define Sponge CRC_PWM_5
#define ElevWheels CRC_PWM_6

#define BallServo CRC_PWM_7
#define EngageBarServo CRC_PWM_8

using namespace Crc;

Encoder ElevEnco(CRC_ENCO_A, CRC_ENCO_B);
Encoder DriveLEnco(CRC_I2C_SDA,CRC_I2C_SCL);
Encoder DriveREnco(CRC_SERIAL_TXD1,CRC_SERIAL_RXD1);



//Program constants
const int Move1m = 385; 
const int Rot1_8 = 1102;
const int Rot1_4 = 2205;
const int Rot3_8 = 3308;
const int Rot1_2 = 4411;

const int Level0 = 0;
const int Level1 = 1;
const int Level2 = 2;
const int Level2_5 = 2;
const int Level3 = 3;
const int Level5 = 5;
const int Level6 = 6;
const int LevelMax = 9;

const unsigned long CarActive = 2000;
const unsigned long ElevWheelsActive = 2000;
const unsigned long SpongeReverse = 1000;
const unsigned long ElevWheelsReverse =1000;
const unsigned long BarServoActive = 500;
const unsigned long DropBall = 1000;

const int MaxSpeed = 127; 

int Step = 1;

void setup() {
  CrcLib::Initialize();
  Serial.begin(9600);  
  
  CrcLib::InitializePwmOutput(DriveR);
  CrcLib::InitializePwmOutput(DriveL);
  CrcLib::InitializePwmOutput(Elev);
  CrcLib::InitializePwmOutput(Car);
  CrcLib::InitializePwmOutput(Sponge);
  CrcLib::InitializePwmOutput(ElevWheels); 
  
  DriveREnco.write(0);
  DriveLEnco.write(0);
  ElevEnco.write(0);

}

void loop() {
  CrcLib::Update();
  Serial.println(ElevEnco.read());
  Serial.println(DriveREnco.read());
  Serial.println(DriveLEnco.read());
   
  while (Step <48)//sponge wall always on unitl step 
  {
    CrcLib::SetPwmOutput(Sponge,127);
  }
  
    while (Step == 1) //rotate CC 1/8
   {
    if(DriveREnco.read() < Rot1_8 && DriveLEnco.read() > -Rot1_8) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR, MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-MaxSpeed);
    }
    else if (DriveREnco.read() > Rot1_8 && DriveLEnco.read() < -Rot1_8) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL, 0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
      Step++;
    }
  }

  while (Step == 2) //move 4.1  forward
  {
    if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,MaxSpeed);
    }
    else if (DriveREnco.read() > 4.1*Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
      Step++;
    }
  }
  
  while (Step == 3) //Compress Car jack hold
  {
    unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, MaxSpeed);
      CrcLib::SetPwmOutput(Elev,-MaxSpeed); 
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CrcLib::SetPwmOutput(Elev,0); 
      ElevEnco.write(0);
      Step++;
      CurentTime = 0;
    }
  }

  while (Step == 4) // Level 1 elevator
  {
    if(ElevEnco.read() < Level1) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, MaxSpeed); 
    }
    else if (ElevEnco.read() > Level1) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev,-0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Elev,0);
      Step++;
    }
  }

  while (Step == 5)// Rotate elevator wheels 
  {
    unsigned long  CurentTime = millis();
    if (CurentTime < ElevWheelsActive)
    {
      CrcLib::SetPwmOutput(ElevWheels, MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(ElevWheels, 0);
      CurentTime = 0;
      Step++;
    }
  }

  while (Step == 6) //uncompress car and level 0
  {
    if(ElevEnco.read() > Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, -MaxSpeed); 
    }
    else if (ElevEnco.read() < Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, 0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Elev,0);
    }
    unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, -MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CurentTime = 0;
      Step++;
    }
  }

  while (Step == 7) // Forward 4.4
  {
  if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,MaxSpeed);
    }
    else if (DriveREnco.read() > 4.1*Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
      Step++;
    }
  }

  while (Step ==8) // Compress Car and hold
  {
    unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, MaxSpeed);
      CrcLib::SetPwmOutput(Elev,-MaxSpeed); 
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CrcLib::SetPwmOutput(Elev,0); 
      ElevEnco.write(0);
      Step++;
      CurentTime = 0;
    }
  }

  while (Step == 9) // Move back 4.2, level 5 
  {
  if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-MaxSpeed);
    }
    else if (DriveREnco.read() > Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
    }

    if(ElevEnco.read() < Level5) //Encoder position not yet reached
      {
        CrcLib::SetPwmOutput(Elev, MaxSpeed); 
      }
    else if (ElevEnco.read() > Level5) //Encoder position not yet reached
      {
        CrcLib::SetPwmOutput(Elev,-0.5*MaxSpeed);
      }
    else 
      {
        CrcLib::SetPwmOutput(Elev,0);
        Step++;
      }
    }

     while (Step == 10) //rotate CC 1/4 and elevatorwheels on 
     {
    if(DriveREnco.read() < Rot1_4 && DriveLEnco.read() > -Rot1_4) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR, MaxSpeed); 
      CrcLib::SetPwmOutput(DriveL,-MaxSpeed);
    }
    else if (DriveREnco.read() > Rot1_4 && DriveLEnco.read() < -Rot1_4) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL, 0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
    }
    unsigned long  CurentTime = millis();
    if (CurentTime < ElevWheelsActive)
    {
      CrcLib::SetPwmOutput(ElevWheels, MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(ElevWheels, 0);
      Step++;
      CurentTime = 0;
    }
  }

  while (Step == 11)// uncompress Car Level 0
  {
    if(ElevEnco.read() > Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, -MaxSpeed); 
    }
    else if (ElevEnco.read() < Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, 0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Elev,0);
    }
    unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, -MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      Step++;
      CurentTime = 0;
    }
  }

  while (Step == 12)// Forward 2.1
  {
  if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,MaxSpeed);
    }
    else if (DriveREnco.read() > 4.1*Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
      Step++;
    }
  }

  while (Step == 13) //Compress Car and hold
  {
     unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, MaxSpeed);
      CrcLib::SetPwmOutput(Elev,-MaxSpeed); 
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CrcLib::SetPwmOutput(Elev,0); 
      ElevEnco.write(0);
      Step++;
      CurentTime = 0;
    }
  }

  while (Step == 14) // Level 2.5 elevator
  {
    if(ElevEnco.read() < Level2_5) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, MaxSpeed); 
    }
    else if (ElevEnco.read() > Level2_5) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev,-0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Elev,0);
      Step++;
    }
  }

  while (Step == 15)// Rotate elevator wheels 
  {
    unsigned long  CurentTime = millis();
    if (CurentTime < ElevWheelsActive)
    {
      CrcLib::SetPwmOutput(ElevWheels, MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(ElevWheels, 0);
      Step++;
      CurentTime = 0;
    }
  }

  while (Step == 16)// uncompress Car Level 0
  {
    if(ElevEnco.read() > Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, -MaxSpeed); 
    }
    else if (ElevEnco.read() < Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, 0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Elev,0);
    }
    unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, -MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CurentTime = 0;
      Step++;
    }
  }

  while (Step == 17)// Forward 2.3
  {
  if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,MaxSpeed);
    }
    else if (DriveREnco.read() > Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
      Step++;
    }
  }

 while (Step == 18) //Compress Car and hold
  {
     unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, MaxSpeed);
      CrcLib::SetPwmOutput(Elev,-MaxSpeed); 
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CrcLib::SetPwmOutput(Elev,0); 
      ElevEnco.write(0);
      Step++;
      CurentTime = 0;
    }
  }

    while (Step == 20) // Move back 2.2, level 1
  {
  if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-MaxSpeed);
    }
    else if (DriveREnco.read() > Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
    }

    if(ElevEnco.read() < Level1) //Encoder position not yet reached
      {
        CrcLib::SetPwmOutput(Elev, MaxSpeed); 
      }
    else if (ElevEnco.read() > Level1) //Encoder position not yet reached
      {
        CrcLib::SetPwmOutput(Elev,-0.5*MaxSpeed);
      }
    else 
      {
        CrcLib::SetPwmOutput(Elev,0);
        Step++;
      }
    }

 while (Step == 20) //rotate CC 1/8 and elevatorwheels on 
     {
    if(DriveREnco.read() < Rot1_8 && DriveLEnco.read() > -Rot1_8) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR, MaxSpeed); 
      CrcLib::SetPwmOutput(DriveL,-MaxSpeed);
    }
    else if (DriveREnco.read() > Rot1_8 && DriveLEnco.read() < -Rot1_8) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL, 0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
    }
     unsigned long  CurentTime = millis();
      if (CurentTime < ElevWheelsActive)
      {
        CrcLib::SetPwmOutput(ElevWheels, MaxSpeed);
      }
      else 
      {
        CrcLib::SetPwmOutput(ElevWheels, 0);
        CurentTime = 0;
        Step++;
      }
    }

  while (Step == 21)// uncompress Car Level 0
  {
    if(ElevEnco.read() > Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, -MaxSpeed); 
    }
    else if (ElevEnco.read() < Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, 0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Elev,0);
    }
    unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, -MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CurentTime = 0;
      Step++;
    }
  }

 while (Step == 22)// Forward 1.1
  {
  if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,MaxSpeed);
    }
    else if (DriveREnco.read() > Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
      Step++;
    }
  }

  while (Step == 23) //Compress Car and hold
  {
     unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, MaxSpeed);
      CrcLib::SetPwmOutput(Elev,-MaxSpeed); 
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CrcLib::SetPwmOutput(Elev,0); 
      ElevEnco.write(0);
      Step++;
      CurentTime = 0;
    }
  }

  while (Step == 24) // Move back 1.1, up level 1
  {
  if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-MaxSpeed);
    }
    else if (DriveREnco.read() > Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
    }

    if(ElevEnco.read() < Level1) //Encoder position not yet reached
      {
        CrcLib::SetPwmOutput(Elev, MaxSpeed); 
      }
    else if (ElevEnco.read() > Level1) //Encoder position not yet reached
      {
        CrcLib::SetPwmOutput(Elev,-0.5*MaxSpeed);
      }
    else 
      {
        CrcLib::SetPwmOutput(Elev,0);
        Step++;
      }
    }
    
   while (Step == 25) //rotate C 3/8 and elevatorwheels on 
     {
    if(DriveREnco.read() > -Rot3_8 && DriveLEnco.read() < Rot3_8) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-MaxSpeed); 
      CrcLib::SetPwmOutput(DriveL, MaxSpeed);
    }
    else if (DriveREnco.read() < -Rot3_8 && DriveLEnco.read() > Rot3_8) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR, 0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
    }
     unsigned long  CurentTime = millis();
      if (CurentTime < ElevWheelsActive)
      {
        CrcLib::SetPwmOutput(ElevWheels, MaxSpeed);
      }
      else 
      {
        CrcLib::SetPwmOutput(ElevWheels, 0);
        CurentTime = 0;
        Step++;
      }
    }

  while (Step == 26)// uncompress Car and Level 0
  {
    if(ElevEnco.read() > Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, -MaxSpeed); 
    }
    else if (ElevEnco.read() < Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, 0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Elev,0);
    }
    unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, -MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CurentTime = 0;
      Step++;
    }
  }

  while (Step == 27)// Forward 4.3
  {
  if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,MaxSpeed);
    }
    else if (DriveREnco.read() > Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
      Step++;
    }
  }

  while (Step == 28) //Compress Car and hold
  {
     unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, MaxSpeed);
      CrcLib::SetPwmOutput(Elev,-MaxSpeed); 
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CrcLib::SetPwmOutput(Elev,0); 
      ElevEnco.write(0);
      Step++;
      CurentTime = 0;
    }
  }

  while (Step == 29) // Move back 4.3, up level 1
  {
  if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-MaxSpeed);
    }
    else if (DriveREnco.read() > Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
    }

    if(ElevEnco.read() < Level1) //Encoder position not yet reached
      {
        CrcLib::SetPwmOutput(Elev, MaxSpeed); 
      }
    else if (ElevEnco.read() > Level1) //Encoder position not yet reached
      {
        CrcLib::SetPwmOutput(Elev,-0.5*MaxSpeed);
      }
    else 
      {
        CrcLib::SetPwmOutput(Elev,0);
        Step++;
      }
    }

     while (Step == 30) //rotate CC 3/8 and elevatorwheels on 
     {
    if(DriveREnco.read() < Rot3_8 && DriveLEnco.read() > -Rot3_8) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR, MaxSpeed); 
      CrcLib::SetPwmOutput(DriveL,-MaxSpeed);
    }
    else if (DriveREnco.read() > Rot3_8 && DriveLEnco.read() < -Rot3_8) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL, 0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
    }
     unsigned long  CurentTime = millis();
      if (CurentTime < ElevWheelsActive)
      {
        CrcLib::SetPwmOutput(ElevWheels, MaxSpeed);
      }
      else 
      {
        CrcLib::SetPwmOutput(ElevWheels, 0);
        CurentTime = 0;
        Step++;
      }
    }

    while (Step == 31)// uncompress Car and Level 0
  {
    if(ElevEnco.read() > Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, -MaxSpeed); 
    }
    else if (ElevEnco.read() < Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, 0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Elev,0);
    }
    unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, -MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CurentTime = 0;
      Step++;
    }
  }

  while (Step == 32)// Forward 1.2
  {
  if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,MaxSpeed);
    }
    else if (DriveREnco.read() > Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
      Step++;
    }
  }   

  while (Step == 33) //Compress Car and hold
  {
     unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, MaxSpeed);
      CrcLib::SetPwmOutput(Elev,-MaxSpeed); 
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CrcLib::SetPwmOutput(Elev,0); 
      ElevEnco.write(0);
      Step++;
      CurentTime = 0;
    }
  }

  while (Step == 34) // Move back 1.2, up level 2
  {
  if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-MaxSpeed);
    }
    else if (DriveREnco.read() > Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
    }

    if(ElevEnco.read() < Level2) //Encoder position not yet reached
      {
        CrcLib::SetPwmOutput(Elev, MaxSpeed); 
      }
    else if (ElevEnco.read() > Level2) //Encoder position not yet reached
      {
        CrcLib::SetPwmOutput(Elev,-0.5*MaxSpeed);
      }
    else 
      {
        CrcLib::SetPwmOutput(Elev,0);
        Step++;
      }
    }

     while (Step == 35) //rotate C 1/4 and elevatorwheels on 
     {
    if(DriveREnco.read() > -Rot1_4 && DriveLEnco.read() < Rot1_4) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-MaxSpeed); 
      CrcLib::SetPwmOutput(DriveL, MaxSpeed);
    }
    else if (DriveREnco.read() < -Rot1_4 && DriveLEnco.read() > Rot1_4) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR, 0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
    }
     unsigned long  CurentTime = millis();
      if (CurentTime < ElevWheelsActive)
      {
        CrcLib::SetPwmOutput(ElevWheels, MaxSpeed);
      }
      else 
      {
        CrcLib::SetPwmOutput(ElevWheels, 0);
        CurentTime = 0;
        Step++;
      }
    }
  
  while (Step == 36)// uncompress Car and go Level 0
  {
    if(ElevEnco.read() > Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, -MaxSpeed); 
    }
    else if (ElevEnco.read() < Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, 0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Elev,0);
    }
    unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, -MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CurentTime = 0;
      Step++;
    }
  }

  while (Step == 37)// Forward 3.1
  {
  if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,MaxSpeed);
    }
    else if (DriveREnco.read() > Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
      Step++;
    }
  }   

  while (Step == 38) //Compress Car and hold
  {
     unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, MaxSpeed);
      CrcLib::SetPwmOutput(Elev,-MaxSpeed); 
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CrcLib::SetPwmOutput(Elev,0); 
      ElevEnco.write(0);
      Step++;
      CurentTime = 0;
    }
  }

  while (Step == 39) // up level 3
  {
    if(ElevEnco.read() < Level3) //Encoder position not yet reached
      {
        CrcLib::SetPwmOutput(Elev, MaxSpeed); 
      }
    else if (ElevEnco.read() > Level3) //Encoder position not yet reached
      {
        CrcLib::SetPwmOutput(Elev,-0.5*MaxSpeed);
      }
    else 
      {
        CrcLib::SetPwmOutput(Elev,0);
        Step++;
      }
    }

     while (Step == 40)// elevatorwheels on 
     {
     unsigned long  CurentTime = millis();
      if (CurentTime < ElevWheelsActive)
      {
        CrcLib::SetPwmOutput(ElevWheels, MaxSpeed);
      }
      else 
      {
        CrcLib::SetPwmOutput(ElevWheels, 0);
        CurentTime = 0;
        Step++;
      }
    }
  
  while (Step == 41)// uncompress Car and go Level 0
  {
    if(ElevEnco.read() > Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, -MaxSpeed); 
    }
    else if (ElevEnco.read() < Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, 0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Elev,0);
    }
    unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, -MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CurentTime = 0;
      Step++;
    }
  }

  while (Step == 42)// Forward 3.3
  {
  if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,MaxSpeed);
    }
    else if (DriveREnco.read() > Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
      Step++;
    }
  }   

  while (Step == 43) //Compress Car and hold
  {
     unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, MaxSpeed);
      CrcLib::SetPwmOutput(Elev,-MaxSpeed); 
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CrcLib::SetPwmOutput(Elev,0); 
      ElevEnco.write(0);
      Step++;
      CurentTime = 0;
    }
  }

  while (Step == 44) // Move back 3.2, up level 6
  {
  if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-MaxSpeed);
    }
    else if (DriveREnco.read() > Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
    }

    if(ElevEnco.read() < Level6) //Encoder position not yet reached
      {
        CrcLib::SetPwmOutput(Elev, MaxSpeed); 
      }
    else if (ElevEnco.read() > Level6) //Encoder position not yet reached
      {
        CrcLib::SetPwmOutput(Elev,-0.5*MaxSpeed);
      }
    else 
      {
        CrcLib::SetPwmOutput(Elev,0);
        Step++;
      }
    }

     while (Step == 45) //rotate C 1/2 and elevatorwheels on 
     {
    if(DriveREnco.read() > -Rot1_2 && DriveLEnco.read() < Rot1_2) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-MaxSpeed); 
      CrcLib::SetPwmOutput(DriveL, MaxSpeed);
    }
    else if (DriveREnco.read() < -Rot1_2 && DriveLEnco.read() > Rot1_2) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR, 0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
    }
     unsigned long  CurentTime = millis();
      if (CurentTime < ElevWheelsActive)
      {
        CrcLib::SetPwmOutput(ElevWheels, MaxSpeed);
      }
      else 
      {
        CrcLib::SetPwmOutput(ElevWheels, 0);
        CurentTime = 0;
        Step++;
      }
    }
  
  while (Step == 46)// uncompress Car and go Level 0
  {
    if(ElevEnco.read() > Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, -MaxSpeed); 
    }
    else if (ElevEnco.read() < Level0) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, 0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Elev,0);
    }
    unsigned long  CurentTime = millis();
    if (CurentTime < CarActive)
    {
      CrcLib::SetPwmOutput(Car, -MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Car, 0);
      CurentTime = 0;
      Step++;
    }
  }

 while (Step == 47)// Forward 6
  {
  if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,MaxSpeed);
    }
    else if (DriveREnco.read() > Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
      Step++;
    }
  }   

   while (Step == 48) //rotate CC 1/4  
     {
       if(DriveREnco.read() < Rot1_4 && DriveLEnco.read() > -Rot1_4) //Encoder position not yet reached
        {
          CrcLib::SetPwmOutput(DriveR, MaxSpeed); 
          CrcLib::SetPwmOutput(DriveL,-MaxSpeed);
        }
        else if (DriveREnco.read() > Rot1_4 && DriveLEnco.read() < -Rot1_4) //Encoder position not yet reached
        {
          CrcLib::SetPwmOutput(DriveR, 0.5*MaxSpeed);
          CrcLib::SetPwmOutput(DriveL,-0.5*MaxSpeed);
        }
        else 
        {
          CrcLib::SetPwmOutput(DriveR,0);
          CrcLib::SetPwmOutput(DriveL,0);
          DriveREnco.write(0);
          DriveLEnco.write(0);
          Step++;
        }
     }

  while (Step = 49)//sponge wall reverse
  {
    unsigned long  CurentTime = millis();
    if (CurentTime < SpongeReverse)
    {
      CrcLib::SetPwmOutput(Sponge,-MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Sponge,0);
      CurentTime = 0;
      Step++;
    }
    
  }

  while (Step == 50) //Elevator Wheels reverse 
  {
    unsigned long  CurentTime = millis();
      if (CurentTime < ElevWheelsReverse)
      {
        CrcLib::SetPwmOutput(ElevWheels, -MaxSpeed);
      }
      else 
      {
        CrcLib::SetPwmOutput(ElevWheels, 0);
        CurentTime = 0;
        Step++;
      }
  }

   while (Step == 51)// Preare Support Bar
   {
    if(ElevEnco.read() < Level6) //not yet decided 
    {
      CrcLib::SetPwmOutput(Elev, MaxSpeed); 
    }
    else if (ElevEnco.read() > Level6) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, -0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Elev,0);
      ElevEnco.write(0);
      Step++;
    }
   }

    while (Step == 52) // servo to engage support bar
    {
      unsigned long  CurentTime = millis();
        if (CurentTime < BarServoActive)
        {
          CrcLib::SetPwmOutput(EngageBarServo, MaxSpeed);
        }
        else 
        {
          CrcLib::SetPwmOutput(EngageBarServo, 0);
          CurentTime = 0;
          Step++;
        }
    }

   while (Step == 53)// Engage Support Bar
   {
    if(ElevEnco.read() < LevelMax)
    {
      CrcLib::SetPwmOutput(Elev, MaxSpeed); 
    }
    else if (ElevEnco.read() > LevelMax) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(Elev, -0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(Elev,0);
      Step++;
    } 
   }
   
    
while (Step == 54)// Forward 7
  {
  if(DriveREnco.read() < Move1m && DriveLEnco.read() < Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,MaxSpeed);
    }
    else if (DriveREnco.read() > Move1m && DriveLEnco.read() > Move1m) //Encoder position not yet reached
    {
      CrcLib::SetPwmOutput(DriveR,-0.5*MaxSpeed);
      CrcLib::SetPwmOutput(DriveL,-0.5*MaxSpeed);
    }
    else 
    {
      CrcLib::SetPwmOutput(DriveR,0);
      CrcLib::SetPwmOutput(DriveL,0);
      DriveREnco.write(0);
      DriveLEnco.write(0);
      Step++;
    }
  }   

    while (Step == 55) // drop ball
    {
      unsigned long  CurentTime = millis();
        if (CurentTime < DropBall)
        {
          CrcLib::SetPwmOutput(BallServo, MaxSpeed);
        }
        else 
        {
          CrcLib::SetPwmOutput(BallServo, 0);
          CurentTime = 0;
          Step++;
        }
    }

  }
  
//  if(ElevEnco.read() <= STOP_POSITION) //Encoder position not yet reached
//  {
//    CrcLib::SetPwmOutput(Elev,MaxSpeed);
//  }
//  else if (ElevEnco.read() >= STOP_POSITION) //Encoder position not yet reached
//  {
//    CrcLib::SetPwmOutput(Elev,-MaxSpeed);
//  }
//    else 
//  {
//    CrcLib::SetPwmOutput(Elev,0);
//  }
   
 
