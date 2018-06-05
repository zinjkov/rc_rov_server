//
// Created by zinjkov on 19.05.18.
//

#ifndef RC_ROV_SERVER_ROLL_REGULATOR_HPP
#define RC_ROV_SERVER_ROLL_REGULATOR_HPP

#include "basic_regulator.hpp"

namespace rov {
class roll_regulator : public basic_regulator {
public:
    roll_regulator();
    virtual ~roll_regulator();
    void apply(std::vector<int8_t> &force,
               const rov_types::rov_control &rc,
               const rov_types::rov_telimetry &rt,
               const regulator_config &config) override final;


};
}



#endif //RC_ROV_SERVER_ROLL_REGULATOR_HPP
