#include "IO/MelodyPlayer.h"
#include "IO/LCD.h"
#include "IO/Lock.h"
#include "ActionHandler.h"
#include "FrontComm.h"

void setup()
{
  Serial.begin(921600);
  LCD::Initialize();
  FrontComm::Initialize();
  MelodyPlayer::Initialize();
  Lock::Initialize();
  ActionHandler::Initialize();

}

void loop()
{
  
}