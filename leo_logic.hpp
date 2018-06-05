//
// Created by zinjkov on 03.06.18.
//

#ifndef RC_ROV_SERVER_LEO_LOGIC_HPP
#define RC_ROV_SERVER_LEO_LOGIC_HPP

#include "core/eventable.hpp"
#include "core/service_logic.hpp"

namespace rov {
    class leo_logic : public service_logic, eventable {
    public:
        leo_logic(const std::shared_ptr<rov::service_io> &driver_,
                  const std::shared_ptr<rov::event_bus> &event_bus_);

        virtual ~leo_logic();

    virtual void on_read(const message_io &msg) override ;
    private:
        void subscribe_to_events();

        void on_leo_control_updated(const event_ptr &event);

    };
}


#endif //RC_ROV_SERVER_LEO_LOGIC_HPP
