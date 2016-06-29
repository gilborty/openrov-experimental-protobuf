#include <iostream>
#include <random>
#include <chrono>
#include <thread>

#include "common/NCommManager.h"
#include "PoseMessage.pb.h"
#include "Master.pb.h"
#include "ROVMessage.h"

#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

class RandomNumberGen
{
    public:

        RandomNumberGen()
        : m_gen(m_rd())
        , m_boolDis(1,2)
        , m_intDis(1, 1000)
        , m_floatDis(0, 100)
        {


        }
        int GetRandomInt()
        {
            return m_intDis(m_gen);
        }
        int GetRandomBool()
        {
            return m_boolDis(m_gen);
        }
        float GetRandomFloat()
        {
            return static_cast<float>(m_floatDis(m_gen));
        }
        
    private:
        std::random_device m_rd;
        std::mt19937 m_gen;

        std::uniform_int_distribution<> m_boolDis;
        std::uniform_int_distribution<> m_intDis;
        std::uniform_real_distribution<> m_floatDis;
}RNG;


int main(int argc, char* argv[])
{
    NCommManager manager;
    while(true)
    {
        //Generate random switch
        if(RNG.GetRandomBool() == 1)
        {
            //std::cout << "Sending a pose message" << std::endl;
            CPoseMessage posMessage;
            PoseMessage messageContents{RNG.GetRandomInt()};
            posMessage.Pack(messageContents);
            auto response = manager.SendMessage(posMessage);
            manager.RecieveMessage(response.second);
        }
        else
        {
            //std::cout << "Sending a temperature message" << std::endl;
            CTemperatureMessage tempMessage;
            TemperatureMessage messageContents{RNG.GetRandomFloat()};
            tempMessage.Pack(messageContents);
            auto response = manager.SendMessage(tempMessage);
            manager.RecieveMessage(response.second);
        }
        auto sleepForDuration = RNG.GetRandomInt();
        std::cout << "SLEEPING FOR: "  << sleepForDuration << " ms" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepForDuration));      
        std::cout << std::endl;
    }

    return 0;
}