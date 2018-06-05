//
// Created by zinjkov on 29.05.18.
//

#ifndef RC_ROV_SERVER_WEBSOCKET_IO_HPP
#define RC_ROV_SERVER_WEBSOCKET_IO_HPP

#include "core/service_io.hpp"

#include "thirdparty/websocketpp/config/asio_no_tls.hpp"
#include "thirdparty/websocketpp/server.hpp"

#include <set>

namespace rov {
    class websocket_io : public service_io {
    public:
        typedef websocketpp::server<websocketpp::config::asio> server;
        typedef server::message_ptr message_ptr;
        typedef websocketpp::lib::error_code er_code;
        typedef websocketpp::connection_hdl connection_hdl;

        websocket_io(uint16_t port, const io_service_ptr &service);

        ~websocket_io();

        virtual void register_on_read_handler(on_read_callback read_function);

        virtual void write(const message_io &write_data);

        virtual void start();

        virtual void stop();

        virtual bool is_connected();

    private:
        typedef std::set<connection_hdl, std::owner_less<connection_hdl>> connections_list;

        uint16_t m_port;
        server m_endpoint;
        connections_list m_connections;

        void on_open(connection_hdl hdl);

        void on_fail(connection_hdl hdl);

        void on_close(connection_hdl hdl);

        void on_read(connection_hdl hdl, message_ptr msg);
    };
}


#endif //RC_ROV_SERVER_WEBSOCKET_IO_HPP
