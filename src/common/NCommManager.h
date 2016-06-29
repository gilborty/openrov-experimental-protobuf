#pragma once

#include <iostream>
#include <utility>

//Include nanopb library
#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

//Include the Base ROV message class
#include "ROVMessage.h"

//The master message list
#include "Master.pb.h"

//Since we are doing this in memory, we need a data structure that gives us details of the buffer being sent
struct BufferInfo
{
    uint8_t* buffer;
    size_t bufferSize;
};
//Since we are doing this in memory, let's return the encoded message (a pointer to the buffer)
using SendResponse = std::pair<bool,BufferInfo>;

class NCommManager
{
    public:
        bool RecieveMessage(BufferInfo& messageIn);
        SendResponse SendMessage(ROVMessage& messageOut);

    private:
        SendResponse Encode(const MasterMessage& messageToBeEncoded);
        
        /*Message callbacks*/
        void HandleRecievedMessage(const TemperatureMessage& messageIn);
        void HandleRecievedMessage(const PoseMessage& messageIn);
};


