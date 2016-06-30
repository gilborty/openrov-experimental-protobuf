#include "CTemperatureSensor.h"

CTemperatureSensor::CTemperatureSensor()
    : m_gen(m_rd())
    , m_dis(1,100)
{
}

SendResponse CTemperatureSensor::Update()
{
    //Create a message handler
    CTemperatureMessage tempMessage;

    //Create a random payload
    TemperatureMessage payload{static_cast<float>(m_dis(m_gen))};

    //Pack the payload
    tempMessage.Pack(payload);

    //Use the return from send 
    return m_manager.SendMessage(tempMessage);
}
