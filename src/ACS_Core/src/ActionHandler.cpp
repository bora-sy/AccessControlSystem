#include "ActionHandler.h"
#define TAG "ActionHandler"

Action ActionHandler::TargetAction = NONE;
DoorState ActionHandler::CurrentState = LOCKED;

ulong ActionHandler::Time_DoorLocked = 0;

Melody ActionHandler::melody_Unlock;
Melody ActionHandler::melody_Lock;
Melody ActionHandler::melody_Disengage;
Melody ActionHandler::melody_Engage;

void ActionHandler::Initialize()
{
    InitializeMelodies();

    bool doorLocked = Lock::IsDoorClosed();
    CurrentState = doorLocked ? LOCKED : UNLOCKED;

    if(doorLocked) CurrentState = LOCKED;
    else
    {
        action_Unlock();
        CurrentState = UNLOCKED;
    }
    

    xTaskCreate(t_DoorHandler, "DoorHandler", 4096, NULL, 1, NULL);

    REMOTELOG_I("ActionHandler Initialized");
}

void ActionHandler::InitializeMelodies()
{
    melody_Unlock.AddNote(2000, 150);
    melody_Unlock.AddNote(1000, 100);
    melody_Unlock.AddRest(200);
    melody_Unlock.AddNote(1000, 200);

    melody_Lock.AddNote(1500, 1000);
    melody_Lock.AddNote(750, 200);

    melody_Engage.AddNote(1500, 100);
    melody_Engage.AddRest(50);
    melody_Engage.AddNote(1500, 100);
    melody_Engage.AddRest(50);
    melody_Engage.AddNote(1500, 100);

    melody_Disengage.AddNote(1500, 100);
    melody_Disengage.AddNote(1000, 100);
    melody_Disengage.AddNote(750, 150);
}


void ActionHandler::ExecuteAction(Action act)
{
    TargetAction = act;
}

ActionRequestResult ActionHandler::Unlock()
{
    if(CurrentState == UNLOCKED || CurrentState == UNLOCKED_WAITINGDOOROPEN || CurrentState == DISENGAGED) return ALREADY_UNLOCKED;
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
            REMOTELOG_D("Locked but door is not closed. Unlocking...");

            action_Unlock();
        }

        if(CurrentState == UNLOCKED && Lock::IsDoorClosed())
        {
            REMOTELOG_D("Door closed. Locking...");

            action_Lock();
            Time_DoorLocked = millis();
        }

        if(CurrentState == UNLOCKED_WAITINGDOOROPEN && !Lock::IsDoorClosed())
        {
            REMOTELOG_D("Door open. Unlocked: stage 2");
            CurrentState = UNLOCKED;
        }

        switch (TargetAction)
        {
            case NONE: break;
            case UNLOCK: action_Unlock(); break;
            case ENGAGE: action_Engage(); break;
            case DISENGAGE: action_Disengage(); break;
            default: REMOTELOG_W("Unknown target action %d", TargetAction); break;
        }

        TargetAction = NONE;
        delay(10);
    }
}

void ActionHandler::AlarmCheck()
{
    if(CurrentState != LOCKED || Lock::IsDoorClosed() || millis() - Time_DoorLocked < DOOR_LOCK_TIMEOUT) return;
    REMOTELOG_W("ALARM");

    MelodyPlayer::SetAlarm(true);

    PreAlarm();

    if(CurrentState == LOCKED && !Lock::IsDoorClosed())
    {
        Alarm();
    }
}

void ActionHandler::PreAlarm()
{
    REMOTELOG_D("PREALARM MODE");

    ulong canBeCancelledUntil = millis() + 730;
    while(millis() <= canBeCancelledUntil)
    {
        if(Lock::IsDoorClosed())
        {
            REMOTELOG_W("Alarm aborted");
            MelodyPlayer::SetAlarm(false);
            return;
        }
        delay(10);
    }
}

void ActionHandler::Alarm()
{
    REMOTELOG_D("ALARM MODE");

    for(;;)
    {
        delay(10);
    }
}

void ActionHandler::action_Lock(bool useFeedback)
{
    REMOTELOG_I("Action Lock Triggered");

    if(CurrentState == LOCKED || !Lock::IsDoorClosed())
    {
        REMOTELOG_W("Door is already locked or not closed. Aborting lock action.");
        return;
    }
    
    CurrentState = LOCKED;

    if(useFeedback)
    {
        MelodyPlayer::PlayMelody(melody_Lock);
    }


    Lock::SetSolenoid(false);
}

void ActionHandler::action_Unlock(bool useFeedback)
{
    REMOTELOG_I("Action Unlock Triggered");

    if(CurrentState == UNLOCKED || CurrentState == UNLOCKED_WAITINGDOOROPEN || CurrentState == DISENGAGED)
    {
        REMOTELOG_W("Door is already unlocked or disengaged. Aborting unlock action.");
        return;
    }

    CurrentState = UNLOCKED_WAITINGDOOROPEN;

    if(useFeedback)
    {
        MelodyPlayer::PlayMelody(melody_Unlock);
    }

    delay(150);
    Lock::SetSolenoid(true);
    delay(100);
    Lock::SetSolenoid(false);
    delay(200);
    Lock::SetSolenoid(true);
}

void ActionHandler::action_Engage(bool useFeedback)
{
    REMOTELOG_I("Action Engage Triggered");

    if(CurrentState != DISENGAGED)
    {
        REMOTELOG_W("Door is already engaged. Aborting engage action.");
        return;
    }

    if(Lock::IsDoorClosed())
    {
        action_Lock(false);
    }
    else
    {
        CurrentState = UNLOCKED;
    }

    if(useFeedback)
    {
        MelodyPlayer::PlayMelody(melody_Engage);
    }
}

void ActionHandler::action_Disengage(bool useFeedback)
{
    REMOTELOG_I("Action Disengage Triggered");
    
    if(CurrentState == DISENGAGED)
    {
        REMOTELOG_W("Door is already disengaged. Aborting disengage action.");
        return;
    }

    CurrentState = DISENGAGED;
    
    Lock::SetSolenoid(true);

    if(useFeedback)
    {
        MelodyPlayer::PlayMelody(melody_Disengage);
    }
}