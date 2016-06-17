#pragma once

#include <iostream>

#include "ROVMessage.h"

#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

class NCommManager
{
    public:
        bool Send(ROVMessage& message);
};

