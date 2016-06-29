#include "NCommManager.h"

SendResponse NCommManager::SendMessage(ROVMessage& messageOut)
{
    std::cout << "=========" << std::endl;
    //Create a master message that we will pack and send
    MasterMessage master;

    //Switch based on the type of message recieved the user wants to send
    switch(messageOut.messageType)
    {
        /*[[[cog
        import cog

        def first_letter_to_lower(stringIn):
            return stringIn[0].lower() + stringIn[1:]

        messages = ['PoseMessage', 'TemperatureMessage']

        for message in messages:
            cog.outl("case(EMessageType::%s):" % message)
            cog.outl("{")
            cog.outl("\tstd::cout << \"SENDING: %s\" << std::endl;" % message)
            cog.outl("\tmaster.type = static_cast<int>(EMessageType::%s);" % message)
            cog.outl("\tmaster.which_messages = MasterMessage_%s_tag;" % first_letter_to_lower(message))
            cog.outl("\tmaster.messages.%s = *static_cast<%s*>(messageOut.GetData());" % (first_letter_to_lower(message), message))
            cog.outl("\tbreak;")
            cog.outl("}")
        ]]]*/
        //
        //[[[end]]]
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
        /*[[[cog
        import cog

        def first_letter_to_lower(stringIn):
            return stringIn[0].lower() + stringIn[1:]

        messages = ['PoseMessage', 'TemperatureMessage']

        for message in messages:
            cog.outl("case(EMessageType::%s):" % message)
            cog.outl("{")
            cog.outl("\tstd::cout << \"RECIEVED: %s\" << std::endl;" % message)
            cog.outl("\tHandleRecievedMessage(decodedMessage.messages.%s);" % first_letter_to_lower(message))
            cog.outl("\tbreak;")
            cog.outl("}")
        ]]]*/
        //
        //[[[end]]]
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