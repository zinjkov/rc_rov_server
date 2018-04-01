//
// Created by zinjkov on 27.03.18.
//

#ifndef RC_ROV_SERVER_EVENTABLE_HPP
#define RC_ROV_SERVER_EVENTABLE_HPP

#include "../name_allias.hpp"
#include "event_bus.hpp"

#include <map>
#include <vector>

namespace rov {
    class eventable {
    public:
        eventable(const std::shared_ptr<event_bus> &event_bus);
        virtual ~eventable();
    protected:
        void subscribe(event_id_t event_id, event_handler handler);
        void call_registered_handlers(const event_ptr &ev);
        void post(const event_ptr &ev);
        void on_event(const event_ptr &ev);

        std::shared_ptr<event_bus> m_event_bus;
        std::map<event_id_t, std::function<void(const event_ptr &)>> m_event_handlers;
    };
}


#endif //RC_ROV_SERVER_EVENTABLE_HPP
