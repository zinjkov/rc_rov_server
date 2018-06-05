//
// Created by zinjkov on 29.05.18.
//

#ifndef RC_ROV_SERVER_WEBSOCKET_LOGIC_HPP
#define RC_ROV_SERVER_WEBSOCKET_LOGIC_HPP

#include "core/eventable.hpp"
#include "core/service_logic.hpp"
namespace rov {
    class websocket_logic : public service_logic, eventable  {
    public:
        websocket_logic(const std::shared_ptr<service_io> &driver_, const std::shared_ptr<event_bus> &event_bus_);
        ~websocket_logic();

        virtual void on_read(const message_io &msg) override final;

    private:
        void subscribe_to_events();
        void on_image_updated(const event_ptr &ev);
    };
}


#endif //RC_ROV_SERVER_WEBSOCKET_LOGIC_HPP
