#include "IO/CardReader.h"
#include "IO/MelodyPlayer.h"
#include "IO/Keypad.h"
#include "IO/LCD.h"

Melody m1;

void setup()
{
  Serial.begin(921600);
  LCD::Initialize();
  Keypad::Initialize();
  CardReader::Initialize();
  MelodyPlayer::Initialize();
}

void loop()
{
  
}