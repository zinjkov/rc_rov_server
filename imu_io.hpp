//
// Created by zinjkov on 18.03.18.
//

#ifndef RC_ROV_SERVER_UART_IO_HPP
#define RC_ROV_SERVER_UART_IO_HPP

#include "core/service_io.hpp"
#include "name_allias.hpp"
#include "thirdparty/posix_serial.hpp"
#include <boost/asio/deadline_timer.hpp>

namespace rov {
    class imu_io : public service_io {
    public:
        imu_io(const io_service_ptr &service);

        virtual bool is_connected() override;

        void start() override;

        void stop() override;

    private:

        std::shared_ptr<posix_serial> m_driver;
        boost::asio::deadline_timer m_transmit_timer;

        void write(const message_io &write_data) override;

        void read_driver(const boost::system::error_code &e);

    };
}

#endif //RC_ROV_SERVER_UART_IO_HPP
