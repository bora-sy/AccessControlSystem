#include "IO/CardReader.h"
#include "IO/MelodyPlayer.h"
#include "IO/Keypad.h"


Melody m1;

void setup()
{
  Serial.begin(921600);
  CardReader::Initialize();
  MelodyPlayer::Initialize();
  Keypad::Initialize();

}

void loop()
{
  char key = Keypad::GetKey();
}