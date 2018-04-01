//
// Created by zinjkov on 18.03.18.
//


#include <boost/asio.hpp>
#include <iostream>
#include "imu_io.hpp"
#include "boost/bind.hpp"
#include "message_io_types.hpp"

rov::imu_io::imu_io(const rov::io_service_ptr &service) :
    service_io(service),
    m_driver(std::make_shared<posix_serial>("port")),
    m_transmit_timer(*service)
{

}

bool rov::imu_io::is_connected() {
    return false;
}


void rov::imu_io::read_driver(const boost::system::error_code &e) {
    if (e){
        //TODO SEE WHAT WRONG
    }
    std::vector<uint8_t> buffer;
    m_driver->read_bytes(buffer);
    if (buffer.size() != 0){
        try {
            m_on_read_callback(message_io_types::create_msg_io<message_io_types::imu_message>(buffer));
        }
        catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
    start();
}

void rov::imu_io::write(const rov::message_io &write_data) {

}

void rov::imu_io::start() {
    //TODO: add config time timer;
    m_transmit_timer.expires_from_now(boost::posix_time::milliseconds(20));
    m_transmit_timer.async_wait(boost::bind(&imu_io::read_driver, this, boost::asio::placeholders::error));
}

void rov::imu_io::stop() {
    m_transmit_timer.cancel();
}

