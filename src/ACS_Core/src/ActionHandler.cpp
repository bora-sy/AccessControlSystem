#include "ActionHandler.h"

Action ActionHandler::TargetAction = NONE;
DoorState ActionHandler::CurrentState = LOCKED;

ulong ActionHandler::Time_DoorLocked = 0;

void ActionHandler::Initialize()
{
    xTaskCreate(t_DoorHandler, "DoorHandler", 4096, NULL, 1, NULL);

    bool isDoorLocked = Lock::IsDoorLocked();
    CurrentState = isDoorLocked ? LOCKED : UNLOCKED;
}

void ActionHandler::ExecuteAction(Action act)
{
    TargetAction = act;
}

//----------------------------------------------------------

void ActionHandler::t_DoorHandler(void *args)
{
    for (;;)
    {
        AlarmCheck();

        if(CurrentState == LOCKED && !Lock::IsDoorLocked() && millis() - Time_DoorLocked < DOOR_LOCK_TIMEOUT)
        {
            action_Unlock();
        }

        switch (TargetAction)
        {
            case LOCK: action_Lock(); break;
            case UNLOCK: action_Unlock(); break;
            case ENGAGE: action_Engage(); break;
            case DISENGAGE: action_Disengage(); break;
        }

        TargetAction = NONE;
    }
}

void ActionHandler::AlarmCheck()
{
    if(CurrentState != LOCKED || Lock::IsDoorLocked() || millis() - Time_DoorLocked < DOOR_LOCK_TIMEOUT) return;

    MelodyPlayer::SetAlarm(true);
    
    for(;;)
    {
        delay(10);
    }

}