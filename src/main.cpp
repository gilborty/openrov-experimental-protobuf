#include <iostream>
#include <random>
#include <chrono>
#include <thread>

//Our NCommManager
#include "NCommManager.h"

//ROV Base messages
#include "Master.pb.h"
#include "ROVMessage.h"

//Our Sensors
#include "PoseSensor/CPoseSensor.h"


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
    //Manager to recieve data
    NCommManager manager;

    //Our sensors
    CPoseSensor poseSensor;

    while(true)
    {
        //Generate random switch
        if(RNG.GetRandomBool() == 1)
        {
            auto response = poseSensor.Update();
            if(response.first)
            {
                manager.RecieveMessage(response.second);
            }
            else
            {
                std::cout << "Unable to get update from Pose Sensor" << std::endl;
            }
        }
        else
        {
            CTemperatureMessage tempMessage;
            TemperatureMessage messageContents{RNG.GetRandomFloat()};
            tempMessage.Pack(messageContents);
            auto response = manager.SendMessage(tempMessage);
            manager.RecieveMessage(response.second);
        }
        auto sleepForDuration = 250;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepForDuration));      
        std::cout << std::endl;
    }

    return 0;
}