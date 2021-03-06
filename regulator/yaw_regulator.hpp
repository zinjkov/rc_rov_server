//
// Created by zinjkov on 01.04.18.
//

#ifndef RC_ROV_SERVER_YAW_REGULATOR_HPP
#define RC_ROV_SERVER_YAW_REGULATOR_HPP

#include "basic_regulator.hpp"
#include "timer.hpp"
namespace rov {
    class yaw_regulator : public basic_regulator {
    public:
        yaw_regulator();

        virtual ~yaw_regulator();

        void apply(std::vector<int8_t> &force,
                   const rov_types::rov_control &rc,
                   const rov_types::rov_telimetry &rt,
                   const basic_regulator::regulator_config &config) override final;

    private:
        timer m_timeout;
        float m_yaw_to_set;
    };
}

#endif //RC_ROV_SERVER_YAW_REGULATOR_HPP
