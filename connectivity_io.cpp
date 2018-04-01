//
// Created by zinjkov on 29.03.18.
//

#include <iostream>
#include "connectivity_io.hpp"

#include "message_io_types.hpp"
#include "core/event_t.hpp"
rov::connectivity_io::connectivity_io(const io_service_ptr &service)
        : service_io(service),
          m_acceptor(std::make_shared<boost::asio::ip::tcp::acceptor>(*service,
                  boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 2090)))
{
    start();
}

void rov::connectivity_io::handle_accept(std::shared_ptr<rov::session> new_session,
                                    const boost::system::error_code &error) {
    if (error) {
        new_session.reset();
        return;
    }
    std::cout << "new client accepted" << std::endl;
    new_session->set_on_disconnect_callback(std::bind(&connectivity_io::on_disconnect, this, std::placeholders::_1));
    new_session->set_on_read_callback(std::bind(&connectivity_io::on_read, this, std::placeholders::_1));
    m_sessions.emplace(new_session->get_id(), new_session);
    new_session->read();
    start();
}

rov::connectivity_io::~connectivity_io() {

}

void rov::connectivity_io::write(const rov::message_io &write_data) {
    message_io_types::connectivity_message msg = write_data.get<message_io_types::connectivity_message>();
    for(auto & session : m_sessions) {
        session.second->write(msg.get());
    }
}

bool rov::connectivity_io::is_connected() {
    return m_sessions.size() > 0;
}

void rov::connectivity_io::start() {
    static std::size_t session_id = 0;
    std::cout << "server opened on 2090" << std::endl;
    std::shared_ptr<session> new_session = std::make_shared<session>(session_id++, m_io_service);
    m_acceptor->async_accept(new_session->get_socket(),
                             boost::bind(&connectivity_io::handle_accept, this, new_session,
                                         boost::asio::placeholders::error));
}

void rov::connectivity_io::on_read(const std::vector<std::uint8_t> &data_recieved) {
    m_on_read_callback(message_io_types::create_msg_io<message_io_types::connectivity_message>(data_recieved));
}

void rov::connectivity_io::stop() {

}

void rov::connectivity_io::on_disconnect(rov::session *ptr) {
    m_sessions.erase(ptr->get_id());
}
