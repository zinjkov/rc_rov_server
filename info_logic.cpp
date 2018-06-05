//
// Created by zinjkov on 03.06.18.
//

#include "info_logic.hpp"
#include <nlohmann_json/json.hpp>

rov::info_logic::info_logic(const std::shared_ptr<rov::service_io> &driver_,
                            const std::shared_ptr<rov::event_bus> &event_bus_)  :
        service_logic(driver_), eventable(event_bus_) {
    subscribe_to_events();

}

rov::info_logic::~info_logic() {

}

void rov::info_logic::on_read(const rov::message_io &msg) {

}

void rov::info_logic::subscribe_to_events() {

}

void rov::info_logic::on_OBS_data_updated(const rov::event_ptr &event) {
    nlohmann::json json;
    json["state"] = "searching";
    json["angleX"] = 3.5;
    json["angleY"] = -3.5;
    json["data"] = {3,5,6};
}

