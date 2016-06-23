#include "NCommManager.h"

bool NCommManager::Send(ROVMessage& message)
{
    std::cout << "Packing message" << std::endl;

    /*Start encoding the message*/
    //Allocate space in a buffer
    uint8_t buffer[message.GetSize()];

    //Create the nanopb encode (out) stream
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    //And pack the buffer
    if(pb_encode(&stream, message.GetFields(), &message))
    {
        std::cout << "Encode successful." << std::endl;
        return {true, buffer};
    }
    else
    {
        std::cout << "Encode FAILED." << std::endl;
        return {false, nullptr};
    }
    return {false, nullptr};
}
