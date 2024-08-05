#pragma once
#include <Arduino.h>
#include "IO/Lock.h"
#include "IO/MelodyPlayer.h"

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
    ALREADY_LOCKED,
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

    static void action_Lock();
    static void action_Unlock();
    static void action_Engage();
    static void action_Disengage();

    static void ExecuteAction(Action act);


    static Melody melody_Unlock;
    static Melody melody_Lock;
    static void InitializeMelodies();


    public:
    static void Initialize();

    static ActionRequestResult Lock();
    static ActionRequestResult Unlock();
    static ActionRequestResult Engage();
    static ActionRequestResult Disengage();
};