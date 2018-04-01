//
// Created by zinjkov on 18.03.18.
//

#ifndef RC_ROV_SERVER_NAME_ALLIAS_HPP
#define RC_ROV_SERVER_NAME_ALLIAS_HPP

#include <boost/asio/io_service.hpp>
#include <boost/function.hpp>
#include "core/event_t.hpp"

namespace rov {
    class message_io;

    using io_service_ptr = std::shared_ptr<boost::asio::io_service>;
//    using event_ptr = std::shared_ptr<event_t>;
    using event_handler = std::function<void(const std::shared_ptr<event_t> &)>;
//    using event_id_t = std::size_t;

    using on_read_callback = std::function<void(const message_io &)>;
    using on_open_callback = std::function<void()>;
    using on_close_callback = std::function<void()>;

}

#endif //RC_ROV_SERVER_NAME_ALLIAS_HPP
