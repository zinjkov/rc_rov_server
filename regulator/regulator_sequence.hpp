//
// Created by zinjkov on 01.04.18.
//

#ifndef RC_ROV_SERVER_REGULATOR_SEQUENCE_HPP
#define RC_ROV_SERVER_REGULATOR_SEQUENCE_HPP

#include "basic_regulator.hpp"
#include <memory>
#include <list>
namespace rov {
    class regulator_sequence {
    public:
        regulator_sequence();
        ~regulator_sequence();

        template <class regulator_type>
        void add_verctical_regulator() {
            regulator_type r;
            for (auto &reg : m_vetical_sequence) {
                if (r.get_id() == reg->get_id()) {
                    return;
                }
            }
            m_vetical_sequence.push_back(make_regulator<regulator_type>());
        }

        template <class regulator_type>
        void add_horizontal_regulator() {
            regulator_type r;
            for (auto &reg : m_horizontal_sequence) {
                if (r.get_id() == reg->get_id()) {
                    return;
                }
            }
            m_horizontal_sequence.push_back(make_regulator<regulator_type>());
        }

        template <class regulator_type>
        void remove_verctical_regulator() {
            regulator_type r;
            for (auto &reg : m_vetical_sequence) {
                if (r.get_id() == reg->get_id()) {
                    m_vetical_sequence.remove(reg);
                }
            }

        }

        template <class regulator_type>
        void remove_horizontal_regulator() {
            regulator_type r;
            for (auto &reg : m_horizontal_sequence) {
                if (r.get_id() == reg->get_id()) {
                    m_horizontal_sequence.remove(reg);
                }
            }
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
        std::list<std::shared_ptr<basic_regulator>> m_horizontal_sequence;
        std::list<std::shared_ptr<basic_regulator>> m_vetical_sequence;

        template <class regulator_type>
        std::shared_ptr<basic_regulator> make_regulator(){
            return std::make_shared<regulator_type>();
        }

    };
}

#endif //RC_ROV_SERVER_REGULATOR_SEQUENCE_HPP
