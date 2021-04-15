#include <CrcLib.h>

using namespace Crc;

void setup()
{
    CrcLib::Initialize();
}

void loop()
{
    CrcLib::Update();
}

//Lines starting with 2 slashes won't be compiled; they are comments.

/*This is a way 
to comment multiple lines of code */
