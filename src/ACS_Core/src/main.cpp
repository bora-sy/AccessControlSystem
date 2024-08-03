#include "IO/MelodyPlayer.h"
#include "IO/LCD.h"
#include "IO/Lock.h"


void setup()
{
  Serial.begin(921600);
  LCD::Initialize();
  MelodyPlayer::Initialize();
  Lock::Initialize();
}

void loop()
{
  
}