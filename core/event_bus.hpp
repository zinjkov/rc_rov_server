//
// Created by zinjkov on 18.03.18.
//

#ifndef RC_ROV_SERVER_EVENT_BUS_HPP
#define RC_ROV_SERVER_EVENT_BUS_HPP

#include <boost/asio.hpp>
#include <boost/function.hpp>

#include <memory>
#include <map>
#include <vector>
#include "event_t.hpp"
#include "../name_allias.hpp"
namespace rov {
    class event_bus {
    public:

        event_bus(const io_service_ptr &service);

        ~event_bus();

        void post_event(const event_ptr &evt);

        void register_event_type(event_id_t evt_type, event_handler hdlr);

        void unregister_event_type(event_id_t evt_type);

    private:
        std::map<event_id_t, std::vector<event_handler>> m_event_map;
        io_service_ptr m_service;
    };
}

#endif //RC_ROV_SERVER_EVENT_BUS_HPP
