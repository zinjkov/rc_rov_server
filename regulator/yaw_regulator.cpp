//
// Created by zinjkov on 01.04.18.
//

#include "yaw_regulator.hpp"
namespace rov {
    yaw_regulator::yaw_regulator() {

    }

    yaw_regulator::~yaw_regulator() {

    }

    void yaw_regulator::apply(rov_types::rov_hardware_control &thruster, const rov_types::rov_control &rc,
                              const rov_types::rov_telimetry &rt, const basic_regulator::regulator_config &config) {

        if (rc.axis_w == 0) {
            std::int8_t w = static_cast<std::int8_t>((config.yaw_to_set - rt.yaw) * config.yaw_multyplier);
            thruster.horizontal_power[0] += w;
            thruster.horizontal_power[1] -= w;
            thruster.horizontal_power[2] += w;
            thruster.horizontal_power[3] -= w;
            constrain_horizontal(thruster);
        }

    }


}