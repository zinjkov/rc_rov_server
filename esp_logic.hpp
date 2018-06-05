//
// Created by zinjkov on 30.05.18.
//

#ifndef RC_ROV_SERVER_ESP_LOGIC_HPP
#define RC_ROV_SERVER_ESP_LOGIC_HPP

#include "core/eventable.hpp"
#include "core/service_logic.hpp"
#include "regulator/timer.hpp"
namespace rov {
class esp_logic : public service_logic, eventable{
public:
    esp_logic(const std::shared_ptr<rov::service_io> &driver_,
              const std::shared_ptr<rov::event_bus> &event_bus_);

    ~esp_logic();

    virtual void on_read(const message_io &msg) override final;
private:
    void subscribe_to_events();


    void send_request(std::string request);
    void next(bool flag);
    void should_step();
    bool replay_check(const std::string &str, int idx);
    void request(const std::string &cmd);

    std::string m_last_response;
    std::string m_obs_data;
    timer m_timer;
    unsigned long m_timeout;
    bool m_response_recvieved;
    bool m_obs_data_updated = false;
    uint8_t m_state = 0;
    uint8_t m_communication = 0;



    const std::string m_commands[5] = {
            "AT+CWLAP=\"MATE2018\"",
            "AT+CWJAP=\"MATE2018\",\"\"",
            "AT+CIPSTART=\"TCP\",\"192.168.4.1\",80",
            "AT+CIPSEND=16",
            "GET / HTTP/1.1\r\n"
    };

    const uint8_t m_state_mach[5][2] = {
            { 1, 0 },
            { 2, 0 },
            { 3, 0 },
            { 4, 0 },
            { 2, 0 }
    };

    const std::string state_rep[5][3] = {
            { "OK", "+CWLAP:", "MATE2018" },
            { "WIFI CONNECTED", "WIFI GOT IP", "OK" },
            { "CONNECT", "OK", "" },
            { "OK", ">", "" },
            { "CLOSED", "Recv", "SEND OK" },
    };

};
}


#endif //RC_ROV_SERVER_ESP_LOGIC_HPP
