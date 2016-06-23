#include "CPositionSensor.h"

CPositionSensor::CPositionSensor()
    : m_gen(m_rd())
    , m_dis(1,100)
    , m_id(45)
{
}

PositionSensorMessage CPositionSensor::Update()
{
    //Create a message
    PositionSensorMessage outMessage;

    //Set the id
    outMessage.data.pid = m_id;

    //Select what we want to send
    outMessage.data.which_payload = PositionSensor_telemetry_tag;
    
    //Write random data for the update loop
    auto x = m_dis(m_gen);
    auto y = m_dis(m_gen);
    outMessage.data.payload.telemetry = {x, y};

    return outMessage;
}

