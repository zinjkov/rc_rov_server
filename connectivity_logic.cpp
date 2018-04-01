//
// Created by zinjkov on 29.03.18.
//
#include <iostream>
#include "connectivity_logic.hpp"
#include "core/event_types.hpp"
#include "message_io_types.hpp"
#include "rov_types/rov_data_types.hpp"


namespace rov {
    rov::connectivity_logic::connectivity_logic(const std::shared_ptr<service_io> &driver_,
                                                const std::shared_ptr<event_bus> &event_bus_) :
            service_logic(driver_), eventable(event_bus_), m_size_last_packet(0)
    {
        m_service->register_on_read_handler(std::bind(&connectivity_logic::on_read, this, std::placeholders::_1));

        subscribe(event_type::connectivity_transmit_request,
                  std::bind(&connectivity_logic::on_transmit_request,
                            this, std::placeholders::_1));

        init_message_mapping();
    }

    void rov::connectivity_logic::on_read(const rov::message_io &msg) {
        auto payload = msg.get<message_io_types::connectivity_message>().get();
        packet_handling(payload);

    }

    rov::connectivity_logic::~connectivity_logic() {

    }

    void rov::connectivity_logic::on_transmit_request(const rov::event_ptr &ev) {
        m_service->write(ev->get<message_io>());
    }

    void connectivity_logic::init_message_mapping() {
        using namespace rov_types;
        m_packet_handler[rov_control::meta::packet_id] =
                std::bind(&connectivity_logic::on_receive_control, this, std::placeholders::_1);
    }

    rov_types::serializable::error_code connectivity_logic::on_receive_control(const std::vector<std::uint8_t> &packet) {
        rov_types::rov_control rc;
        rov_types::serializable::error_code err = rc.deserialize(packet);
        if (rov_types::serializable::check_for_success(err)) {
            event_ptr e;
            e->set(event_type::rov_control_received, rc);
            post(e);
        }
        return err;
    }


    void connectivity_logic::packet_handling(const std::vector<std::uint8_t> &packet) {
        try {
            using namespace rov_types;
            std::cout << (int)packet[0] << " " << packet.size() << std::endl;
            serializable::error_code err = m_packet_handler[packet[0]](packet);
            if (err == serializable::error_code::success_size_greater){
                auto next = packet;
                next.erase(next.begin(), next.begin() + m_size_last_packet);
                packet_handling(next);
            }

        } catch (std::bad_function_call &e){
            std::cerr << "Exception in connectivity_logic::packet_handling with " ;
            std::cerr << "0x" << std::hex << (int)packet[0];
            std::cerr << " " << e.what() << std::endl;
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

};