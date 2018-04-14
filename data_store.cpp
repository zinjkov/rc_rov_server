//
// Created by zinjkov on 01.04.18.
//

#include "data_store.hpp"
#include "imu_service.hpp"

rov::data_store::data_store(const std::shared_ptr<rov::event_bus> &event_bus_) : eventable(event_bus_) {

    subscribe_to_event();

}



rov::data_store::~data_store() {

}


void rov::data_store::on_control_updated(const rov::event_ptr &event) {
    m_control = event->get<rov_types::rov_control>();
}

void rov::data_store::on_imu_updated(const rov::event_ptr &event) {
    auto imu_data = event->get<imu_service::eular_angles>();
    m_telimetry.pitch = imu_data.pitch;
    m_telimetry.roll = imu_data.roll;
    m_telimetry.yaw = imu_data.yaw;
    post(event_t::make_event_ptr(event_type::telimetry_updated, m_telimetry));
}

void rov::data_store::subscribe_to_event() {
    subscribe(event_type::rov_control_received, std::bind(&data_store::on_control_updated,
                                                          this, std::placeholders::_1));

    subscribe(event_type::imu_data_recieved, std::bind(&data_store::on_imu_updated,
                                                       this, std::placeholders::_1));
    subscribe(event_type::hardware_telemetry_updated, std::bind(&data_store::on_hardware_telimetry_updated,
                                                       this, std::placeholders::_1));
}

void rov::data_store::on_hardware_telimetry_updated(const rov::event_ptr &event) {
    auto hardware = event->get<rov_types::rov_hardware_telimetry>();

    m_telimetry.depth = hardware.depth;

    m_telimetry.manipulator_rotated = hardware.manipulator_rotated;
    m_telimetry.manipulator_opened_closed = hardware.manipulator_opened_closed;

    m_telimetry.esp_state = hardware.esp_state;
    m_telimetry.esp_communication = hardware.esp_communication;

    m_telimetry.mega_communication = hardware.mega_communication;
    m_telimetry.mini_communication = hardware.mini_communication;

    m_telimetry.camera_rotate[0] = hardware.camera_rotate[0];
    m_telimetry.camera_rotate[1] = hardware.camera_rotate[1];

    m_telimetry.acoustic = hardware.acoustic;
    m_telimetry.magnet = hardware.magnet;

//    for (int i = 0; i < 4; i++) {
//        m_telimetry.twisting_motors[i] = hardware.twisting_motors[i];
//    }

    post(event_t::make_event_ptr(event_type::telimetry_updated, m_telimetry));
}

