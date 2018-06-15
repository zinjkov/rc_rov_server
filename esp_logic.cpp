//
// Created by zinjkov on 30.05.18.
//

#include "esp_logic.hpp"

#include "message_io_types.hpp"

rov::esp_logic::esp_logic(const std::shared_ptr<rov::service_io> &driver_,
                          const std::shared_ptr<rov::event_bus> &event_bus_) :
        service_logic(driver_), eventable(event_bus_){

    driver_->register_on_read_handler(std::bind(&esp_logic::on_read, this, std::placeholders::_1));
    subscribe_to_events();
}

rov::esp_logic::~esp_logic() {

}

void rov::esp_logic::on_read(const rov::message_io &msg) {
    auto data  = msg.get<message_io_types::hardware>().get();
    std::string reply(data.size(), 0);
    std::copy(data.begin(), data.end(), reply.begin());
    post(event_t::make_event_ptr(event_type::obs_data, reply));
}

void rov::esp_logic::subscribe_to_events() {
    subscribe(event_type::obs_command,
              std::bind(&esp_logic::on_transmit_request,
                        this, std::placeholders::_1));
}

void rov::esp_logic::on_transmit_request(const rov::event_ptr &ev)  {
    auto command = ev->get<std::string>();
    std::cout << command << std::endl;
    command += "\r\n";
    std::vector<uint8_t> data(command.size());
    std::copy(command.begin(), command.end(), data.begin());
    m_service->write(message_io_types::create_msg_io<message_io_types::hardware>(data));
}