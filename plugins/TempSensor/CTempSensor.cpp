#include "CTempSensor.h"

CTempSensor::CTempSensor()
    : m_gen(m_rd())
    , m_dis(20,30)
    , m_id(38)
{
}

TempSensorMessage CTempSensor::Update()
{
    //Create a message
    TempSensorMessage outMessage;

    //Set the id
    outMessage.data.pid = m_id;

    //Select what we want to send
    outMessage.data.which_payload = TempSensor_telemetry_tag;
    
    //Write random data for the update loop
    auto temp = m_dis(m_gen);
    outMessage.data.payload.telemetry = {temp};

    return outMessage;
}