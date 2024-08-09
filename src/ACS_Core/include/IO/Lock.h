#pragma once
#include <Arduino.h>
#include "Pinout.h"
#include "RemoteLogging.h"

class Lock
{
    private:

    public:
    static void Initialize();
    static void SetSolenoid(bool state);
    static bool GetSolenoid();
    static bool IsDoorClosed();
};