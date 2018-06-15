//
// Created by zinjkov on 03.06.18.
//

#ifndef RC_ROV_SERVER_INFO_LOGIC_HPP
#define RC_ROV_SERVER_INFO_LOGIC_HPP
#include "core/eventable.hpp"
#include "core/service_logic.hpp"
namespace rov {
class info_logic : public service_logic, eventable {
public:
    info_logic(const std::shared_ptr<rov::service_io> &driver_,
                const std::shared_ptr<rov::event_bus> &event_bus_);

    virtual ~info_logic();

    virtual void on_read(const message_io &msg) override ;
private:
    void subscribe_to_events();

    void on_OBS_data_updated(const event_ptr &event);
    void on_transmit_request(const event_ptr &event);
};
}


#endif //RC_ROV_SERVER_INFO_LOGIC_HPP
