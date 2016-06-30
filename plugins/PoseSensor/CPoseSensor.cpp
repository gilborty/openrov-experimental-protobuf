#include "CPoseSensor.h"

CPoseSensor::CPoseSensor()
    : m_gen(m_rd())
    , m_dis(1,100)
{
}

SendResponse CPoseSensor::Update()
{
    //Create a message handler
    CPoseMessage poseMessage;

    //Create a random payload
    PoseMessage payload{m_dis(m_gen)};

    //Pack the payload
    poseMessage.Pack(payload);

    //Use the return from send 
    return m_manager.SendMessage(poseMessage);
}
