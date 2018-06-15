//
// Created by zinjkov on 01.04.18.
//

#include <cmath>
#include "basic_regulator.hpp"
namespace rov {
    basic_regulator::basic_regulator(std::uint8_t id) : m_id(id), m_prev_error(0) {
        m_timer.start();
    }

    basic_regulator::~basic_regulator() {

    }

    std::int8_t basic_regulator::constrain(int p) {
        if (p > 100) p = 100;
        if (p < -100) p = -100;
        return static_cast<std::int8_t >(p);
    }

    std::uint8_t basic_regulator::get_id() const {
        return m_id;
    }

    bool basic_regulator::operator==(const basic_regulator &regualtor) {
        return get_id() == regualtor.get_id();
    }

    std::int8_t basic_regulator::diff_strategy(float kd, float error) {
        int to_ret = static_cast<int>(kd * (error - m_prev_error));
        m_prev_error = error;
        return constrain(to_ret);
    }

    std::int8_t basic_regulator::int_strategy(float ki, float error) {
        float last_update = m_timer.elapsed() / (float)1000;
        auto to_ret = static_cast<int>(ki *  error * last_update + m_prev_integral);
        m_prev_integral = to_ret;
        if (m_prev_error * error < 0) {
            m_prev_integral = 0;
        }
        return constrain(to_ret);
    }

    std::int8_t basic_regulator::prop_strategy(float kp, float error) {
        auto to_ret = static_cast<int>(kp * error);
        return constrain(to_ret);
    }

    std::int8_t basic_regulator::pid_strategy(float p, float i, float d, float error) {
        auto to_ret = prop_strategy(p, error) + int_strategy(i, error) + diff_strategy(d, error);
        m_timer.restart();
        return constrain(to_ret);
    }

    int basic_regulator::constrainto(int p, int v) {
        if (p > v) {
            p = v;
        }
        if (p < -v) {
            p = -v;
        }
        return p;
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


