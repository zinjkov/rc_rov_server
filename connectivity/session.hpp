//
// Created by zinjkov on 29.03.18.
//

#ifndef RC_ROV_SERVER_SESSION_HPP
#define RC_ROV_SERVER_SESSION_HPP

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <memory>
namespace rov {
    class session {
    public:
        session(std::size_t session_id,const std::shared_ptr<boost::asio::io_service> &io_service_);

        void write(const std::vector<std::uint8_t> &data);

        void read();

        boost::asio::ip::tcp::socket &get_socket();
        std::size_t get_id();

        void handle_read(const boost::system::error_code &error,
                         size_t bytes_transferred);

        void set_on_disconnect_callback(std::function<void(session *)>);
        void set_on_read_callback(std::function<void(const std::vector<std::uint8_t> &)> callback);
    private:
        std::size_t m_id;
        std::shared_ptr<boost::asio::io_service> m_io_serice;
        std::shared_ptr<boost::asio::ip::tcp::socket> m_socket;
        boost::asio::deadline_timer m_update_timer;
        std::vector<std::uint8_t> m_buffer;
        bool is_diconnected;
        std::function<void(session *)> m_on_disconnect;
        std::function<void(const std::vector<std::uint8_t> &)> m_on_read;

        void handle_write(const boost::system::error_code &ec);
    };
}


#endif //RC_ROV_SERVER_SESSION_HPP
