//
// Created by zinjkov on 01.04.18.
//

#include "pitch_roll_regulator.hpp"

rov::pitch_roll_regulator::pitch_roll_regulator() : basic_regulator(5){

}

rov::pitch_roll_regulator::~pitch_roll_regulator() {

}

void rov::pitch_roll_regulator::apply(rov_types::rov_hardware_control &thruster, const rov_types::rov_control &rc,
                                      const rov_types::rov_telimetry &rt,
                                      const rov::basic_regulator::regulator_config &config) {


    int p[4] = {
            thruster.vertical_power[0],
            thruster.vertical_power[1],
            thruster.vertical_power[2],
            thruster.vertical_power[3]
    };

    if (config.enabled_pd.roll_pd == 1) {
        std::cout << "roll enabled " << std::endl;
        float roll_real = to360(rt.roll);
        float roll_to = to360(config.pd.roll_to_set);
        float err = normilize360(roll_to - roll_real);

        err = to180(err);

        std::int8_t z_roll = pd_strategy(config.pd.roll_p, config.pd.roll_d, err);
        std::cout << "error: " << err << " z_depth: " << z_roll << std::endl;
        p[0] += -z_roll;
        p[1] += -z_roll;
        p[2] += z_roll;
        p[3] += z_roll;

    }

    if (config.enabled_pd.pitch_pd == 1) {
        std::cout << "pitch enabled " << std::endl;
        float pitch_real = to360(rt.pitch);
        float pitch_to = to360(config.pd.pitch_to_set);
        float err = normilize360(pitch_to - pitch_real);

        err = to180(err);

        std::int8_t z_pitch = pd_strategy(config.pd.pitch_p, config.pd.pitch_d, err);
        std::cout << "error: " << err << " z_depth: " << z_pitch << std::endl;
        p[0] += -z_pitch;
        p[1] += z_pitch;
        p[2] += -z_pitch;
        p[3] += z_pitch;
    }

    for (int i = 0; i < 4; i++) {
        thruster.vertical_power[i] = constrain(p[i]);
    }



}
