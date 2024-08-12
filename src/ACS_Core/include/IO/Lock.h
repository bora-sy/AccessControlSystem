#pragma once
#include <Arduino.h>
#include "Pinout.h"
#include "RemoteLogging.h"

class Lock
{
    private:
    static bool lastState;
    static ulong lastChange;


    public:
    static void Initialize();
    static void SetSolenoid(bool state);
    static bool GetSolenoid();
    static bool IsDoorClosed();
};