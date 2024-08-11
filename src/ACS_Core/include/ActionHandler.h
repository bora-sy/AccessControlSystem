#pragma once
#include <Arduino.h>
#include "IO/Lock.h"
#include "IO/MelodyPlayer.h"
#include "RemoteLogging.h"
#include "Network/WebClient.h"

#define DOOR_LOCK_TIMEOUT 5000

#define DOOR_UNLOCK_TIMEOUT_BUTTON 5000
#define DOOR_UNLOCK_TIMEOUT_FRONT 10000
#define DOOR_UNLOCK_TIMEOUT_DISCORD 30000
#define DOOR_UNLOCK_TIMEOUT_WEB 30000
#define DOOR_UNLOCK_TIMEOUT_DEFAULT 5000


#define ACTIONSOURCE_MAXVALUE 4
enum ActionSource
{
    SRCNONE = 0,
    Button,
    Front,
    Discord,
    Web
};

#define ACTION_MAXVALUE 4
enum Action
{
    ACTNONE = 0,
    UNLOCK,
    ENGAGE,
    DISENGAGE,
    ABORTALARM
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
    static ActionSource TargetActionSource;

    static ulong Time_DoorLocked;

    static ulong UnlockTimeout;

    static DoorState CurrentState;
    
    static void t_DoorHandler(void *args);

    static void AlarmCheck();
    static void PreAlarm();
    static void Alarm();

    static void action_Lock(bool useFeedback = true);
    static void action_Unlock(ActionSource src, bool useFeedback = true);
    static void action_Engage(bool useFeedback = true);
    static void action_Disengage(bool useFeedback = true);

    static void ExecuteAction(Action act, ActionSource src);


    static Melody melody_Unlock;
    static Melody melody_Lock;
    static Melody melody_Disengage;
    static Melody melody_Engage;

    static void InitializeMelodies();


    public:
    static void Initialize();
    static ActionRequestResult Unlock(ActionSource src);
    static ActionRequestResult Engage(ActionSource src);
    static ActionRequestResult Disengage(ActionSource src);
};