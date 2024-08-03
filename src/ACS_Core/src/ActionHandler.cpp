#include "ActionHandler.h"

Action ActionHandler::TargetAction = Action::NONE;

void ActionHandler::Initialize()
{
    xTaskCreate(t_DoorHandler, "DoorHandler", 4096, NULL, 1, NULL);
}

void ActionHandler::ExecuteAction(Action act)
{
    TargetAction = act;
}


//----------------------------------------------------------
void AlarmCheck()
{
    
}

void ActionHandler::t_DoorHandler(void *args)
{
    for (;;)
    {
        
    }
}