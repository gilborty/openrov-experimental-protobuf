#pragma once

#include "pb.h"
class ROVMessage
{
    public:

        //Pure virtual functions. Your message class must implement these
        virtual size_t GetSize() = 0;
        virtual const pb_field_t* GetFields() = 0;
};
