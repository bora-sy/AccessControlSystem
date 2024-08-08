#include "RemoteLogging.h"

void RemoteLogging::Initialize()
{
    udp.connect(Config::ServerIPAddress, 1256);
    
}
