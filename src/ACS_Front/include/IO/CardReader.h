#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include "Pinout.h"

#define FIXED_UID_LENGTH 10

struct Card
{

    uint8_t UID[FIXED_UID_LENGTH];

    Card();

    Card(uint8_t *uid, uint8_t uidLength);

    bool IsEmpty();

    bool operator==(const Card &other) const;
};

namespace CardReader
{
    void Initialize();

    Card ReadCard();
}




