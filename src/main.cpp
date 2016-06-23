#include "NCommManager.h"
#include "CPositionSensor.h"

#include "PositionSensor.pb.h"

#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

int main(int argc, char* argv[])
{
    std::cout << "Starting..." << std::endl;

    //Master messaging class
    NCommManager manager;

    //We are doing this in memory, not network
    //So let's create an instance of each class to get data from
    CTempSensor tempSensor;
    CPositionSensor posSensor;

    //Get a message from a random sensor
    //TODO: Pick a random message(ie switch/case)
    auto message = posSensor.Update();

    //And send it. This will be where it gets sent over serial
    if(manager.Send(message))
    {
        std::cout << "Message was successfully sent.";
    }
    else
    {
        std::cout << "Uh oh. Message failed to send" << std::endl;
    }


    return 0;
}