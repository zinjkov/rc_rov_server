//
// Created by zinjkov on 01.04.18.
//

#include "basic_regulator.hpp"
namespace rov {
    basic_regulator::basic_regulator(std::uint8_t id) : m_id(id), m_prev_error(0) {

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
        return static_cast<std::int8_t >(p);
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

    std::uint8_t basic_regulator::get_id() const {
        return m_id;
    }

    bool basic_regulator::operator==(const basic_regulator &regualtor) {
        return get_id() == regualtor.get_id();
    }

    std::int8_t basic_regulator::constrain75(int p) {
        int val = 75;
        if (p > val) p = val;
        if (p < -val) p = -val;
        return static_cast<std::int8_t >(p);

    }

    std::int8_t basic_regulator::diff_strategy(float kd, float error) {
        int to_ret = static_cast<int>(kd * (m_prev_error - error));
        m_prev_error = error;
        return constrain(to_ret);
    }

    std::int8_t basic_regulator::prop_strategy(float kp, float error) {
        int to_ret = static_cast<int>(kp * error);
        return constrain(to_ret);
    }

    std::int8_t basic_regulator::pd_strategy(float kp, float kd, float error) {
        auto to_ret = prop_strategy(kp, error) + diff_strategy(kd, error);
        return constrain(to_ret);
    }


    float to360(float angle) {
        return angle > 0 ? angle : (360 + angle);
    }

    float to180(float angle) {
        if (angle > 180) {
            angle -= 360;
        }
        return angle;
    }

    float normilize360(float angle) {
        if (angle < 0) {
            angle += 360;
        }

        if (angle > 360) {
            angle -= 360;
        }

        return angle;

    }


}


