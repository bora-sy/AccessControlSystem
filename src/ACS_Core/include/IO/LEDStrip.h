#pragma once
#include "Pinout.h"
#include "RemoteLogging.h"
#include "ActionButtons.h"

#define CHANNEL_LEDSTR_R 2
#define CHANNEL_LEDSTR_G 3
#define CHANNEL_LEDSTR_B 4

class LEDStrip
{
    private:

    static void t_LedStripHandler(void* args);

    static LEDState stripState;
    static ulong lastStripStateChange;
    
    static void delaySinceStateChange(ulong val);

    static void SetRGB(uint8_t r, uint8_t g, uint8_t b);

    public:
    static void Initialize();

    static void SetStripState(LEDState state);
    
};