#include "IO/CardReader.h"
#include "IO/Keypad.h"
#include "IO/LCD.h"
#include "CoreComm.h"

#define IGNORE_CORE_PING

#define PASSWORD_TIMEOUT_MS 30000


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

  #ifndef IGNORE_CORE_PING
  if(!res)
  {
    Serial.println("Core Ping failed!");
    LCD::PrintCenter("Core Ping failed!");
    delay(1000);
    ESP.restart();
    return;
  }
  #endif

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
      delay(5);
      /*
      char key = Keypad::GetKey();
      if(key != 0)
      {
        CoreComm::feedbackEndMS = 0;
        return;
      }
      */
    }
    
  }
}

inline void HandleConfigMenu()
{

  if(CoreComm::configMenuAuth == 0) return;

  LCD::PrintCenterRow("Enter Backend IP", 0);
  char ipStr[16];
  memset(ipStr, 0, 16);

  uint8_t nextChar = 0;

  while (true)
  {
    char key = Keypad::GetKey();
    if(key == 0) continue;
    
    if(key >= 48 && key <= 57) // numeric
    {
      if(nextChar >= 15) continue;
      if(nextChar == 3 || nextChar == 7 || nextChar == 11) ipStr[nextChar++] = '.';
      ipStr[nextChar++] = key;
      LCD::PrintCenterRow(ipStr, 1);
    }
    else if(key == 'A') // Backspace
    {
      if(nextChar == 0) continue;
      ipStr[--nextChar] = 0;
      LCD::PrintCenterRow(ipStr, 1);
    }
    else if(key == '#') // Submit
    {
      if(nextChar < 15) continue;
      uint ip[4];
      sscanf(ipStr, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
      Serial.printf("IP: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
      
      for(int i = 0; i<4; i++) 
      if(ip[i] > 255)
      {
        LCD::PrintCenter("Invalid IP");
        delay(2000);
        return;
      }

      uint8_t ip_formatted[] = {ip[0], ip[1], ip[2], ip[3]};

      CoreComm::Config_SetIP(ip_formatted);
      LCD::PrintCenter("IP Submitted");
      delay(200);

      CoreComm::configMenuAuth = 0;
      return;
    }
    else if(key == '*') // Cancel
    {
      CoreComm::configMenuAuth = 0;
      return;
    }
    
  }
  

}

inline void HandleCardEntry()
{
  Card card = CardReader::ReadCard();
  if(card.IsEmpty()) return;
  
  CoreComm::CardEntry(card.UID);
  LCD::PrintCenter("Card Read");
  delay(200);
}


ulong lastPasswordKeyInput = 0;
inline void HandlePasswordEntry()
{
  char key = Keypad::GetKey();
  if(key == 0) return;

  uint8_t nextIndex = 0;
  char password[17];
  char maskedPassword[17];
  memset(password, 0, 17);
  memset(maskedPassword, 0, 17);

  bool passwordMasked = true;
  

  if(key >= 48 && key <= 57)
  {
    password[nextIndex] = key;
    maskedPassword[nextIndex] = '*';
    nextIndex++;
    LCD::PrintCenterRow("Password", 0);
    LCD::PrintCenterRow(passwordMasked ? maskedPassword : password, 1);
    lastPasswordKeyInput = millis();
    CoreComm::PlayTone(1000, 100); // TODO: Set tone
  }
  else return;


  while (true)
  {
    if(lastPasswordKeyInput + PASSWORD_TIMEOUT_MS < millis())
    {
      Serial.println("Timeout");
      return;
    }

    key = Keypad::GetKey();
    if(key == 0) continue;

    if(key == 'A') // Backspace
    {
      if(nextIndex == 0) return;

      password[nextIndex-1] = 0;
      maskedPassword[nextIndex-1] = 0;
      nextIndex--;
      LCD::PrintCenterRow(passwordMasked ? maskedPassword : password, 1);
      lastPasswordKeyInput = millis();
      CoreComm::PlayTone(800, 100); // TODO: Set tone
      continue;
    }
    else if (key == 'D') // Toggle Mask
    {
      passwordMasked = !passwordMasked;
      LCD::PrintCenterRow(passwordMasked ? maskedPassword : password, 1);
      lastPasswordKeyInput = millis();
      continue;
    }
    else if (key == '#') // Submit
    {
      if(nextIndex == 0) return;
      CoreComm::PasswordEntry(atoll(password));
      LCD::PrintCenter("Password Submitted");
      delay(200);
      return;
    }
    else if ((key < 48 || key > 57)) continue;

    if(nextIndex >= 16) continue;
    

    password[nextIndex] = key;
    maskedPassword[nextIndex] = '*';
    nextIndex++;
    lastPasswordKeyInput = millis();
    CoreComm::PlayTone(1000, 100); // TODO: Set tone
    Serial.println(password);

    LCD::PrintCenterRow(passwordMasked ? maskedPassword : password, 1);
  }
  




  
  char passwordBuf[16];
  memset(passwordBuf, 0, 16);
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