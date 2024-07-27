#include "IO/CardReader.h"
#include "IO/MelodyPlayer.h"
#include "IO/Keypad.h"


Melody m1;

void setup()
{
  Serial.begin(921600);
  Keypad::Initialize();
  CardReader::Initialize();

}

void loop()
{
  Card c = CardReader::ReadCard();
  if(c.IsEmpty()) return;
  Serial.printf("Card: %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n", c.UID[0], c.UID[1], c.UID[2], c.UID[3], c.UID[4], c.UID[5], c.UID[6], c.UID[7], c.UID[8], c.UID[9]);
}