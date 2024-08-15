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
    digitalWrite(PIN_BTN_UNLOCK_LED, (led & LEDUnlock) == LEDUnlock);
    digitalWrite(PIN_BTN_DISENG_LED, (led & LEDDisengage) == LEDDisengage);
}

void ActionButtons::SetLEDState(LEDState newState)
{
    ledState = newState;
    lastLedStateChange = millis();
}


LEDState ActionButtons::ledState = LEDState::Initializing;
ulong ActionButtons::lastLedStateChange = 0;

void ActionButtons::t_LedHandler(void* args)
{
    LEDState lastState = LEDState::Initializing;

    for(;;)
    {
        LEDState currState = ledState;
        switch(currState)
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

                if(lastState == LEDState::Unlocked)
                {
                    SetLED(LEDUnlock);
                    delaySinceStateChange(1000);
                }

                SetLED(NONE);
                break;

            case LEDState::Alarm:
                SetLED((LED)(LEDUnlock | LEDDisengage));
                delay(100);
                SetLED(NONE);
                delay(100);
                break;
            
            case LEDState::Disabled:
            break;
        }

        lastState = currState;
    }

    REMOTELOG_E("LED Handler Task Ended");
    vTaskDelete(NULL);
    delay(0);
}

void ActionButtons::t_ButtonHandler(void* args)
{
    for(;;)
    {
        if(ActionHandler::IsAlarmOn())
        {
            bool unlockState = digitalRead(PIN_BTN_UNLOCK);
            bool disengageState = digitalRead(PIN_BTN_DISENG);
            if(unlockState && disengageState)
            {   
                REMOTELOG_D("Both buttons are pressed");
                
                SetLEDState(LEDState::Disabled);
                delay(100);
                SetLED((LED)(LEDUnlock | LEDDisengage));
                REMOTELOG_D("LEDS ON");
                delay(3000);
                if(!digitalRead(PIN_BTN_UNLOCK) || !digitalRead(PIN_BTN_DISENG))
                {
                    REMOTELOG_D("One of the buttons is not pressed");
                    
                    if(ActionHandler::IsAlarmOn())
                    {
                        REMOTELOG_D("Alarm is on, entering LED state Alarm");
                        SetLEDState(LEDState::Alarm);
                    }
                    else REMOTELOG_D("Alarm is not on");
                    continue;
                }



                ButtonAlarmAbortMode();
                if(ActionHandler::IsAlarmOn())
                {
                        REMOTELOG_D("Alarm is on, entering LED state Alarm");
                    SetLEDState(LEDState::Alarm);
                }
                else REMOTELOG_D("Alarm is not on");
            }


            continue;
        }

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

        delay(10);
    }

    REMOTELOG_E("Button Handler Task Ended");
    vTaskDelete(NULL);
    delay(0);
}


void ActionButtons::delaySinceStateChange(ulong val)
{
    long actualDelay = val - (millis() - lastLedStateChange);
    REMOTELOG_V("Delaying %d", actualDelay);
    actualDelay = actualDelay < 0 ? 0 : actualDelay; // Clamp

    delay(actualDelay);
}

void ActionButtons::ButtonAlarmAbortMode()
{
    uint8_t requiredSequence[] = {0,0,1,1,0,0,1,1};
    uint8_t sequence[8];
    uint8_t sequenceIndex = 0;

    SetLED(NONE);

    REMOTELOG_D("Entering Button Alarm Abort Mode");

    for(;;)
    {
        if(!ActionHandler::IsAlarmOn()) return;

        if(ShouldUnlock())
        {
            sequence[sequenceIndex++] = 0;
            SetLED(LEDUnlock);
            delay(50);
            SetLED(NONE);
            REMOTELOG_D("+0");
        }
        else if(ShouldEngageDisengage())
        {
            sequence[sequenceIndex++] = 1;
            SetLED(LEDDisengage);
            delay(50);
            SetLED(NONE);
            REMOTELOG_D("+1");
        }

        if(sequenceIndex == 8)
        {
            if(memcmp(sequence, requiredSequence, 8) == 0) ActionHandler::AbortAlarm(ActionSource::Button);
            
            return;
        }
        
        delay(10);
    }
}