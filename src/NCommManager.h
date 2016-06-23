#pragma once

#include <iostream>

#include "ROVMessage.h"

#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

//Since we are doing this in memory, we need a data structure that gives us details of the buffer being sent
struct BufferInfo
{
    size_t bufferLength;
    void *buffer;
};
//Since we are doing this in memory, let's return the encoded message (a pointer to the buffer)
using SendResponse = std::pair<bool,BufferInfo>;

//Since all of our buffers are of type uint8_t, lets have a quick convience function to calcualte the size
#define BUFFER_SIZE(a) (sizeof(a)/sizeof(*a))

class NCommManager
{
    public:
        SendRespone Send(ROVMessage& message);
        void Recieve(uint8_t* encodedMessage);

    private:

};


