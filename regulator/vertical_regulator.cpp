//
// Created by zinjkov on 01.04.18.
//

#include "vertical_regulator.hpp"

rov::vertical_regulator::vertical_regulator() {

}

rov::vertical_regulator::~vertical_regulator() {

}

void rov::vertical_regulator::apply(rov_types::rov_hardware_control &thruster, const rov_types::rov_control &rc,
                                    const rov_types::rov_telimetry &rt,
                                    const rov::basic_regulator::regulator_config &config) {
    thruster.vertical_power[0] = rc.axis_z;
    thruster.vertical_power[1] = rc.axis_z;
    thruster.vertical_power[2] = rc.axis_z;
    thruster.vertical_power[3] = rc.axis_z;
}
