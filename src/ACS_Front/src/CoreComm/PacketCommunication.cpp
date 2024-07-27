#include "CoreComm/PacketCommunication.h"

namespace PacketCommunication
{
    HardwareSerial _serial;

    void Initialize(HardwareSerial _s)
    {
        _serial = _s;
    }
} 

// namespace name
