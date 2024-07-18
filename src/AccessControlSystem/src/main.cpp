#include "IO/CardReader.h"

void setup()
{
  Serial.begin(921600);
  CardReader::Initialize();
}

void loop()
{
    Card card = CardReader::ReadCard();

    if (card.IsEmpty())
        return;

    Serial.print("Card UID: ");
    for (int i = 0; i < FIXED_UID_LENGTH; i++)
    {
        Serial.print(card.UID[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}