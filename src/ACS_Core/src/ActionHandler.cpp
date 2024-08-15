#include "ActionHandler.h"
#define TAG "ActionHandler"

Action ActionHandler::TargetAction = Action::ACTNONE;
ActionSource ActionHandler::TargetActionSource = ActionSource::SRCNONE;

DoorState ActionHandler::CurrentState = LOCKED;

ulong ActionHandler::Time_DoorLocked = 0;
ulong ActionHandler::UnlockTimeout = 0;

bool ActionHandler::AlarmOn = false;

Melody ActionHandler::melody_Unlock;
Melody ActionHandler::melody_Lock;
Melody ActionHandler::melody_Disengage;
Melody ActionHandler::melody_Engage;

void ActionHandler::Initialize()
{
    InitializeMelodies();

    bool doorLocked = Lock::IsDoorClosed();

    if(doorLocked)
    {
        CurrentState = LOCKED;
        ActionButtons::SetLEDState(LEDState::Locked);
    }
    else
    {
        action_Unlock(ActionSource::SRCNONE);
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

DoorState ActionHandler::GetCurrentState()
{
    return CurrentState;
}


void ActionHandler::ExecuteAction(Action act, ActionSource src)
{
    TargetAction = act;
    TargetActionSource = src;
}

ActionRequestResult ActionHandler::Unlock(ActionSource src)
{
    if(CurrentState == UNLOCKED || CurrentState == UNLOCKED_WAITINGDOOROPEN || CurrentState == DISENGAGED) return ALREADY_UNLOCKED;
    ExecuteAction(UNLOCK, src);
    return SUCCESS;
}

ActionRequestResult ActionHandler::Engage(ActionSource src)
{
    if(CurrentState != DISENGAGED) return ALREADY_ENGAGED;
    ExecuteAction(ENGAGE, src);
    return SUCCESS;
}

ActionRequestResult ActionHandler::Disengage(ActionSource src)
{
    if(CurrentState == DISENGAGED) return ALREADY_DISENGAGED;
    ExecuteAction(DISENGAGE, src);
    return SUCCESS;
}

ActionRequestResult ActionHandler::AbortAlarm(ActionSource src)
{
    if(!AlarmOn)
    {
        REMOTELOG_D("Abort Alarm requested but alarm is not active");
        return ALARM_NOT_ACTIVE;
    }
    
    ExecuteAction(ABORTALARM, src);
    REMOTELOG_D("Target Action set to: %d", (int)TargetAction);
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

            action_Unlock(ActionSource::SRCNONE);
        }

        if(CurrentState == UNLOCKED && Lock::IsDoorClosed())
        {
            REMOTELOG_D("Door closed. Locking...");

            action_Lock();
        }


        if(CurrentState == UNLOCKED_WAITINGDOOROPEN && !Lock::IsDoorClosed())
        {
            REMOTELOG_D("Door open. Unlocked: stage 2");
            CurrentState = UNLOCKED;
        }
        
        if(CurrentState == UNLOCKED_WAITINGDOOROPEN && millis() > UnlockTimeout && Lock::IsDoorClosed())
        {
            REMOTELOG_D("Door unlock timeout reached. Locking...");
            action_Lock();
        }

        switch (TargetAction)
        {
            case ACTNONE: break;
            case UNLOCK: action_Unlock(TargetActionSource); break;
            case ENGAGE: action_Engage(); break;
            case DISENGAGE: action_Disengage(); break;
            case ABORTALARM: REMOTELOG_I("Received Abort Alarm.. Ignoring"); break;
            default: REMOTELOG_W("Unknown target action %d", TargetAction); break;
        }

        TargetAction = Action::ACTNONE;
        TargetActionSource = ActionSource::SRCNONE;
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
    WebClient::LogAlarm(true, true);

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

bool ActionHandler::IsAlarmOn()
{
    return AlarmOn;
}

void ActionHandler::Alarm()
{
    REMOTELOG_D("ALARM MODE");
    AlarmOn = true;
    ActionButtons::SetLEDState(LEDState::Alarm);
    WebClient::LogAlarm(false, true);

    for(;;)
    {
        if(TargetAction == ABORTALARM)
        {
            TargetAction = ACTNONE;
            REMOTELOG_I("Alarm Aborted");
            break;
        }

        delay(100);
    }

    MelodyPlayer::SetAlarm(false);
    WebClient::LogAlarm(false, false);
    AlarmOn = false;

    delay(100);

    action_Unlock(ActionSource::SRCNONE);
}

void ActionHandler::action_Lock(bool useFeedback)
{
    REMOTELOG_I("Action Lock Triggered");

    if(CurrentState == LOCKED || !Lock::IsDoorClosed())
    {
        REMOTELOG_W("Door is already locked or not closed. Aborting lock action.");
        return;
    }

    ActionButtons::SetLEDState(LEDState::Locked);
    CurrentState = LOCKED;

    if(useFeedback)
    {
        MelodyPlayer::PlayMelody(melody_Lock);
    }


    Lock::SetSolenoid(false);
    Time_DoorLocked = millis();
}

void ActionHandler::action_Unlock(ActionSource src, bool useFeedback)
{
    REMOTELOG_I("Action Unlock Triggered");

    if(CurrentState == UNLOCKED || CurrentState == UNLOCKED_WAITINGDOOROPEN || CurrentState == DISENGAGED)
    {
        REMOTELOG_W("Door is already unlocked or disengaged. Aborting unlock action.");
        return;
    }

    ActionButtons::SetLEDState(LEDState::Unlocked);
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

    ulong timeout = 0;

    switch (src)
    {
    
    case Button: timeout = DOOR_UNLOCK_TIMEOUT_BUTTON; break;

    case Front: timeout = DOOR_UNLOCK_TIMEOUT_FRONT; break;
    case Discord: timeout = DOOR_UNLOCK_TIMEOUT_DISCORD; break;
    case Web: timeout = DOOR_UNLOCK_TIMEOUT_WEB; break;
    case ActionSource::SRCNONE: timeout = DOOR_UNLOCK_TIMEOUT_DEFAULT; break;
    
    default: 
    timeout = DOOR_UNLOCK_TIMEOUT_DEFAULT;
    REMOTELOG_W("Default Door Unlock Timeout");
    break;
    }

    UnlockTimeout = millis() + timeout;
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
        ActionButtons::SetLEDState(LEDState::Unlocked);
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

    ActionButtons::SetLEDState(LEDState::Disengaged);
    CurrentState = DISENGAGED;
    
    Lock::SetSolenoid(true);

    if(useFeedback)
    {
        MelodyPlayer::PlayMelody(melody_Disengage);
    }
}