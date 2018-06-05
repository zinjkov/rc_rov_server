//
// Created by zinjkov on 01.04.18.
//

#include <fstream>
#include <thread>
#include "data_store.hpp"
#include "imu_service.hpp"
#include "message_io_types.hpp"

rov::data_store::data_store(const std::shared_ptr<rov::event_bus> &event_bus_) : eventable(event_bus_) {

    subscribe_to_event();

    load_pd();

    post(event_t::make_event_ptr(event_type::rov_pd_updated, m_pd));
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

    subscribe(event_type::rov_pd, std::bind(&data_store::on_pd_updated,
                                                                this, std::placeholders::_1));

    subscribe(event_type::rov_enable_pd, std::bind(&data_store::on_enable_pd_updated,
                                            this, std::placeholders::_1));

    subscribe(event_type::leo_telimetry_updated, std::bind(&data_store::on_leo_telimetry_updated,
                                                   this, std::placeholders::_1));
}

void rov::data_store::on_hardware_telimetry_updated(const rov::event_ptr &event) {
    auto hardware = event->get<rov_types::rov_hardware_telimetry>();
    m_telimetry.depth = hardware.depth;

    m_telimetry.manipulator_rotated = hardware.manipulator_rotated;
    m_telimetry.manipulator_opened_closed = hardware.manipulator_opened_closed;

    m_telimetry.mega_communication = hardware.mega_communication;
    m_telimetry.mini_communication = hardware.mini_communication;

    m_telimetry.camera_rotate[0] = hardware.camera_rotate[0];
    m_telimetry.camera_rotate[1] = hardware.camera_rotate[1];

    m_telimetry.magnet = hardware.magnet;

    post(event_t::make_event_ptr(event_type::telimetry_updated, m_telimetry));
}

void rov::data_store::on_pd_updated(const rov::event_ptr &event) {
    auto new_pd = event->get<rov_types::rov_pd>();

    float default_value = -20.0f;
    float default_value_to = -200.0f;

    auto checker_setter = [](float &old_, float &new_, float default_value_) {
        if (new_ != default_value_) {
            old_ = new_;
        }
    };

    checker_setter(m_pd.yaw_p, new_pd.yaw_p, default_value);
    checker_setter(m_pd.yaw_i, new_pd.yaw_i, default_value);
    checker_setter(m_pd.yaw_d, new_pd.yaw_d, default_value);

    checker_setter(m_pd.depth_p, new_pd.depth_p, default_value);
    checker_setter(m_pd.depth_i, new_pd.depth_i, default_value);
    checker_setter(m_pd.depth_d, new_pd.depth_d, default_value);

    checker_setter(m_pd.roll_p, new_pd.roll_p, default_value);
    checker_setter(m_pd.roll_i, new_pd.roll_i, default_value);
    checker_setter(m_pd.roll_d, new_pd.roll_d, default_value);
    checker_setter(m_pd.roll_to_set, new_pd.roll_to_set, default_value_to);

    checker_setter(m_pd.pitch_p, new_pd.pitch_p, default_value);
    checker_setter(m_pd.pitch_i, new_pd.pitch_i, default_value);
    checker_setter(m_pd.pitch_d, new_pd.pitch_d, default_value);
    checker_setter(m_pd.pitch_to_set, new_pd.pitch_to_set, default_value_to);

    post(event_t::make_event_ptr(event_type::rov_pd_updated, m_pd));

    auto data = m_pd.serialize();

    message_io msg = message_io_types::create_msg_io<message_io_types::connectivity>(data);
    post(event_t::make_event_ptr(event_type::connectivity_transmit_request, msg));

    save_pd();
}

void rov::data_store::on_enable_pd_updated(const rov::event_ptr &event) {
    auto new_enable_pd = event->get<rov_types::rov_enable_pd>();

    int8_t default_value = -1;

    auto checker_setter = [](int8_t &old_, int8_t &new_, int8_t default_value_) {
        if (new_ != default_value_) {
            old_ = new_;
        }
    };

    checker_setter(m_enabled_pd.yaw_pd, new_enable_pd.yaw_pd, default_value);
    checker_setter(m_enabled_pd.depth_pd, new_enable_pd.depth_pd, default_value);
    checker_setter(m_enabled_pd.roll_pd, new_enable_pd.roll_pd, default_value);
    checker_setter(m_enabled_pd.pitch_pd, new_enable_pd.pitch_pd, default_value);

    post(event_t::make_event_ptr(event_type::rov_pd_enable_updated, m_enabled_pd));

    auto data = m_enabled_pd.serialize();

    message_io msg = message_io_types::create_msg_io<message_io_types::connectivity>(data);

    post(event_t::make_event_ptr(event_type::connectivity_transmit_request, msg));

    save_enabled_pd();
}

void rov::data_store::load_pd() {
    std::vector<uint8_t> data(rov_types::rov_pd::meta().packet_size);
    std::ifstream f("multiplyer_pd.ini");

    for(auto &byte : data) {
        f >> byte;
    }

    rov_types::rov_pd pd;
    auto err = pd.deserialize(data);
    if (rov_types::serializable::check_for_success(err)) {
        m_pd = pd;
        std::cout << "pd multiplyers loaded" << std::endl;
    } else {
        std::cerr << "pd multiplyers load failed " << rov_types::serializable::error_to_string(err) << std::endl;
    }

}

void rov::data_store::save_pd() {
    auto data = m_pd.serialize();
    std::ofstream f("multiplyer_pd.ini");

    for(auto byte : data) {
        f << byte;
    }
    std::cout << "new multuplyer saved " << std::endl;


}

void rov::data_store::load_enabled_pd() {
    std::vector<uint8_t> data(rov_types::rov_enable_pd::meta().packet_size);
    std::ifstream f("enabled_pd.ini");

    for(auto &byte : data) {
        f >> byte;
    }

    rov_types::rov_enable_pd epd;
    auto err = epd.deserialize(data);
    if (rov_types::serializable::check_for_success(err)) {
        m_enabled_pd = epd;
        std::cout << "pd enabled loaded " << std::endl;
    } else {
       std::cerr << "pd enabled load failed " << rov_types::serializable::error_to_string(err) << std::endl;
    }
}

void rov::data_store::save_enabled_pd() {
    auto data = m_enabled_pd.serialize();
    std::ofstream f("enabled_pd.ini");

    for(auto byte : data) {
        f << byte;
    }
    std::cout << "new enabled pd saved " << std::endl;
}

void rov::data_store::on_leo_telimetry_updated(const rov::event_ptr &event) {
    rov_types::rov_leo_telimetry rlt = event->get<rov_types::rov_leo_telimetry>();
    m_telimetry.esp = rlt.esp;
    m_telimetry.secondary_manipulator = rlt.secondary_manipulator;
    m_telimetry.flashlight = rlt.flashlight;
    m_telimetry.esp_comm = rlt.esp_comm;
    m_telimetry.leo_comm = rlt.leo_comm;
    post(event_t::make_event_ptr(event_type::telimetry_updated, m_telimetry));
}

