//
// Created by zinjkov on 29.05.18.
//

#include <opencv2/opencv.hpp>
#include "websocket_logic.hpp"
#include "message_io_types.hpp"
#include <vector>

rov::websocket_logic::websocket_logic(const std::shared_ptr<rov::service_io> &driver_,
                                      const std::shared_ptr<rov::event_bus> &event_bus_) :
        service_logic(driver_), eventable(event_bus_){

    driver_->register_on_read_handler(std::bind(&websocket_logic::on_read, this, std::placeholders::_1));
    subscribe_to_events();
    m_quality = 50;
}

rov::websocket_logic::~websocket_logic() {

}

void rov::websocket_logic::on_read(const rov::message_io &msg) {

}

void rov::websocket_logic::subscribe_to_events() {
    subscribe(event_type::camera_one_frame_updated,
              std::bind(&websocket_logic::on_image_updated,
                        this, std::placeholders::_1));
    subscribe(event_type::camera_set_config,
              std::bind(&websocket_logic::on_set_config,
                        this, std::placeholders::_1));
}

#include "regulator/timer.hpp"
void rov::websocket_logic::on_image_updated(const rov::event_ptr &ev) {
    if (m_guard.try_lock()) {
        auto data = ev->get<cv::Mat>();
        std::vector<uchar> encoded;
        std::vector<int> params(2);

        params[0] = cv::IMWRITE_JPEG_QUALITY;
        params[1] = m_quality;

        cv::imencode(".jpg", data, encoded, params);
        std::string out(encoded.size(), 0);
        std::copy(encoded.begin(), encoded.end(), out.begin());
        m_service->write(message_io_types::create_msg_io<message_io_types::websocket>(out));
    }
}

void rov::websocket_logic::on_set_config(const rov::event_ptr &ev) {
    try {
        m_quality = ev->get<nlohmann::json>()["quality"];
    } catch (const std::exception &e) {
        std::cerr << "websocket_logic::on_set_config " << e.what() << std::endl;
    }
}
