#pragma once
#include <Arduino.h>
#include "Pinout.h"

class Lock
{
    private:

    public:
    static void Initialize();
    static void SetSolenoid(bool state);
    static bool IsDoorLocked();
};