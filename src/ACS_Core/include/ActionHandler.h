#pragma once
#include <Arduino.h>

enum Action
{
    NONE = 0,
    LOCK,
    UNLOCK,
    ENGAGE,
    DISENGAGE
};

class ActionHandler
{
    private:
    static void t_DoorHandler(void *args);

    static Action TargetAction;
    static ulong Time_DoorLocked;

    public:
    static void Initialize();
    static void ExecuteAction(Action act);
};