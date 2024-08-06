#include "ActionHandler.h"
#define TAG "ActionHandler"

Action ActionHandler::TargetAction = NONE;
DoorState ActionHandler::CurrentState = LOCKED;

ulong ActionHandler::Time_DoorLocked = 0;

Melody ActionHandler::melody_Unlock;
Melody ActionHandler::melody_Lock;

void ActionHandler::Initialize()
{
    InitializeMelodies();

    bool doorLocked = Lock::IsDoorClosed();
    CurrentState = doorLocked ? LOCKED : UNLOCKED;

    if(CurrentState == UNLOCKED) action_Unlock();

    xTaskCreate(t_DoorHandler, "DoorHandler", 4096, NULL, 1, NULL);
}

void ActionHandler::InitializeMelodies()
{
    melody_Unlock.AddNote(2000, 150);
    melody_Unlock.AddNote(1000, 100);
    melody_Unlock.AddRest(200);
    melody_Unlock.AddNote(1000, 200);

    melody_Lock.AddNote(1500, 1000);
    melody_Lock.AddNote(750, 200);
}


void ActionHandler::ExecuteAction(Action act)
{
    TargetAction = act;
}

ActionRequestResult ActionHandler::Unlock()
{
    ESP_LOGI(TAG, "Unlock requested (Current State: %d)", CurrentState);
    if(CurrentState == UNLOCKED || CurrentState == UNLOCKED_WAITINGDOOROPEN) return ALREADY_UNLOCKED;
    ExecuteAction(UNLOCK);
    return SUCCESS;
}

ActionRequestResult ActionHandler::Engage()
{
    if(CurrentState != DISENGAGED) return ALREADY_ENGAGED;
    ExecuteAction(ENGAGE);
    return SUCCESS;
}

ActionRequestResult ActionHandler::Disengage()
{
    if(CurrentState == DISENGAGED) return ALREADY_DISENGAGED;
    ExecuteAction(DISENGAGE);
    return SUCCESS;
}


//----------------------------------------------------------

void ActionHandler::t_DoorHandler(void *args)
{
    for (;;)
    {

        AlarmCheck();

        if(CurrentState == LOCKED && !Lock::IsDoorClosed() && millis() - Time_DoorLocked < DOOR_LOCK_TIMEOUT)
        {
            action_Unlock();
        }

        if(CurrentState == UNLOCKED && Lock::IsDoorClosed())
        {
            action_Lock();
            Time_DoorLocked = millis();
        }

        if(CurrentState == UNLOCKED_WAITINGDOOROPEN && !Lock::IsDoorClosed())
        {
            CurrentState = UNLOCKED;
            Serial.println("Door Unlocked");
        }

        switch (TargetAction)
        {
            case UNLOCK: action_Unlock(); break;
            case ENGAGE: action_Engage(); break;
            case DISENGAGE: action_Disengage(); break;
        }

        TargetAction = NONE;
        delay(10);
    }
}

void ActionHandler::AlarmCheck()
{
    if(CurrentState != LOCKED || Lock::IsDoorClosed() || millis() - Time_DoorLocked < DOOR_LOCK_TIMEOUT) return;

    MelodyPlayer::SetAlarm(true);

    PreAlarm();

    if(CurrentState == LOCKED && !Lock::IsDoorClosed())
    {
        Alarm();
    }
}

void ActionHandler::PreAlarm()
{
    ESP_LOGW(TAG, "PRE-ALARM");
    ulong canBeCancelledUntil = millis() + 730;
    while(millis() <= canBeCancelledUntil)
    {
        if(Lock::IsDoorClosed())
        {
            MelodyPlayer::SetAlarm(false);
            return;
        }
        delay(10);
    }
}

void ActionHandler::Alarm()
{
    ESP_LOGW(TAG, "ALARM");
    for(;;)
    {
        delay(10);
    }
}

void ActionHandler::action_Lock()
{
    CurrentState = LOCKED;
    MelodyPlayer::PlayMelody(melody_Lock);
    Lock::SetSolenoid(false);
    
    Serial.println("Door Locked");
}

void ActionHandler::action_Unlock()
{
    CurrentState = UNLOCKED_WAITINGDOOROPEN;
    MelodyPlayer::PlayMelody(melody_Unlock);

    delay(150);
    digitalWrite(PIN_SOLENOID, HIGH);
    delay(100);
    digitalWrite(PIN_SOLENOID, LOW);
    delay(200);
    digitalWrite(PIN_SOLENOID, HIGH);


    Serial.println("Door Unlocked (Waiting for door to open)");
}

void ActionHandler::action_Engage()
{

}

void ActionHandler::action_Disengage()
{

}
