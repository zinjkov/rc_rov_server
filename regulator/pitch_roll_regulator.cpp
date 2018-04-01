//
// Created by zinjkov on 01.04.18.
//

#include "pitch_roll_regulator.hpp"

rov::pitch_roll_regulator::pitch_roll_regulator() {

}

rov::pitch_roll_regulator::~pitch_roll_regulator() {

}

void rov::pitch_roll_regulator::apply(rov_types::rov_hardware_control &thruster, const rov_types::rov_control &rc,
                                      const rov_types::rov_telimetry &rt,
                                      const rov::basic_regulator::regulator_config &config) {

    std::int8_t z_roll = static_cast<std::int8_t>((config.roll_to_set - rt.roll) * config.roll_multyplier);
    std::int8_t z_pitch = static_cast<std::int8_t>((config.pitch_to_set - rt.pitch) * config.pitch_multyplier);

    thruster.vertical_power[0] += (z_roll + z_pitch);
    thruster.vertical_power[1] += (-z_roll + z_pitch);
    thruster.vertical_power[2] += (z_roll - z_pitch);
    thruster.vertical_power[3] += (-z_roll - z_pitch);

    constrain_vertical(thruster);

}
