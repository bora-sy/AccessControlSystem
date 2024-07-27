#include "IO/CardReader.h"
#include "IO/MelodyPlayer.h"
#include "IO/Keypad.h"


Melody m1;

void setup()
{
  Serial.begin(921600);
  Keypad::Initialize();

}

void loop()
{
  char key = Keypad::GetKey();
  if(key == '\0') return;

  if(key == 'A') Serial.println();
  else Serial.print(key);
}