//
// Created by zinjkov on 01.04.18.
//

#include "basic_regulator.hpp"
namespace rov {
    basic_regulator::basic_regulator() {

    }

    basic_regulator::~basic_regulator() {

    }

    void basic_regulator::constrain_thruster(rov_types::rov_hardware_control &thruster) {
        constrain_horizontal(thruster);
        constrain_vertical(thruster);
    }

    std::int8_t basic_regulator::constrain(int p) {
        if (p > 100) p = 100;
        if (p < -100) p = -100;
        return p;
    }

    void basic_regulator::constrain_horizontal(rov_types::rov_hardware_control &thruster) {
        for (auto &p : thruster.horizontal_power) {
            p = constrain(p);
        }
    }

    void basic_regulator::constrain_vertical(rov_types::rov_hardware_control &thruster) {
        for (auto &p : thruster.vertical_power) {
            p = constrain(p);
        }
    }
}


