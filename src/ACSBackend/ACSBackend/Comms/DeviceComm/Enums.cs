namespace ACSBackend.Comms.DeviceComm
{
    public enum DeviceType
    {
        CORE,
        FRONT
    }

    public enum DeviceAction
    {
        NONE = 0,
        Unlock,
        Engage,
        Disengage,
        AbortAlarm
    }

    public enum DeviceActionSource
    {
        NONE = 0,
        Button,
        Front,
        Discord,
        Web
    }

    public enum ActionRequestResult
    {
        ERROR = -1,
        SUCCESS = 0,
        ALREADY_UNLOCKED,
        ALREADY_ENGAGED,
        ALREADY_DISENGAGED,
        ALARM_NOT_ACTIVE
    }
}
