//
// Created by zinjkov on 19.05.18.
//

#include "cameras_logic.hpp"
#include "message_io_types.hpp"
#include <opencv2/opencv.hpp>
rov::cameras_logic::cameras_logic(const std::shared_ptr<rov::service_io> &driver_,
                                  const std::shared_ptr<rov::event_bus> &event_bus_)
        : service_logic(driver_), eventable(event_bus_)
{
    m_service->register_on_read_handler(std::bind(&cameras_logic::on_read, this, std::placeholders::_1));
}

rov::cameras_logic::~cameras_logic() {

}

void rov::cameras_logic::on_read(const rov::message_io &msg) {
    cv::Mat new_frame = msg.get<message_io_types::camera>().get();
    post(event_t::make_event_ptr(event_type::camera_one_frame_updated, new_frame));
}

void rov::cameras_logic::on_camera_config_recvied(const std::string &config) {
    auto str = config;
    m_service->write(message_io_types::create_msg_io<message_io_types::camera_config>(str));
}

