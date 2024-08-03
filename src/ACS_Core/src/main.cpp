#include "IO/MelodyPlayer.h"
#include "IO/LCD.h"
#include "IO/Lock.h"
#include "ActionHandler.h"

void setup()
{
  Serial.begin(921600);
  LCD::Initialize();
  MelodyPlayer::Initialize();
  Lock::Initialize();
  ActionHandler::Initialize();

}

bool oldState = false;

void loop()
{
}