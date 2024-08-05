#include "IO/MelodyPlayer.h"
#include "IO/LCD.h"
#include "IO/Lock.h"
#include "ActionHandler.h"
#include "FrontComm.h"
#include "DataSaving.h"
#include "Network/NetworkMain.h"
#include "Network/OTA.h"
#include "Network/WebServer.h"

void InitCritical(bool (*initFunc)(), const char* loadingText, const char* errorText)
{
  LCD::PrintCenter(loadingText);

  bool suc = initFunc();
  if (!suc)
  {
    LCD::PrintCenter(errorText);
    delay(3000);
    LCD::PrintCenter("Restarting");
    delay(1500);
    ESP.restart();
  }
}

void setup()
{
  Serial.begin(921600);
  Serial.println("Serial Begin");

  LCD::Initialize();

  InitCritical(DataSaving::Initialize, "Initializing DataSaving", "DataSaving failed to initialize");
  InitCritical(NetworkMain::Initialize, "Initializing NetworkMain", "NetworkMain failed to initialize");
  InitCritical(WebServer::Initialize, "Initializing WebServer", "WebServer failed to initialize");
  InitCritical(OTA::Initialize, "Initializing OTA", "OTA failed to initialize");



  FrontComm::Initialize();
  MelodyPlayer::Initialize();
  Lock::Initialize();
  ActionHandler::Initialize();

  ESP_LOGI("Main", "Initialization complete");
}

void loop()
{
  OTA::Loop();
  delay(100);
}