#include "IO/MelodyPlayer.h"
#include "IO/LCD.h"
#include "IO/Lock.h"
#include "ActionHandler.h"
#include "FrontComm.h"
#include "DataSaving.h"
#include "Network/NetworkMain.h"

void setup()
{
  Serial.begin(921600);
  Serial.println("Serial Begin");

  LCD::Initialize();

  if(!DataSaving::Initialize())
  {
    LCD::Print("DataSaving failed to initialize");
    delay(5000);
    ESP.restart();
    return;
  }

  if(!NetworkMain::Initialize())
  {
    LCD::Print("NetworkMain failed to initialize");
    delay(5000);
    ESP.restart();
    return;
  }



  FrontComm::Initialize();
  MelodyPlayer::Initialize();
  Lock::Initialize();
  ActionHandler::Initialize();

}

void loop()
{
  
}