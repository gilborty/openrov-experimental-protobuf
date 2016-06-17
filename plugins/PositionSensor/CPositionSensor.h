#pragma once

#include <iostream>
#include <random>

#include "PositionSensor.pb.h"
#include "NCommManager.h"

class CPositionSensor
{
    public:
        CPositionSensor();

        PositionSensorMessage Update();

    private:
        std::random_device m_rd;
        std::mt19937 m_gen;
        std::uniform_int_distribution<> m_dis;
        
        int m_id;
};

