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
    void on_transmit_request(const rov::event_ptr &ev);
};
}


#endif //RC_ROV_SERVER_ESP_LOGIC_HPP
