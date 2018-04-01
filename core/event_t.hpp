//
// Created by zinjkov on 18.03.18.
//

#ifndef RC_ROV_SERVER_EVENT_T_HPP
#define RC_ROV_SERVER_EVENT_T_HPP

#include <cstdint>
#include <boost/any.hpp>
#include <memory>
#include <iostream>
#include "event_types.hpp"
namespace rov {
    class event_t;
    using event_ptr = std::shared_ptr<event_t>;

    class event_t {
    public:
        event_t();



        std::size_t get_type() {
            return m_type;
        }

        template<class T>
        const T &get() const{
            return boost::any_cast<const T &>(m_payload);
        }

        template<class T>
        void set(std::size_t type, const T &payload) {
            m_type = type;
            m_payload = payload;
        }

        template<typename payload_type>
        static event_ptr make_event_ptr(event_type::event_id id, payload_type payload) {
            event_ptr e = std::make_shared<event_t>();
            e->set(id, payload);
            return e;
        };
    private:
        std::size_t m_type;
        boost::any m_payload;
    };

};


#endif //RC_ROV_SERVER_EVENT_T_HPP
