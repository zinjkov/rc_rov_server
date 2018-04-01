//
// Created by zinjkov on 01.04.18.
//

#ifndef RC_ROV_SERVER_BASIC_REGULATOR_HPP
#define RC_ROV_SERVER_BASIC_REGULATOR_HPP

#include <vector>
#include <cstdint>
#include "../rov_types/rov_data_types.hpp"

namespace rov {
    class basic_regulator {
    public:
        struct regulator_config {
            float horizontal_multypliers[4] = {1};
            float vertical_multypliers[4] = {1};
            float yaw_to_set = 0;
            float yaw_multyplier = 1;
            float pitch_to_set = 0;
            float roll_to_set = 0;
            float depth_to_set = 0;
            float roll_multyplier = 1;
            float depth_multyplier = 1;
            float pitch_multyplier = 1;

        };

        basic_regulator();

        virtual ~basic_regulator();

        virtual void apply(rov_types::rov_hardware_control &thruster,
                           const rov_types::rov_control &rc,
                           const rov_types::rov_telimetry &rt,
                           const regulator_config &config) = 0;

    protected:
        void constrain_thruster(rov_types::rov_hardware_control &thruster);
        void constrain_horizontal(rov_types::rov_hardware_control &thruster);
        void constrain_vertical(rov_types::rov_hardware_control &thruster);
        std::int8_t constrain(std::int8_t p);
    };
}

#endif //RC_ROV_SERVER_BASIC_REGULATOR_HPP
