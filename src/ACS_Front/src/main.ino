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
  LCD::Clear();
}

void loop()
{
  HandleFeedback();
  HandleConfigMenu();
  HandleCardEntry();
  HandlePasswordEntry();
  LCD::Clear();
}


inline void HandleFeedback()
{
  if(CoreComm::feedbackEndMS > millis())
  {
    LCD::PrintCenter(CoreComm::feedback);

    while (CoreComm::feedbackEndMS > millis())
    {
      char key = Keypad::GetKey();
      if(key != 0)
      {
        CoreComm::feedbackEndMS = 0;
        return;
      }
    }
    
  }
}

inline void HandleConfigMenu()
{
  if(CoreComm::configMenuAuth == 0) return;
}

inline void HandleCardEntry()
{
  Card card = CardReader::ReadCard();
  if(card.IsEmpty()) return;
  
  CoreComm::CardEntry(card.UID);
  LCD::PrintCenter("Kart Okundu");
  delay(200);
}

inline void HandlePasswordEntry()
{

}
/*
void setup()
{
  Serial.begin(921600);
  Serial2.begin(115200);
}

void loop()
{
  if(Serial.available())
  {
    Serial2.write(Serial.read());
  }

  if(Serial2.available())
  {
    Serial.write(Serial2.read());
  }
}
*/