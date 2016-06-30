#pragma once

#include <iostream>
#include <random>

//Include generated proto header file
#include "TemperatureMessage.pb.h"

//Master Send/Recieve module (a.k.a. NCommManager)
#include "NCommManager.h"


class CTemperatureSensor
{
    public:
        //Default Constructor that initializes the random num generator
        CTemperatureSensor();

        //The main update loop of this sensor
        SendResponse Update();

    private:
        //The unique id of this plugin
        NCommManager m_manager;
        
        //Private members for the random device
        std::random_device m_rd;
        std::mt19937 m_gen;
        std::uniform_real_distribution<> m_dis;
};