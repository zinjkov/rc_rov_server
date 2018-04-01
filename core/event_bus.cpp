//
// Created by zinjkov on 18.03.18.
//

#include "event_bus.hpp"
#include <boost/bind.hpp>
rov::event_bus::event_bus(const rov::io_service_ptr &service) : m_service(service) {

}

rov::event_bus::~event_bus() {

}

void rov::event_bus::post_event(const rov::event_ptr &ev) {

    if (m_event_map.count(ev->get_type())) {
        const auto &handlers = m_event_map[ev->get_type()];
        for (const auto &handler : handlers) {
            m_service->post(boost::bind(handler, ev));
        }
    }
}

void rov::event_bus::register_event_type(event_id_t evt_type, event_handler hdlr) {
    m_event_map[evt_type].emplace_back(hdlr);
}

void rov::event_bus::unregister_event_type(rov::event_id_t evt_type) {
    auto it = m_event_map.find(evt_type);
    m_event_map.erase(it);
}
