//
// Created by zinjkov on 01.04.18.
//

#include "regulator_sequence.hpp"

rov::regulator_sequence::regulator_sequence() {

}

rov::regulator_sequence::~regulator_sequence() {

}

void rov::regulator_sequence::apply_horizontal(rov_types::rov_hardware_control &thruster,
                                              const rov_types::rov_control &rc,
                                              const rov_types::rov_telimetry &rt,
                                              const rov::basic_regulator::regulator_config &config) {
    for(auto &regulator : m_horizontal_sequence) {
        regulator->apply(thruster, rc, rt, config);
    }
}

void rov::regulator_sequence::apply_vertical(rov_types::rov_hardware_control &thruster,
                                            const rov_types::rov_control &rc,
                                            const rov_types::rov_telimetry &rt,
                                            const rov::basic_regulator::regulator_config &config) {
    for(auto &regulator : m_vetical_sequence) {
        regulator->apply(thruster, rc, rt, config);
    }
}
