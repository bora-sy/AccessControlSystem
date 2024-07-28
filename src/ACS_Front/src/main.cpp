#include "IO/CardReader.h"
#include "IO/Keypad.h"
#include "IO/LCD.h"
#include "CoreComm.h"

void setup()
{
  Serial.begin(921600);
  Serial.println("App Started");
  if(!LCD::Initialize())
  {
    Serial.println("LCD initialization failed!"); 
    ESP.restart(); 
    return;
  }

  LCD::PrintCenter("Starting...");

  CoreComm::Initialize();
  Keypad::Initialize();
  CardReader::Initialize();

  bool res = CoreComm::Ping(1000);
  if(!res)
  {
    Serial.println("Core Ping failed!");
    LCD::PrintCenter("Core Ping failed!");
    delay(1000);
    ESP.restart();
    return;
  }

  Serial.println("Loop");
}

void loop()
{

}