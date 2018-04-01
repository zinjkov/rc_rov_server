//
// Created by zinjkov on 29.03.18.
//

#ifndef RC_ROV_SERVER_CONNECTIVITY_LOGIC_HPP
#define RC_ROV_SERVER_CONNECTIVITY_LOGIC_HPP

#include "core/service_logic.hpp"
#include "core/eventable.hpp"
#include "rov_types/serializable.hpp"
namespace rov {
    class connectivity_logic : service_logic, eventable {
    public:
        connectivity_logic(const std::shared_ptr<service_io> &driver_, const std::shared_ptr<event_bus> &event_bus_);

        virtual ~connectivity_logic();

        virtual void on_read(const message_io &msg) override final;

    private:
        void on_transmit_request(const event_ptr &ev);

        void init_message_mapping();

        rov_types::serializable::error_code on_receive_control(const std::vector<std::uint8_t> & packet);

        void packet_handling(const std::vector<std::uint8_t> &packet);

        std::map<uint8_t, std::function<rov_types::serializable::error_code(const std::vector<std::uint8_t>)>> m_packet_handler;
        std::uint8_t m_size_last_packet;
    };
}


#endif //RC_ROV_SERVER_CONNECTIVITY_LOGIC_HPP
