//
// Created by zinjkov on 18.03.18.
//

#include "event_t.hpp"
#include <string>

namespace rov {
    event_t::event_t() : m_type(0), m_payload(std::string("type: 0; empty payload")) {

    }
}


