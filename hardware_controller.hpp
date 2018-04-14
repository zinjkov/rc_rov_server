//
// Created by zinjkov on 01.04.18.
//

#ifndef RC_ROV_SERVER_HARDWARE_CONTROLLER_HPP
#define RC_ROV_SERVER_HARDWARE_CONTROLLER_HPP

#include "core/eventable.hpp"
#include "core/service_logic.hpp"
#include "rov_types/rov_data_types.hpp"
#include "regulator/regulators.hpp"
namespace rov {

    class hardware_controller : public service_logic, public eventable {
    public:
        hardware_controller(const std::shared_ptr<service_io> &driver_, const std::shared_ptr<event_bus> &event_bus_);
        virtual ~hardware_controller();
        void on_read(const message_io &msg) override final;
    private:
        void subscribe_to_event();
        void on_telimetry_updated(const event_ptr &event);
        void on_control_updated(const event_ptr &event);

        void emit_control();
        void update_config();
        void init_packet_handler();

        rov_types::serializable::error_code on_hardware_telimetry(const std::vector<std::uint8_t> &packet);

        rov_types::rov_control m_control;
        rov_types::rov_telimetry m_telimetry;
        regulator_sequence m_regulators;
        basic_regulator::regulator_config m_config;

        std::map<uint8_t, std::function<rov_types::serializable::error_code (const std::vector<std::uint8_t>)>> m_packet_handler;
    };
}


#endif //RC_ROV_SERVER_HARDWARE_CONTROLLER_HPP
