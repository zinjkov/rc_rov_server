//
// Created by zinjkov on 02.04.18.
//

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <thread>
#include "hardware_io.hpp"
#include "message_io_types.hpp"

inline void sleepFor(long long int time) {
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}


rov::hardware_io::hardware_io(const rov::io_service_ptr &service) :
        service_io(service),
        m_driver(std::make_shared<serial::Serial>("/dev/ttyUSB0", 115200,
                                                  serial::Timeout(serial::Timeout::simpleTimeout(10)))),
        m_transmit_timer(*service),
        m_recvieve_timer(*service),
        m_write_updated(false)
{
    sleepFor(1000);
    start();
    restart_wr();
}

rov::hardware_io::~hardware_io() {

}

void rov::hardware_io::write(const rov::message_io &write_data) {
    if (!m_driver->isOpen()) {
        return;
    }
    m_write_buffer = write_data.get<message_io_types::hardware>().get();
    m_write_updated = true;
}

void rov::hardware_io::start() {
    if (!m_driver->isOpen()) {
        m_driver->open();
        sleepFor(1000);

    }
}

void rov::hardware_io::stop() {
    m_driver->close();
    m_transmit_timer.cancel();
    m_recvieve_timer.cancel();
}

bool rov::hardware_io::is_connected() {
    return m_driver->isOpen();
}

void rov::hardware_io::read_driver(const boost::system::error_code &e) {

    if (e){
        //TODO SEE WHAT WRONG
        return;
    }
    if (!m_driver->isOpen()) {
        return;
    }

    std::vector<std::uint8_t> vec_to_ret;
    m_driver->read(vec_to_ret, 1024);
    m_driver->flushInput();
    if (!vec_to_ret.empty()){
        try {
            m_on_read_callback(message_io_types::create_msg_io<message_io_types::hardware>(vec_to_ret));
        }
        catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
    restart_read();

}

void rov::hardware_io::write_driver(const boost::system::error_code &e) {
    if (e){
        //TODO SEE WHAT WRONG
        return;
    }
    if (!m_driver->isOpen()) {
        return;
    }
    if (m_write_updated) {
        m_driver->write(m_write_buffer);
        m_driver->flushOutput();
        m_write_updated = false;
    }
    restart_write();
}

void rov::hardware_io::restart_read() {
    m_recvieve_timer.expires_from_now(boost::posix_time::milliseconds(20));
    m_recvieve_timer.async_wait(boost::bind(&hardware_io::read_driver, this, boost::asio::placeholders::error));
}

void rov::hardware_io::restart_write() {
    m_transmit_timer.expires_from_now(boost::posix_time::milliseconds(10));
    m_transmit_timer.async_wait(boost::bind(&hardware_io::write_driver, this, boost::asio::placeholders::error));
}

void rov::hardware_io::restart_wr() {
    restart_read();
    restart_write();
}
