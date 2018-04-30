//
// Created by zinjkov on 01.04.18.
//

#include "horizontal_regulator.hpp"

rov::horizontal_regulator::horizontal_regulator() : basic_regulator(2) {

}

rov::horizontal_regulator::~horizontal_regulator() {

}

uint8_t constrain(int val) {
    if (val < -100) {
        val = -100;
    }
    if (val > 100) {
        val = 100;
    }
    return static_cast<uint8_t>(val);
}

void rov::horizontal_regulator::apply(rov_types::rov_hardware_control &thruster, const rov_types::rov_control &rc,
                                      const rov_types::rov_telimetry &rt,
                                      const rov::basic_regulator::regulator_config &config) {

    thruster.horizontal_power[0] = constrain75(rc.axis_y + rc.axis_y + rc.axis_w);
    thruster.horizontal_power[1] = constrain75(rc.axis_y - rc.axis_y - rc.axis_w);
    thruster.horizontal_power[2] = constrain75(rc.axis_y - rc.axis_y + rc.axis_w);
    thruster.horizontal_power[3] = constrain75(rc.axis_y + rc.axis_y - rc.axis_w);
    constrain_horizontal(thruster);
}

