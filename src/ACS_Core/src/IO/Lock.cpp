#include "IO/Lock.h"

void Lock::Initialize()
{
    pinMode(PIN_SOLENOID, OUTPUT);
    pinMode(PIN_DOORLIMITSWITCH, INPUT_PULLDOWN);

    SetSolenoid(false);
}

void Lock::SetSolenoid(bool state)
{
    digitalWrite(PIN_SOLENOID, state);
}

bool lastState = false;
ulong lastChange = 0;

bool Lock::IsDoorLocked()
{
    bool read = digitalRead(PIN_DOORLIMITSWITCH);
    if(lastState == read) return read;
    if(millis() - lastChange < 100) return lastState;
    lastState = read;
    Serial.println(read ? "Door is locked" : "Door is unlocked");
    return read;
}