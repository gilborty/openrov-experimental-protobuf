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
#include "TemperatureSensor/CTemperatureSensor.h"

//Random number generator
namespace rng
{
    std::random_device m_rd;
    std::mt19937 m_gen;

    int GetRandomSwitch()
    {
        auto intDis = std::uniform_int_distribution<>(1,2);
        return static_cast<int>(intDis(m_gen));
    }
};


int main(int argc, char* argv[])
{
    //Manager to recieve data
    NCommManager manager;

    //Our sensors
    CPoseSensor poseSensor;
    CTemperatureSensor tempSensor;

    while(true)
    {
        //Generate random switch
        if(rng::GetRandomSwitch() == 1)
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
            auto response = tempSensor.Update();
            if(response.first)
            {
                manager.RecieveMessage(response.second);
            }
            else
            {
                std::cout << "Unable to get update from Temperature Sensor" << std::endl;
            }
            
        }
        auto sleepForDuration = 250;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepForDuration));      
        std::cout << std::endl;
    }

    return 0;
}