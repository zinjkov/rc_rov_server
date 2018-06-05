//
// Created by zinjkov on 19.05.18.
//

#ifndef RC_ROV_SERVER_PITCH_REGULATOR_HPP
#define RC_ROV_SERVER_PITCH_REGULATOR_HPP

#include "basic_regulator.hpp"

namespace rov {
class pitch_regulator : public basic_regulator {
public:
    pitch_regulator();
    virtual ~pitch_regulator();
    void apply(std::vector<int8_t> &force,
               const rov_types::rov_control &rc,
               const rov_types::rov_telimetry &rt,
               const regulator_config &config) override final;


};
}


#endif //RC_ROV_SERVER_PITCH_REGULATOR_HPP
