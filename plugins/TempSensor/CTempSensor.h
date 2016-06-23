#pragma once

#include <iostream>
#include <random>

//Include generated proto header file 
#include "TempSensor.pb.h"

//Master Send/Recieve module (a.k.a. NCommManager)
//#include "NCommManager.h"

class CTempSensor
{
    public:
        //Default Constructor that initializes the random num generator 
        CTempSensor();

        //The main update loop of this sensor
        TempSensorMessage Update();

    private:
        //The unique id of this plugin
        int m_id;

        //Private members for the random device
        std::random_device m_rd;
        std::mt19937 m_gen;
        std::uniform_real_distribution<> m_dis;
};