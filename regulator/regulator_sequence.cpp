//
// Created by zinjkov on 01.04.18.
//

#include "regulator_sequence.hpp"
#include "horizontal_regulator.hpp"
#include "vertical_regulator.hpp"

rov::regulator_sequence::regulator_sequence() {

}

rov::regulator_sequence::~regulator_sequence() {

}

int8_t constrainto(int p, int8_t v) {
    if (p > v) {
        p = v;
    }
    if (p < -v) {
        p = -v;
    }
    return static_cast<int8_t>(p);
}

void rov::regulator_sequence::apply_horizontal(rov_types::rov_hardware_control &thruster,
                                              const rov_types::rov_control &rc,
                                              const rov_types::rov_telimetry &rt,
                                              const rov::basic_regulator::regulator_config &config) {

    std::vector<int8_t> force = {0, 0, 0, 0};


    for(auto &regulator : m_horizontal_sequence) {
        regulator->apply(force, rc, rt, config);
    }

    horizontal_regulator hr;
    hr.apply(thruster, rc, rt, config);

    int8_t diff = calc_diff(force, thruster.horizontal_power);

    for (int i = 0; i < 4; i++) {
        thruster.horizontal_power[i] = constrainto(thruster.horizontal_power[i], static_cast<int8_t>(100 - diff));
        thruster.horizontal_power[i] += force[i];
    }


}

void rov::regulator_sequence::apply_vertical(rov_types::rov_hardware_control &thruster,
                                            const rov_types::rov_control &rc,
                                            const rov_types::rov_telimetry &rt,
                                            const rov::basic_regulator::regulator_config &config) {
    std::vector<int8_t> force = {0, 0, 0, 0};

    for(auto &regulator : m_vetical_sequence) {
        regulator->apply(force, rc, rt, config);
    }

    vertical_regulator vr;
    vr.apply(thruster, rc, rt, config);

    int8_t diff = calc_diff(force, thruster.vertical_power);

    for (int i = 0; i < 4; i++) {
        thruster.vertical_power[i] = constrainto(thruster.vertical_power[i], static_cast<int8_t>(100 - diff));
        thruster.vertical_power[i] += force[i];
    }

}

int8_t rov::regulator_sequence::calc_diff(std::vector<int8_t> &force, int8_t *thrusters) {
    int max_force = 0;
    for (const auto &th : force) {
        if (max_force < abs(th)) {
            max_force = abs(th);
        }
    }

    int max_throttle = 0;
    for (int i = 0; i < 4; i++) {
        if (max_throttle < abs(thrusters[i])) {
            max_throttle = abs(thrusters[i]);
        }
    }
    int8_t diff = 0;
    if (max_force + max_throttle > 100) {
        diff = static_cast<int8_t>(max_force + max_throttle - 100);
    }
    return diff;
}
