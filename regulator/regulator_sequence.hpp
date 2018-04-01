//
// Created by zinjkov on 01.04.18.
//

#ifndef RC_ROV_SERVER_REGULATOR_SEQUENCE_HPP
#define RC_ROV_SERVER_REGULATOR_SEQUENCE_HPP

#include "basic_regulator.hpp"
#include <memory>
namespace rov {
    class regulator_sequence {
    public:
        regulator_sequence();
        ~regulator_sequence();

        template <class regulator_type>
        void add_verctical_regulator() {
            m_vetical_sequence.push_back(make_regulator<regulator_type>());
        }

        template <class regulator_type>
        void add_horizontal_regulator() {
            m_horizontal_sequence.push_back(make_regulator<regulator_type>());
        }

        void apply_horizontal(rov_types::rov_hardware_control &thruster,
                              const rov_types::rov_control &rc,
                              const rov_types::rov_telimetry &rt,
                              const basic_regulator::regulator_config &config);

        void apply_vertical(rov_types::rov_hardware_control &thruster,
                              const rov_types::rov_control &rc,
                              const rov_types::rov_telimetry &rt,
                              const basic_regulator::regulator_config &config);


    private:
        std::vector<std::shared_ptr<basic_regulator>> m_horizontal_sequence;
        std::vector<std::shared_ptr<basic_regulator>> m_vetical_sequence;

        template <class regulator_type>
        std::shared_ptr<basic_regulator> make_regulator(){
            return std::make_shared<regulator_type>();
        }

    };
}

#endif //RC_ROV_SERVER_REGULATOR_SEQUENCE_HPP
