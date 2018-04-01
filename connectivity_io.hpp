//
// Created by zinjkov on 29.03.18.
//

#ifndef RC_ROV_SERVER_CONNECTIVITY_IO_HPP
#define RC_ROV_SERVER_CONNECTIVITY_IO_HPP

#include "core/service_io.hpp"
#include "connectivity/session.hpp"

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind.hpp>
#include <unordered_set>
#include <memory>


namespace rov {
    class connectivity_io : public service_io {
    public:

        connectivity_io(const io_service_ptr &service);

        virtual ~connectivity_io();


        virtual void write(const message_io &write_data) override final;

        virtual void start() override final;

        virtual void stop() override final;

        void on_read(const std::vector<std::uint8_t> &data_recieved);
        void on_disconnect(session *ptr);

        virtual bool is_connected() override final;

    private:
        std::shared_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
        std::map<std::size_t, std::shared_ptr<session>> m_sessions;

        void handle_accept(std::shared_ptr<session> new_session, const boost::system::error_code& error);
    };
}


#endif //RC_ROV_SERVER_CONNECTIVITY_IO_HPP
