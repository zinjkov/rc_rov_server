//
// Created by zinjkov on 18.03.18.
//

#include <iostream>
#include "imu_service.hpp"
#include "core/event_types.hpp"
#include "imu_io.hpp"
#include "message_io_types.hpp"

extern "C" {
    #include "thirdparty/packet.h"
    #include "thirdparty/imu_data_decode.h"
}
rov::imu_service::imu_service(const std::shared_ptr<rov::service_io> &driver_,
                              const std::shared_ptr<rov::event_bus> &event_bus_) :
    service_logic(driver_), eventable(event_bus_)
{
    subscribe_to_events();
    m_service->register_on_read_handler(std::bind(&imu_service::on_read, this, std::placeholders::_1));
    m_service->start();
    imu_data_decode_init();
}

rov::imu_service::~imu_service() {

}

void rov::imu_service::on_read(const rov::message_io &msg) {
    packet_decoding(msg.get<message_io_types::imu>().get());
    post(event_t::make_event_ptr(event_type::imu_data_recieved, get_angles()));
}

void rov::imu_service::subscribe_to_events() {
    subscribe(event_type::imu_config,
              std::bind(&imu_service::on_imu_config_event,
                        this, std::placeholders::_1));
}

void rov::imu_service::on_imu_config_event(const rov::event_ptr &ev) {
    std::cout << "on imu config event called" << std::endl;
}

void rov::imu_service::packet_decoding(const std::vector<std::uint8_t> &bytes) {
    for(auto &byte : bytes){
        Packet_Decode(byte);
    }
}

rov::imu_service::eular_angles rov::imu_service::get_angles() {
    eular_angles angles = {0.0f, 0.0f, 0.0f};
    float eular[3] = {0};
    get_eular(eular);
    angles.pitch = eular[0];
    angles.roll = eular[1];
    angles.yaw = eular[2];
    return angles;
}
