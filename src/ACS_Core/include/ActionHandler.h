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

enum DoorState
{
    LOCKED = 0,
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


    static Melody melody_Unlock;
    static Melody melody_Lock;
    static void InitializeMelodies();


    public:
    static void Initialize();
    static void ExecuteAction(Action act);
};