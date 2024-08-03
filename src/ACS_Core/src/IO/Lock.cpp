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

bool Lock::IsDoorLocked()
{
    return digitalRead(PIN_DOORLIMITSWITCH);
}