//
// Created by zinjkov on 01.04.18.
//

#include "depth_regulator.hpp"

rov::depth_regulator::depth_regulator() : m_depth_to_set(0), basic_regulator(7) {

}

rov::depth_regulator::~depth_regulator() {

}

void rov::depth_regulator::apply(rov_types::rov_hardware_control &thruster, const rov_types::rov_control &rc,
                                 const rov_types::rov_telimetry &rt,
                                 const rov::basic_regulator::regulator_config &config) {
    if (rc.axis_z == 0) {
        if (config.enabled_pd.depth_pd == 1) {
            std::cout << "depth enabled " << std::endl;
            float error = m_depth_to_set - rt.depth;

            std::int8_t z_depth = pd_strategy(config.pd.depth_p, config.pd.depth_d, error);

            std::cout << "error: " << error << " z_depth: " << z_depth << std::endl;

            thruster.vertical_power[0] -= z_depth;
            thruster.vertical_power[1] -= z_depth;
            thruster.vertical_power[2] -= z_depth;
            thruster.vertical_power[3] -= z_depth;
        }
    } else {
        m_depth_to_set = rt.depth;
    }

}
