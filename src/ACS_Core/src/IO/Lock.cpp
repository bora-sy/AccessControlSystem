#include "IO/Lock.h"

bool Lock::lastState = false;
ulong Lock::lastChange = 0;

void Lock::Initialize()
{
    pinMode(PIN_SOLENOID, OUTPUT);
    pinMode(PIN_DOORLIMITSWITCH, INPUT_PULLDOWN);

    SetSolenoid(false);
}

void Lock::SetSolenoid(bool state)
{
    REMOTELOG_D("New Solenoid State: %d", state);
    digitalWrite(PIN_SOLENOID, state);
}

bool Lock::GetSolenoid()
{
    return digitalRead(PIN_SOLENOID);
}


bool Lock::IsDoorClosed()
{
    bool read = digitalRead(PIN_DOORLIMITSWITCH);
    if(lastState == read) return read;
    if(millis() - lastChange < 100) return lastState;
    lastState = read;
    return read;
}
