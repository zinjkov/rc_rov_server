//
// Created by zinjkov on 29.05.18.
//

#include "websocket_io.hpp"
#include "message_io_types.hpp"

rov::websocket_io::websocket_io(uint16_t port,const rov::io_service_ptr &service)
        : service_io(service), m_port(port) {
    //1) Socket settings
    m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
    m_endpoint.set_access_channels(websocketpp::log::alevel::access_core);
    m_endpoint.set_access_channels(websocketpp::log::alevel::app);

    m_endpoint.init_asio(m_io_service.get());
    m_endpoint.set_reuse_addr(true); //Hack to avoid "Address already in use" error after program restart.

    //2) Set handlers
    using websocketpp::lib::placeholders::_1;
    using websocketpp::lib::placeholders::_2;
    using websocketpp::lib::bind;

    m_endpoint.set_open_handler(bind(&websocket_io::on_open, this, _1));
    m_endpoint.set_fail_handler(bind(&websocket_io::on_fail, this, _1));
    m_endpoint.set_close_handler(bind(&websocket_io::on_close, this, _1));
    m_endpoint.set_message_handler(bind(&websocket_io::on_read, this, _1, _2));
    start();

}

rov::websocket_io::~websocket_io() {

}

void rov::websocket_io::register_on_read_handler(rov::on_read_callback read_function) {
    m_on_read_callback = read_function;
}

void rov::websocket_io::write(const rov::message_io &write_data) {
    if (is_connected()) {
        connections_list::iterator i;
        std::string data = write_data.get<message_io_types::websocket>().get();
        for (i = m_connections.begin(); i != m_connections.end(); ++i) {
            m_endpoint.send(*i, data, websocketpp::frame::opcode::binary);
        }
    }
}

void rov::websocket_io::start() {
    std::stringstream ss;

    ss << "Websocket is listening on port " << m_port;

    m_endpoint.get_alog().write(websocketpp::log::alevel::app, ss.str());
    m_endpoint.listen(m_port);
    m_endpoint.start_accept();
}

void rov::websocket_io::stop() {
    er_code ec;
    m_endpoint.stop_listening(ec);
    if (ec) {
        m_endpoint.get_alog().write(websocketpp::log::alevel::fail, "Faild to stop listening...");
    }

    connections_list::iterator it;
    for (it = m_connections.begin(); it != m_connections.end(); ++it) {
        m_endpoint.close(*it, websocketpp::close::status::going_away, "Terminating...", ec);
        if (ec) {
            m_endpoint.get_alog().write(websocketpp::log::alevel::fail, "Faild to close connection...");
        }
    }
}

bool rov::websocket_io::is_connected() {
    return !m_connections.empty();
}

void rov::websocket_io::on_open(rov::websocket_io::connection_hdl hdl) {
    m_endpoint.get_alog().write(websocketpp::log::alevel::app, "Connection accepted.");
    m_connections.insert(hdl);
}

void rov::websocket_io::on_fail(rov::websocket_io::connection_hdl hdl) {
    m_endpoint.get_con_from_hdl(hdl);//No use
    m_endpoint.get_alog().write(websocketpp::log::alevel::app, "Can not connect...");
}

void rov::websocket_io::on_close(rov::websocket_io::connection_hdl hdl) {
    m_endpoint.get_alog().write(websocketpp::log::alevel::app, "Close connection...");
    m_connections.erase(hdl);

    if (m_connections.empty()) {
        //m_on_close_handler();
    }
}

void rov::websocket_io::on_read(rov::websocket_io::connection_hdl hdl, rov::websocket_io::message_ptr msg) {
    std::stringstream ss;
    ss << "web_server_io::on_read called with " << hdl.lock().get();
    m_endpoint.get_alog().write(websocketpp::log::alevel::app, ss.str());

    std::string s = msg.get()->get_payload();

    m_on_read_callback(message_io_types::create_msg_io<message_io_types::websocket>(s));
}
