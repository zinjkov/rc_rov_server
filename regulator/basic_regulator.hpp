//
// Created by zinjkov on 01.04.18.
//

#ifndef RC_ROV_SERVER_BASIC_REGULATOR_HPP
#define RC_ROV_SERVER_BASIC_REGULATOR_HPP

#include <vector>
#include <cstdint>
#include "../rov_types/rov_data_types.hpp"
#include "timer.hpp"
namespace rov {
    class basic_regulator {
    public:
        struct regulator_config {
            rov_types::rov_pd pd;
            rov_types::rov_enable_pd enabled_pd;
        };

        basic_regulator(std::uint8_t id);

        virtual ~basic_regulator();

        virtual void apply(std::vector<int8_t> &force,
                           const rov_types::rov_control &rc,
                           const rov_types::rov_telimetry &rt,
                           const regulator_config &config) = 0;

        virtual std::uint8_t get_id() const;

        bool operator==(const basic_regulator & regualtor);

    protected:
        std::int8_t pid_strategy(float p, float i, float d, float error);
        std::int8_t diff_strategy(float kd, float error);
        std::int8_t prop_strategy(float pd, float error);
        std::int8_t int_strategy(float pd, float error);

        std::int8_t constrain(int p);
        int constrainto(int p, int v);

        timer m_timer;
        std::uint8_t m_id;
        float m_prev_error;
        float m_prev_integral;
    };

    float to360(float angle);
    float to180(float angle);
    float normilize360(float angle);

}

#endif //RC_ROV_SERVER_BASIC_REGULATOR_HPP
