#include "IO/ActionButtons.h"


void ActionButtons::Initialize()
{
    pinMode(PIN_BTN_UNLOCK, INPUT_PULLDOWN);
    pinMode(PIN_BTN_DISENG, INPUT_PULLDOWN);

    pinMode(PIN_BTN_UNLOCK_LED, OUTPUT);
    pinMode(PIN_BTN_DISENG_LED, OUTPUT);

    xTaskCreate(t_LedHandler, "LedHandler", 4096, NULL, 1, NULL);
    xTaskCreate(t_ButtonHandler, "ButtonHandler", 4096, NULL, 1, NULL);

    REMOTELOG_I("ActionButtons Initialized");
}


bool ActionButtons::unlockLastState = false;
ulong ActionButtons::unlockLastChange = 0;

bool ActionButtons::ShouldUnlock()
{
    bool read = digitalRead(PIN_BTN_UNLOCK);
    if(read == unlockLastState) return false;
    if(millis() - unlockLastChange < 100) return false;
    unlockLastState = read;
    unlockLastChange = millis();
    return read;
}


bool ActionButtons::disengageLastState = false;
ulong ActionButtons::disengageLastChange = 0;

bool ActionButtons::ShouldEngageDisengage()
{
    bool read = digitalRead(PIN_BTN_DISENG);
    if(read == disengageLastState) return false;
    if(millis() - disengageLastChange < 100) return false;
    disengageLastState = read;
    disengageLastChange = millis();
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
                delay(800);
                SetLED((LED)(LEDUnlock | LEDDisengage));
                delay(800);
                break;

            case LEDState::Unlocked:
                SetLED(LEDUnlock);
                delay(150);
                SetLED(NONE);
                delay(150);
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

void ActionButtons::t_ButtonHandler(void* args)
{
    for(;;)
    {
        if(ShouldUnlock())
        {
            ActionHandler::Unlock(ActionSource::Button);
        }
        if(ShouldEngageDisengage())
        {
            if(ActionHandler::GetCurrentState() == DoorState::DISENGAGED)
            {
                ActionHandler::Engage(ActionSource::Button);
            }
            else
            {
                ActionHandler::Disengage(ActionSource::Button);
            }
        }

        delay(200);
    }
}