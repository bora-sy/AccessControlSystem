#include "IO/MelodyPlayer.h"
#include "IO/LCD.h"


void setup()
{
  Serial.begin(921600);
  LCD::Initialize();
  MelodyPlayer::Initialize();
}

void loop()
{
  
}