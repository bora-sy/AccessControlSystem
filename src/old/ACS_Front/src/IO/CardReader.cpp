#include "IO/CardReader.h"


namespace CardReader
{
    MFRC522 rfid(PIN_RC522_SS, PIN_RC522_RST);

    void Initialize()
    {
        SPI.begin();
        rfid.PCD_Init();
    }

    Card ReadCard()
    {
        if (!rfid.PICC_IsNewCardPresent())
            return Card();

        if (!rfid.PICC_ReadCardSerial())
            return Card();

        return Card(rfid.uid.uidByte, rfid.uid.size);
    }
}


// --- CARD START ---

Card::Card()
{
    memset(UID, 0, FIXED_UID_LENGTH);
}

Card::Card(uint8_t *uid, uint8_t uidLength)
{
    memset(UID, 0, FIXED_UID_LENGTH);
    memcpy(&UID[FIXED_UID_LENGTH - uidLength], uid, uidLength);
}

bool Card::IsEmpty()
{
    static const uint8_t emptyUID[FIXED_UID_LENGTH] = {0};

    return memcmp(UID, emptyUID, FIXED_UID_LENGTH) == 0;
}

bool Card::operator==(const Card &other) const
{
    return memcmp(UID, other.UID, FIXED_UID_LENGTH) == 0;
}

// --- CARD END ---
