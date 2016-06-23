#pragma once

#include <iostream>
#include <random>

//Include generated proto header file
#include "PositionSensor.pb.h"

//Master Send/Recieve module (a.k.a. NCommManager)
//#include "NCommManager.h"

class CPositionSensor
{
    public:
        //Default Constructor that initializes the random num generator
        CPositionSensor();

        //The main update loop of this sensor
        PositionSensorMessage Update();

    private:
        //The unique id of this plugin
        int m_id;
        
        //Private members for the random device
        std::random_device m_rd;
        std::mt19937 m_gen;
        std::uniform_int_distribution<> m_dis;
};

