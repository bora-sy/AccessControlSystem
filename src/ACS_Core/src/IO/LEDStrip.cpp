#include "IO/LEDStrip.h"

LEDState LEDStrip::stripState = LEDState::Initializing;
ulong LEDStrip::lastStripStateChange = 0;

void LEDStrip::Initialize()
{
    pinMode(PIN_LEDSTRIP_R, OUTPUT);
    pinMode(PIN_LEDSTRIP_R, OUTPUT);
    pinMode(PIN_LEDSTRIP_R, OUTPUT);

    digitalWrite(PIN_LEDSTRIP_R, LOW);
    digitalWrite(PIN_LEDSTRIP_G, LOW);
    digitalWrite(PIN_LEDSTRIP_B, LOW);

    ledcAttachPin(PIN_LEDSTRIP_R, CHANNEL_LEDSTR_R);
    ledcAttachPin(PIN_LEDSTRIP_G, CHANNEL_LEDSTR_G);
    ledcAttachPin(PIN_LEDSTRIP_B, CHANNEL_LEDSTR_B);

    xTaskCreate(t_LedStripHandler, "t_LedStripHandler", 4096, NULL, 1, NULL);

    REMOTELOG_I("LEDStrip Initialized");
}

void LEDStrip::SetRGB(uint8_t r, uint8_t g, uint8_t b)
{
    ledcWrite(CHANNEL_LEDSTR_R, r);
    ledcWrite(CHANNEL_LEDSTR_G, g);
    ledcWrite(CHANNEL_LEDSTR_B, b);
}

void LEDStrip::SetStripState(LEDState state)
{
    stripState = state;
}

void LEDStrip::delaySinceStateChange(ulong val)
{
    long actualDelay = val - (millis() - lastStripStateChange);
    REMOTELOG_V("Delaying %d", actualDelay);
    actualDelay = actualDelay < 0 ? 0 : actualDelay; // Clamp

    delay(actualDelay);
}

void LEDStrip::t_LedStripHandler(void* args)
{
    LEDState lastState = LEDState::Initializing;

    for(;;)
    { 
        LEDState currState = stripState;
        switch(currState)
        {
            case LEDState::Initializing:
                SetRGB(0,0,0);
                delay(800);
                SetRGB(0,0,255);
                delay(800);
                break;

            case LEDState::Unlocked:
                SetRGB(0,255,0);
                break;
                
            case LEDState::Disengaged:
                SetRGB(0, 195, 255);
                break;

            case LEDState::Locked:

                if(lastState == LEDState::Unlocked)
                {
                    SetRGB(255, 162, 0);
                    delaySinceStateChange(1000);
                }

                SetRGB(255, 0, 0);
                break;

            case LEDState::Alarm:
                SetRGB(255, 0, 0);
                delay(100);
                SetRGB(0,0,0);
                delay(100);
                break;
            
            case LEDState::Disabled:
                SetRGB(0, 0, 0);
            break;
        }

        lastState = currState;
        delay(10);
    }

    REMOTELOG_E("LEDStrip Handler task ended unexpectedly");
    vTaskDelete(NULL);
    delay(0);
}

