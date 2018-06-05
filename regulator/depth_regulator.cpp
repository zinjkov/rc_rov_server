//
// Created by zinjkov on 01.04.18.
//

#include "depth_regulator.hpp"

rov::depth_regulator::depth_regulator() : basic_regulator(7), m_depth_to_set(0) {

}

rov::depth_regulator::~depth_regulator() {

}

void rov::depth_regulator::apply(std::vector<int8_t> &force, const rov_types::rov_control &rc,
                                 const rov_types::rov_telimetry &rt,
                                 const rov::basic_regulator::regulator_config &config) {
    if (rc.axis_z == 0) {
        if (config.enabled_pd.depth_pd == 1) {
            float error = m_depth_to_set - rt.depth;
            std::int8_t z_depth = pid_strategy(config.pd.depth_p, config.pd.depth_i, config.pd.depth_d, error);
            for (auto & th : force) {
                th -= z_depth;
            }
        }
    } else {
        m_depth_to_set = rt.depth;
    }
}

