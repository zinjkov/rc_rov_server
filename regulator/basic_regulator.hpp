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
            rov_types::rov_pd pd;
            rov_types::rov_enable_pd enabled_pd;
        };

        struct thruster_place_config {
            enum : uint8_t {
                hor_front_left = 0,
                hor_front_right = 1,
                hor_back_left = 2,
                hor_back_right = 3,
                vert_front_left = 4,
                vert_front_right = 5,
                vert_back_left = 6,
                vert_back_right = 7
            };
        };

        basic_regulator(std::uint8_t id);

        virtual ~basic_regulator();

        virtual void apply(rov_types::rov_hardware_control &thruster,
                           const rov_types::rov_control &rc,
                           const rov_types::rov_telimetry &rt,
                           const regulator_config &config) = 0;

        virtual std::uint8_t get_id() const;
        bool operator==(const basic_regulator & regualtor);
    protected:
        void constrain_thruster(rov_types::rov_hardware_control &thruster);
        std::int8_t constrain75(int p);
        void constrain_horizontal(rov_types::rov_hardware_control &thruster);
        void constrain_vertical(rov_types::rov_hardware_control &thruster);
        std::int8_t pd_strategy(float kd, float pd, float error);
        std::int8_t diff_strategy(float kd, float error);
        std::int8_t prop_strategy(float pd, float error);
        std::int8_t constrain(int p);
        std::uint8_t m_id;
        float m_prev_error;
    };

    float to360(float angle);
    float to180(float angle);
    float normilize360(float angle);

}

#endif //RC_ROV_SERVER_BASIC_REGULATOR_HPP
