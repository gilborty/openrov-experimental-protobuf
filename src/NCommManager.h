#pragma once

#include <iostream>

#include "ROVMessage.h"

#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

//Since we are doing this in memory, let's return the encoded message (a pointer to the buffer)
using SendResponse = std::pair<bool,uint8_t*>;
class NCommManager
{
    public:
        SendRespone Send(ROVMessage& message);
        void Recieve(MasterMessage& message);
};

