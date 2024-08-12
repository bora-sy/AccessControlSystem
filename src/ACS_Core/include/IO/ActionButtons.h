#pragma once
#include "Pinout.h"
#include "RemoteLogging.h"
#include "ActionHandler.h" 


enum LEDState
{
    Initializing = 0,

    Locked,
    Unlocked,
    Disengaged,
};

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


    static LEDState state;

    static void SetLED(LED led);

    public:
    static void Initialize();

    static bool ShouldUnlock();
    static bool ShouldEngageDisengage();

    static void SetLEDState(LEDState state);
    
};