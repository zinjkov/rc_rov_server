//
// Created by zinjkov on 19.05.18.
//

#include "roll_regulator.hpp"

rov::roll_regulator::roll_regulator() : basic_regulator(11){

}

rov::roll_regulator::~roll_regulator() {

}

void rov::roll_regulator::apply(std::vector<int8_t> &force, const rov_types::rov_control &rc,
                                const rov_types::rov_telimetry &rt,
                                const rov::basic_regulator::regulator_config &config) {

    if (config.enabled_pd.roll_pd == 1) {

        float roll_real = to360(rt.roll);
        float roll_to = to360(config.pd.roll_to_set);
        float err = normilize360(roll_to - roll_real);

        err = to180(err);

        std::int8_t z_roll = pid_strategy(config.pd.roll_p, config.pd.roll_i, config.pd.roll_d, err);
        force[0] += -z_roll;
        force[1] += -z_roll;

        force[2] += +z_roll;
        force[3] += +z_roll;

    }
}
