//
// Created by zinjkov on 29.03.18.
//
#include <iostream>
#include "connectivity_logic.hpp"
#include "message_io_types.hpp"
#include "rov_types/rov_data_types.hpp"


namespace rov {
    rov::connectivity_logic::connectivity_logic(const std::shared_ptr<service_io> &driver_,
                                                const std::shared_ptr<event_bus> &event_bus_) :
            service_logic(driver_), eventable(event_bus_), m_size_last_packet(0)
    {
        m_service->register_on_read_handler(std::bind(&connectivity_logic::on_read, this, std::placeholders::_1));
        subscibe_to_event();
        init_message_mapping();
    }

    void rov::connectivity_logic::on_read(const rov::message_io &msg) {
        auto payload = msg.get<message_io_types::connectivity>().get();
        packet_handling(payload);

    }

    rov::connectivity_logic::~connectivity_logic() {

    }

    void rov::connectivity_logic::on_transmit_request(const rov::event_ptr &ev) {
        m_service->write(ev->get<message_io>());
    }

    void connectivity_logic::init_message_mapping() {
        using namespace rov_types;
        m_packet_handler[rov_control::meta().packet_id] =
                std::bind(&connectivity_logic::on_receive_control, this, std::placeholders::_1);

        m_packet_handler[rov_debug::meta().packet_id] =
                std::bind(&connectivity_logic::on_receive_debug, this, std::placeholders::_1);

        m_packet_handler[rov_hardware_firmware::meta().packet_id] =
                std::bind(&connectivity_logic::on_receive_firmware, this, std::placeholders::_1);

        m_packet_handler[rov_pd::meta().packet_id] =
                std::bind(&connectivity_logic::on_receive_pd, this, std::placeholders::_1);

        m_packet_handler[rov_enable_pd::meta().packet_id] =
                std::bind(&connectivity_logic::on_receive_enable_pd, this, std::placeholders::_1);
    }


    void connectivity_logic::packet_handling(const std::vector<std::uint8_t> &packet) {
        try {
            using namespace rov_types;
            int i = 0;
            auto decoding = packet;

            while (i != decoding.size()) {
                if (m_packet_handler.find(decoding[i]) != m_packet_handler.end()) {
                    serializable::error_code err = m_packet_handler[decoding[i]](decoding);
                    if (serializable::check_for_success(err)) {
                        decoding.erase(decoding.begin(), decoding.begin() + m_size_last_packet);
                        i = 0;
                        continue;
                    }
                }
                i++;
            }

        } catch (std::bad_function_call &e){
            std::cerr << "Exception in connectivity_logic::packet_handling with " ;
            std::cerr << "0x" << std::hex << (int)packet[0];
            std::cerr << " " << e.what() << std::endl;
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }


    void connectivity_logic::subscibe_to_event() {
        subscribe(event_type::connectivity_transmit_request,
                  std::bind(&connectivity_logic::on_transmit_request,
                            this, std::placeholders::_1));

        subscribe(event_type::telimetry_updated,
                  std::bind(&connectivity_logic::on_telimetry_updated,
                            this, std::placeholders::_1));
    }

    rov_types::serializable::error_code connectivity_logic::on_receive_control(const std::vector<std::uint8_t> &packet) {
        rov_types::rov_control rc;
        rov_types::serializable::error_code err = rc.deserialize(packet);
        if (rov_types::serializable::check_for_success(err)) {
            m_size_last_packet = rov_types::rov_control::meta().packet_size;
            post(event_t::make_event_ptr(event_type::rov_control_received, rc));

        }

        return err;
    }

    void connectivity_logic::on_telimetry_updated(const event_ptr &ev) {
        auto telimetry = ev->get<rov_types::rov_telimetry>();
        m_service->write(message_io_types::create_msg_io<message_io_types::connectivity>(
                telimetry.serialize()));
    }

    rov_types::serializable::error_code connectivity_logic::on_receive_debug(const std::vector<std::uint8_t> &packet) {
        rov_types::rov_debug rd;
        auto err = rd.deserialize(packet);
        if (rov_types::serializable::check_for_success(err)) {
            m_size_last_packet = rov_types::rov_debug::meta().packet_size;
            post(event_t::make_event_ptr(event_type::rov_debug_received, rd));

        }
        return err;
    }

    rov_types::serializable::error_code
    connectivity_logic::on_receive_firmware(const std::vector<std::uint8_t> &packet) {
        rov_types::rov_hardware_firmware firmware;
        auto err = firmware.deserialize(packet);
        if (rov_types::serializable::check_for_success(err)) {
            m_size_last_packet = rov_types::rov_hardware_firmware::meta().packet_size + (uint16_t)firmware.size;
            post(event_t::make_event_ptr(event_type::rov_firmware_received, firmware));
        }
        return err;
    }

    rov_types::serializable::error_code
    connectivity_logic::on_receive_enable_pd(const std::vector<std::uint8_t> &packet) {
        rov_types::rov_enable_pd enable_pd;
        auto err = enable_pd.deserialize(packet);
        std::cout << rov_types::serializable::error_to_string(err) << std::endl;
        if (rov_types::serializable::check_for_success(err)) {
            m_size_last_packet = rov_types::rov_enable_pd::meta().packet_size;
            post(event_t::make_event_ptr(event_type::rov_enable_pd, enable_pd));
            std::cout << "enable pd recieved" <<std::endl;
        }

        return err;
    }

    rov_types::serializable::error_code connectivity_logic::on_receive_pd(const std::vector<std::uint8_t> &packet) {
        rov_types::rov_pd pd;
        auto err = pd.deserialize(packet);
        if (rov_types::serializable::check_for_success(err)) {
            m_size_last_packet = rov_types::rov_pd::meta().packet_size;
            post(event_t::make_event_ptr(event_type::rov_pd, pd));
            std::cout << "pd recieved" <<std::endl;
        }
        return err;
    }


};