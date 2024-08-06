#pragma once
#include <Arduino.h>
#include "IO/Lock.h"
#include "IO/MelodyPlayer.h"
#include "esp_log.h"

#define DOOR_LOCK_TIMEOUT 5000

enum Action
{
    NONE = 0,
    LOCK,
    UNLOCK,
    ENGAGE,
    DISENGAGE
};

enum ActionRequestResult
{
    SUCCESS = 0,
    ALREADY_UNLOCKED,
    ALREADY_ENGAGED,
    ALREADY_DISENGAGED,
};

enum DoorState
{
    LOCKED = 0,
    UNLOCKED_WAITINGDOOROPEN,
    UNLOCKED,
    DISENGAGED
};


class ActionHandler
{
    private:

    static Action TargetAction;
    static ulong Time_DoorLocked;

    static DoorState CurrentState;
    
    static void t_DoorHandler(void *args);

    static void AlarmCheck();
    static void PreAlarm();
    static void Alarm();

    static void action_Lock(bool useFeedback = true);
    static void action_Unlock(bool useFeedback = true);
    static void action_Engage(bool useFeedback = true);
    static void action_Disengage(bool useFeedback = true);

    static void ExecuteAction(Action act);


    static Melody melody_Unlock;
    static Melody melody_Lock;
    static Melody melody_Disengage;
    static Melody melody_Engage;

    static void InitializeMelodies();


    public:
    static void Initialize();
    static ActionRequestResult Unlock();
    static ActionRequestResult Engage();
    static ActionRequestResult Disengage();
};