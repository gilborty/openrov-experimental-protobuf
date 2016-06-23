#include "NCommManager.h"

SendResponse NCommManager::Send(ROVMessage& message)
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

        //Pack the struct for returning
        BufferInfo info = {BUFFER_SIZE(buffer), buffer};
        return {true, info};
    }
    else
    {
        std::cout << "Encode FAILED." << std::endl;
        return {false, nullptr};
    }
    return {false, nullptr};
}

uint8_t* NCommManager::Recieve(BufferInfo encodedMessage)
{
    //Create a nanopb decode stream from the buffer we just recieved
    

}
