//
// Created by zinjkov on 19.05.18.
//

#include "cameras_logic.hpp"
#include "message_io_types.hpp"
#include <opencv2/opencv.hpp>
#include <fstream>

rov::cameras_logic::cameras_logic(const std::shared_ptr<rov::service_io> &driver_,
                                  const std::shared_ptr<rov::event_bus> &event_bus_)
        : service_logic(driver_), eventable(event_bus_)
{
    m_service->register_on_read_handler(std::bind(&cameras_logic::on_read, this, std::placeholders::_1));
    subscribe_to_event();
    load_config();
}

rov::cameras_logic::~cameras_logic() {

}

void rov::cameras_logic::on_read(const rov::message_io &msg) {
    auto new_frame = msg.get<message_io_types::camera>().get();
    post(event_t::make_event_ptr(event_type::camera_one_frame_updated, new_frame));
}

void rov::cameras_logic::on_camera_config_recvied(const event_ptr &ev) {
    m_service->write(message_io_types::create_msg_io<message_io_types::camera_config>(
            ev->get<nlohmann::json>()
            ));
    m_last_config = ev->get<nlohmann::json>();
    save_config();
}

void rov::cameras_logic::subscribe_to_event() {
    subscribe(event_type::camera_set_config,
              std::bind(&cameras_logic::on_camera_config_recvied,
                        this, std::placeholders::_1));
    subscribe(event_type::camera_get_config,
              std::bind(&cameras_logic::on_camera_config_request,
                        this, std::placeholders::_1));
}

void rov::cameras_logic::save_config() {
    std::ofstream f("cameras");
    std::string config = m_last_config.dump();
    f << config;
    f.close();
}

void rov::cameras_logic::load_config() {
    try {
        std::ifstream f("cameras");
        std::string config;
        f >> config;
        m_last_config = nlohmann::json::parse(config);
        f.close();
        m_service->write(message_io_types::create_msg_io<message_io_types::camera_config>(m_last_config));
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void rov::cameras_logic::on_camera_config_request(const rov::event_ptr &ev) {
    auto config = m_last_config;
    std::vector<uint8_t> cameras;
    for(int i = 0; i < 10; i++) {
        std::stringstream ss;
        ss << "/dev/video" << i;
        if (access(ss.str().c_str(), F_OK) != -1) {
            cameras.push_back(i);
        }
    }
    config["cameras"] = cameras;
    config["type"] = "cameras_config_reply";
    auto out = config.dump();
    post(event_t::make_event_ptr(event_type::info_transmit_request, out));
}

