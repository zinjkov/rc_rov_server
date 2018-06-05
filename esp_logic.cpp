//
// Created by zinjkov on 30.05.18.
//

#include "esp_logic.hpp"

#include "message_io_types.hpp"

rov::esp_logic::esp_logic(const std::shared_ptr<rov::service_io> &driver_,
                          const std::shared_ptr<rov::event_bus> &event_bus_) :
        service_logic(driver_), eventable(event_bus_),
        m_timeout(0), m_response_recvieved(true), m_state(0){

    driver_->register_on_read_handler(std::bind(&esp_logic::on_read, this, std::placeholders::_1));
    subscribe_to_events();
    next(false);

}

rov::esp_logic::~esp_logic() {


}

void rov::esp_logic::on_read(const rov::message_io &msg) {
    auto data  = msg.get<message_io_types::hardware>().get();
    std::string reply(data.size(), 0);
    std::copy(data.begin(), data.end(), reply.begin());
    m_last_response += reply;
    std::cout << m_last_response << std::endl;
    m_response_recvieved = replay_check(m_last_response, m_state);
    should_step();
}

void rov::esp_logic::subscribe_to_events() {

}

void rov::esp_logic::send_request(std::string request) {
    request += "\r\n";
    std::vector<uint8_t> data(request.size());
    std::copy(request.begin(), request.end(), data.begin());
    m_service->write(message_io_types::create_msg_io<message_io_types::hardware>(data));
    m_last_response = "";
    m_response_recvieved = false;
    m_timer.restart();
    m_timeout = 10000;
}

void rov::esp_logic::next(bool flag) {
    if (flag) {
        m_state = m_state_mach[m_state][0];
    }
    else {
        m_state = m_state_mach[m_state][1];
        m_response_recvieved = true;
    }
    request(m_commands[m_state]);
}

void rov::esp_logic::should_step() {
    if (m_response_recvieved) {
        if (m_state == 4) {
            m_obs_data = m_last_response;
            m_obs_data_updated = true;
        }
        next(true);
    }
    if (m_timer.elapsed() > m_timeout) {
        next(false);
    }
}

bool rov::esp_logic::replay_check(const std::string &str, int idx) {
    for (int i = 0; i < 3; i++) {
        if (str.find(state_rep[idx][i]) == std::string::npos) {
            return false;
        }
    }
    return true;
}

void rov::esp_logic::request(const std::string &cmd) {
    if (m_response_recvieved) {
        send_request(cmd);
    }
}
