#include "IO/ActionButtons.h"


void ActionButtons::Initialize()
{
    pinMode(PIN_BTN_UNLOCK, INPUT_PULLUP);
    pinMode(PIN_BTN_DISENG, INPUT_PULLUP);

    pinMode(PIN_BTN_UNLOCK_LED, OUTPUT);
    pinMode(PIN_BTN_DISENG_LED, OUTPUT);

    xTaskCreate(t_LedHandler, "LedHandler", 4096, NULL, 1, NULL);

    REMOTELOG_I("ActionButtons Initialized");
}


bool ActionButtons::unlockLastState = false;
ulong ActionButtons::unlockLastChange = 0;

bool ActionButtons::ShouldUnlock()
{
    bool read = digitalRead(PIN_BTN_UNLOCK);
    if(unlockLastState == read) return read;
    if(millis() - unlockLastChange < 100) return unlockLastState;
    unlockLastState = read;
    return read;
}


bool ActionButtons::disengageLastState = false;
ulong ActionButtons::disengageLastChange = 0;

bool ActionButtons::ShouldEngageDisengage()
{
    bool read = digitalRead(PIN_BTN_DISENG);
    if(disengageLastState == read) return read;
    if(millis() - disengageLastChange < 100) return disengageLastState;
    disengageLastState = read;
    return read;
}

void ActionButtons::SetLED(LED led)
{
    digitalWrite(PIN_BTN_UNLOCK_LED, led & LEDUnlock == LEDUnlock);
    digitalWrite(PIN_BTN_DISENG_LED, led & LEDDisengage == LEDDisengage);
}

void ActionButtons::SetLEDState(LEDState newState)
{
    state = newState;
}

LEDState ActionButtons::state = LEDState::Initializing;

void ActionButtons::t_LedHandler(void* args)
{
    for(;;)
    {
        switch(state)
        {
            case LEDState::Initializing:
                SetLED(NONE);
                delay(500);
                SetLED((LED)(LEDUnlock | LEDDisengage));
                delay(500);
                break;

            case LEDState::Unlocked:
                SetLED(LEDUnlock);
                delay(250);
                SetLED(NONE);
                delay(250);
                break;
                
            case LEDState::Disengaged:
                SetLED(LEDDisengage);
                break;

            case LEDState::Locked:
                SetLED(NONE);
                break;
        }
    }
}