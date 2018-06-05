//
// Created by zinjkov on 01.04.18.
//

#include "yaw_regulator.hpp"
namespace rov {
    yaw_regulator::yaw_regulator() :basic_regulator(3), m_yaw_to_set(0) {

    }

    yaw_regulator::~yaw_regulator() {

    }

    void yaw_regulator::apply(std::vector<int8_t> &force, const rov_types::rov_control &rc,
                              const rov_types::rov_telimetry &rt, const basic_regulator::regulator_config &config) {
        if (rc.axis_w == 0) {
            if (config.enabled_pd.yaw_pd == 1) {

                if (!m_timeout.is_started()) {
                    m_timeout.start();
                    return;
                }

                if (m_timeout.elapsed() < 500) {
                    return;
                }

                float yaw_real = to360(rt.yaw);
                float yaw_to = to360(m_yaw_to_set);
                float err = normilize360(yaw_to - yaw_real);

                err = to180(err);

                std::int8_t w = pid_strategy(config.pd.yaw_p, config.pd.yaw_i, config.pd.yaw_d, err);

                force[0] -= w;
                force[1] += w;
                force[2] -= w;
                force[3] += w;

            }
        } else {
            m_timeout.stop();
            m_yaw_to_set = rt.yaw;
        }
    }

}