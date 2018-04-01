//
// Created by zinjkov on 27.03.18.
//

#include "eventable.hpp"

rov::eventable::eventable(const std::shared_ptr<event_bus> &event_bus) : m_event_bus(event_bus) {

}

rov::eventable::~eventable() {

}

void rov::eventable::subscribe(event_id_t event_id, event_handler handler) {
    m_event_bus->register_event_type(event_id, std::bind(&eventable::on_event, this, std::placeholders::_1));
    m_event_handlers[event_id] = handler;
}

void rov::eventable::call_registered_handlers(const event_ptr &ev) {
    auto it = m_event_handlers.find(ev->get_type());
    if (it != m_event_handlers.end()) {
        it->second(ev);
    }
}

void rov::eventable::post(const rov::event_ptr &ev) {
    m_event_bus->post_event(ev);
}

void rov::eventable::on_event(const rov::event_ptr &ev) {
    call_registered_handlers(ev);
}
