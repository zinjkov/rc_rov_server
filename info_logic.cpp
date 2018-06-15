//
// Created by zinjkov on 03.06.18.
//

#include "info_logic.hpp"
#include "message_io_types.hpp"
#include <nlohmann_json/json.hpp>

rov::info_logic::info_logic(const std::shared_ptr<rov::service_io> &driver_,
                            const std::shared_ptr<rov::event_bus> &event_bus_)  :
        service_logic(driver_), eventable(event_bus_) {
    subscribe_to_events();
    m_service->register_on_read_handler(std::bind(&info_logic::on_read,
                                                  this,
                                                  std::placeholders::_1));
}

rov::info_logic::~info_logic() {

}

void rov::info_logic::on_read(const rov::message_io &msg) {
    try {
        nlohmann::json json = nlohmann::json::parse(msg.get<message_io_types::websocket>().get());
        if (json["type"] == "OBS") {
            std::string paylaod = json["payload"];
            post(event_t::make_event_ptr(event_type::obs_command, paylaod));
        }
        if (json["type"] == "camera_set_config") {
            post(event_t::make_event_ptr(event_type::camera_set_config, json));
        }
        if (json["type"] == "camera_get_config") {
            post(event_t::make_event_ptr(event_type::camera_get_config, json));
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void rov::info_logic::subscribe_to_events() {
    subscribe(event_type::obs_data,
              std::bind(&info_logic::on_OBS_data_updated,
                        this, std::placeholders::_1));
    subscribe(event_type::info_transmit_request,
              std::bind(&info_logic::on_transmit_request,
                        this, std::placeholders::_1));
}

void rov::info_logic::on_OBS_data_updated(const rov::event_ptr &event) {
    auto data = event->get<std::string>();
    nlohmann::json json;
    json["type"] = "OBS";
    json["payload"] = data;
    m_service->write(message_io_types::create_msg_io<message_io_types::websocket>(json.dump()));
}

void rov::info_logic::on_transmit_request(const rov::event_ptr &event) {
    m_service->write(message_io_types::create_msg_io<message_io_types::websocket>(event->get<std::string>()));
}

