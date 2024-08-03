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

bool Lock::GetSolenoid()
{
    return digitalRead(PIN_SOLENOID);
}

bool lastState = false;
ulong lastChange = 0;

bool Lock::IsDoorClosed()
{
    bool read = digitalRead(PIN_DOORLIMITSWITCH);
    if(lastState == read) return read;
    if(millis() - lastChange < 100) return lastState;
    lastState = read;
    return read;
}
