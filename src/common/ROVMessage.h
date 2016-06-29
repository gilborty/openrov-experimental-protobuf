#pragma once

#include "pb.h"
#include "MessageTypes.h"

class ROVMessage
{
    public:

        //Pure virtual functions. Your message class must implement these
        virtual size_t GetSize() = 0;
        virtual const pb_field_t* GetFields() = 0;
        //virtual T GetData() = 0;
        virtual void* GetData() = 0;

        void SetMessageType(EMessageType typeIn)
        {
            messageType = typeIn;
        }

        EMessageType messageType = EMessageType::DefaultMessage;
};



