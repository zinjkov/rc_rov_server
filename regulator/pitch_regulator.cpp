//
// Created by zinjkov on 19.05.18.
//

#include "pitch_regulator.hpp"


rov::pitch_regulator::pitch_regulator() : basic_regulator(5) {

}

rov::pitch_regulator::~pitch_regulator() {

}

void rov::pitch_regulator::apply(std::vector<int8_t> &force, const rov_types::rov_control &rc,
                                 const rov_types::rov_telimetry &rt,
                                 const rov::basic_regulator::regulator_config &config) {
    if (config.enabled_pd.pitch_pd == 1) {

        float pitch_real = to360(rt.pitch);
        float pitch_to = to360(config.pd.pitch_to_set);
        float err = normilize360(pitch_to - pitch_real);

        err = to180(err);

        std::int8_t z_pitch = pid_strategy(config.pd.pitch_p, config.pd.pitch_i, config.pd.pitch_d, err);
        force[0] += -z_pitch;
        force[1] += z_pitch;
        force[2] += -z_pitch;
        force[3] += z_pitch;

    }

}
