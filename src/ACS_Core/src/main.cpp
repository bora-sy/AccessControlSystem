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
    delay(1500);
    LCD::PrintCenter("Restarting");
    delay(800);
    ESP.restart();
  }
}

void setup()
{
  Serial.begin(921600);
  Serial.println("Serial Begin");
  MelodyPlayer::Initialize();
  delay(300);

  Melody startMelody;
  startMelody.AddNote(1000, 250);
  startMelody.AddRest(100);
  startMelody.AddNote(2000, 150);
  MelodyPlayer::PlayMelody(startMelody);

  delay(500);

  LCD::Initialize();

  InitCritical(DataSaving::Initialize, "Initializing DataSaving", "DataSaving failed to initialize");
  InitCritical(Config::Initialize, "Initializing Config", "Config failed to initialize");
  InitCritical(NetworkMain::Initialize, "Initializing NetworkMain", "NetworkMain failed to initialize");
  InitCritical(WebServer::Initialize, "Initializing WebServer", "WebServer failed to initialize");
  InitCritical(OTA::Initialize, "Initializing OTA", "OTA failed to initialize");



  FrontComm::Initialize();
  Lock::Initialize();
  ActionHandler::Initialize();

  ESP_LOGI("Main", "Initialization complete");
}

void loop()
{
  OTA::Loop();
  NetworkMain::CheckWiFiConnection();
  delay(100);
}