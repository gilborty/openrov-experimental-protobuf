#include "NCommManager.h"

SendResponse NCommManager::SendMessage(ROVMessage& messageOut)
{
    std::cout << "=========" << std::endl;
    //Create a master message that we will pack and send
    MasterMessage master;

    //Switch based on the type of message recieved the user wants to send
    switch(messageOut.messageType)
    {
        case(EMessageType::PoseMessage):
        {
        	std::cout << "SENDING: PoseMessage" << std::endl;
        	master.type = static_cast<int>(EMessageType::PoseMessage);
        	master.which_payload = MasterMessage_poseMessage_tag;
        	master.payload.poseMessage = *static_cast<PoseMessage*>(messageOut.GetData());
        	break;
        }
        case(EMessageType::TemperatureMessage):
        {
        	std::cout << "SENDING: TemperatureMessage" << std::endl;
        	master.type = static_cast<int>(EMessageType::TemperatureMessage);
        	master.which_payload = MasterMessage_temperatureMessage_tag;
        	master.payload.temperatureMessage = *static_cast<TemperatureMessage*>(messageOut.GetData());
        	break;
        }
        default:
        {
            std::cout << "SEND FAILED: Unknown message type" << std::endl;
            return std::make_pair(false, BufferInfo{});
            break;
        }
    }

    auto encodeResponse = Encode(master);

    if(!encodeResponse.first)
    {
        std::cout << "Encoding failed!" << std::endl;
        return std::make_pair(false, BufferInfo{});
    };
    return (true,encodeResponse);
}

SendResponse NCommManager::Encode(const MasterMessage& messageToBeEncoded)
{
    size_t bufferSize = MasterMessage_size;

    uint8_t buffer[bufferSize];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    
    if(pb_encode(&stream, MasterMessage_fields, &messageToBeEncoded))
    {
        BufferInfo successfulMessage;
        successfulMessage.bufferSize = stream.bytes_written;
        
        uint8_t* newArray = new uint8_t[bufferSize];
        memcpy(newArray, buffer, bufferSize*sizeof(uint8_t));
        successfulMessage.buffer = newArray;

        return std::make_pair(true, successfulMessage);  
    }
    else
    {
        std::cout << "Encode Failed!" << std::endl;
        return std::make_pair(false, BufferInfo{});
    }
}

bool NCommManager::RecieveMessage(BufferInfo& message)
{    
    pb_istream_t decodeStream = pb_istream_from_buffer(message.buffer, message.bufferSize);
    MasterMessage decodedMessage;

    if (!pb_decode(&decodeStream, MasterMessage_fields, &decodedMessage))
    {
        std::cout << "Decode failed" << std::endl;
        return false;
    }

    //Figure out how we are going to handle this messageType
    switch(static_cast<EMessageType>(decodedMessage.type))
    {
        case(EMessageType::PoseMessage):
        {
        	std::cout << "RECIEVED: PoseMessage" << std::endl;
        	HandleRecievedMessage(decodedMessage.payload.poseMessage);
        	break;
        }
        case(EMessageType::TemperatureMessage):
        {
        	std::cout << "RECIEVED: TemperatureMessage" << std::endl;
        	HandleRecievedMessage(decodedMessage.payload.temperatureMessage);
        	break;
        }
        default:
        {
            std::cout << "RECIEVE FAILED: Unknown message type" << std::endl;
            return false;
            break;
        }
    }
    std::cout << "=========" << std::endl;
    
    delete[] message.buffer;
    message.buffer = nullptr;
}

void NCommManager::HandleRecievedMessage(const TemperatureMessage& messageIn)
{
    std::cout << "Value: " << messageIn.temperature << std::endl;
}

void NCommManager::HandleRecievedMessage(const PoseMessage& messageIn)
{
    std::cout << "Value: " << messageIn.x << std::endl;
}