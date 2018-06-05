//
// Created by zinjkov on 29.03.18.
//

#include "session.hpp"
#include <algorithm>
#include <iostream>
rov::session::session(std::size_t session_id,const std::shared_ptr<boost::asio::io_service> &io_service_):
    m_id(session_id),
    m_io_serice(io_service_),
    m_socket(std::make_shared<boost::asio::ip::tcp::socket>(*io_service_)),
    m_update_timer(*io_service_),
    m_buffer(1024),
    is_diconnected(false)
{

}

void rov::session::handle_write(const boost::system::error_code &ec) {
    if (ec) {
        if (!is_diconnected) {
            is_diconnected = true;
            m_socket->cancel();
            m_on_disconnect(this);
        }
    }
}


void rov::session::handle_read(const boost::system::error_code &error, size_t bytes_transferred) {
    if (error) {
        if (!is_diconnected) {
            is_diconnected = true;
            m_socket->cancel();
            m_on_disconnect(this);
        }
        return;
    }

    std::vector<std::uint8_t> vec_to_ret = m_buffer;
    vec_to_ret.resize(bytes_transferred);
    m_on_read(std::move(vec_to_ret));
    read();
}

boost::asio::ip::tcp::socket &rov::session::get_socket() {
    return *m_socket;
}

void rov::session::write(const std::vector<std::uint8_t> &data) {
    m_socket->async_send(boost::asio::buffer(data.data(), data.size()),
                        boost::bind(&session::handle_write, this,
                                    boost::asio::placeholders::error));
}

std::size_t rov::session::get_id() {
    return m_id;
}

void rov::session::set_on_disconnect_callback(std::function<void(rov::session *)> callback) {
    m_on_disconnect = callback;
}

void rov::session::set_on_read_callback(std::function<void(const std::vector<std::uint8_t> &)> callback) {
    m_on_read = callback;
}

void rov::session::read() {
    m_socket->async_receive(boost::asio::buffer(m_buffer.data(), m_buffer.size()),
                            boost::bind(&session::handle_read, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}



