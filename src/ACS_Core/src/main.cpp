#include "IO/MelodyPlayer.h"
#include "IO/LCD.h"
#include "IO/Lock.h"
#include "ActionHandler.h"
#include "FrontComm.h"
#include "DataSaving.h"
#include "Network/NetworkMain.h"
#include "Network/OTA.h"
#include "Network/WebServer.h"
#include "RemoteLogging.h"
#include "Network/WebClient.h"

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

void InitOther(void (*initFunc)(), const char* title)
{
  char* title_alloc = (char*)malloc(strlen(title) + 1);
  memcpy(title_alloc, title, strlen(title) + 1);

  REMOTELOG_D("Initializing %s", title_alloc);

  free(title_alloc);

  initFunc();
}

void setup()
{ 
  Serial.begin(921600);
  MelodyPlayer::Initialize();

  delay(350);

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
  InitCritical(RemoteLogging::Initialize, "Initializing RemoteLogging", "RemoteLogging failed to init");
  InitCritical(WebClient::Initialize, "Initializing WebClient", "WebClient failed to initialize");
  
  InitOther(WebServer::Initialize, "WebServer");
  InitOther(OTA::Initialize, "OTA");
  InitOther(Lock::Initialize, "Lock");
  InitOther(ActionHandler::Initialize, "ActionHandler");

  REMOTELOG_I("Initialization complete");
}

void loop()
{
  OTA::Loop();
  NetworkMain::CheckWiFiConnection();
  delay(50);
}