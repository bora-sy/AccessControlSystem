#pragma once


enum LEDState
{
    Disabled = 0,
    Initializing,

    Locked,
    Unlocked,
    Disengaged,
    Alarm
};

#include "Pinout.h"
#include "RemoteLogging.h"
#include "ActionHandler.h" 

enum LED : int
{
    NONE = 0,
    LEDUnlock,
    LEDDisengage
};

class ActionButtons
{
    private:
    
    static bool  unlockLastState;
    static ulong unlockLastChange;

    static bool  disengageLastState;
    static ulong disengageLastChange;

    static void t_LedHandler(void* args);
    static void t_ButtonHandler(void* args);

    static void ButtonAlarmAbortMode();

    static void delaySinceStateChange(ulong val);


    static LEDState ledState;
    static ulong lastLedStateChange;

    static void SetLED(LED led);

    public:
    static void Initialize();

    static bool ShouldUnlock();
    static bool ShouldEngageDisengage();

    static void SetLEDState(LEDState state);
    
};