#include "NCommManager.h"
#include "CPositionSensor.h"

#include "PositionSensor.pb.h"

#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

int main(int argc, char* argv[])
{
    std::cout << "Starting..." << std::endl;
    NCommManager manager;
    CPositionSensor posSensor;

    auto message = posSensor.Update();

    manager.Send(message);


    return 0;
}