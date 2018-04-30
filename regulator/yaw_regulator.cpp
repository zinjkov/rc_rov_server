//
// Created by zinjkov on 01.04.18.
//

#include "yaw_regulator.hpp"
namespace rov {
    yaw_regulator::yaw_regulator() :m_yaw_to_set(0), basic_regulator(3) {

    }

    yaw_regulator::~yaw_regulator() {

    }



    void yaw_regulator::apply(rov_types::rov_hardware_control &thruster, const rov_types::rov_control &rc,
                              const rov_types::rov_telimetry &rt, const basic_regulator::regulator_config &config) {

        if (rc.axis_w == 0) {
            if (config.enabled_pd.yaw_pd == 1) {

                std::cout << "yaw enabled " << std::endl;
                float yaw_real = to360(rt.yaw);
                float yaw_to = to360(m_yaw_to_set);
                float err = normilize360(yaw_to - yaw_real);

                err = to180(err);

                std::int8_t w = pd_strategy(config.pd.yaw_p, config.pd.yaw_d, err);
                std::cout << "error: " << err << " z_depth: " << w << std::endl;
                int p[4] = {
                        thruster.horizontal_power[0],
                        thruster.horizontal_power[1],
                        thruster.horizontal_power[2],
                        thruster.horizontal_power[3]
                };

                thruster.horizontal_power[0] = constrain(p[0] - w);
                thruster.horizontal_power[1] = constrain(p[1] + w);
                thruster.horizontal_power[2] = constrain(p[2] - w);
                thruster.horizontal_power[3] = constrain(p[3] + w);
                constrain_horizontal(thruster);
            }
        } else {
            m_yaw_to_set = rt.yaw;
        }

    }


}