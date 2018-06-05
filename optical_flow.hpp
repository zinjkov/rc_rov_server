//
// Created by zinjkov on 19.05.18.
//

#ifndef RC_ROV_SERVER_OPTICAL_FLOW_HPP
#define RC_ROV_SERVER_OPTICAL_FLOW_HPP

#include "core/eventable.hpp"
namespace rov {
    class optical_flow : public eventable {
    public:
        optical_flow(const std::shared_ptr<event_bus> &event_bus_);
    private:
        void subscribe_to_event();
        void on_frame_updated(const event_ptr &e);
        std::string m_name;
    };
}

#endif //RC_ROV_SERVER_OPTICAL_FLOW_HPP
