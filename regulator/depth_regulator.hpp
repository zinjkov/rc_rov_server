//
// Created by zinjkov on 01.04.18.
//

#ifndef RC_ROV_SERVER_DEPTH_REGULATOR_HPP
#define RC_ROV_SERVER_DEPTH_REGULATOR_HPP

#include "basic_regulator.hpp"

namespace rov {
class depth_regulator : public basic_regulator {
public:
    depth_regulator();
    virtual ~depth_regulator();
    void apply(rov_types::rov_hardware_control &thruster,
               const rov_types::rov_control &rc,
               const rov_types::rov_telimetry &rt,
               const regulator_config &config) override final;


};
}

#endif //RC_ROV_SERVER_DEPTH_REGULATOR_HPP
