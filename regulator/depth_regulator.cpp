//
// Created by zinjkov on 01.04.18.
//

#include "depth_regulator.hpp"

rov::depth_regulator::depth_regulator() {

}

rov::depth_regulator::~depth_regulator() {

}

void rov::depth_regulator::apply(rov_types::rov_hardware_control &thruster, const rov_types::rov_control &rc,
                                 const rov_types::rov_telimetry &rt,
                                 const rov::basic_regulator::regulator_config &config) {
    if (rc.axis_z == 0) {
        std::int8_t z_depth = 0;
        thruster.vertical_power[0] += z_depth;
        thruster.vertical_power[0] += z_depth;
        thruster.vertical_power[0] += z_depth;
        thruster.vertical_power[0] += z_depth;
    }

}
