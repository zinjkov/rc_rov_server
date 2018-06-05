//
// Created by zinjkov on 02.04.18.
//

#ifndef RC_ROV_SERVER_HARDWARE_IO_HPP
#define RC_ROV_SERVER_HARDWARE_IO_HPP

#include "core/service_io.hpp"
#include <serial/serial.h>
#include <boost/asio/deadline_timer.hpp>
#include <mutex>

namespace rov {
    class hardware_io : public service_io {
    public:

        hardware_io(const std::string& port, const io_service_ptr &service);

        virtual ~hardware_io();

        virtual void write(const message_io &write_data) override final;

        virtual void start() override final;

        virtual void stop() override final;



        virtual bool is_connected() override final;

    private:
        std::shared_ptr<serial::Serial> m_driver;
        std::mutex m_driver_guard;
        boost::asio::deadline_timer m_recvieve_timer;
        std::vector<std::uint8_t> m_write_buffer;
        bool m_write_updated;

        void read_driver(const boost::system::error_code &e);
        void write_driver();

        void restart_read();

        void restart_wr();

    };
}

#endif //RC_ROV_SERVER_HARDWARE_IO_HPP
