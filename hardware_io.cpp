//
// Created by zinjkov on 02.04.18.
//

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "hardware_io.hpp"
#include "message_io_types.hpp"

rov::hardware_io::hardware_io(const rov::io_service_ptr &service) :
        service_io(service),
        m_driver(std::make_shared<serial::Serial>("/dev/ttyACM0", 115200)),
        m_transmit_timer(*service)
{

    start();
}

rov::hardware_io::~hardware_io() {

}

void rov::hardware_io::write(const rov::message_io &write_data) {
    m_driver->write(write_data.get<message_io_types::hardware>().get());
}

void rov::hardware_io::start() {
    m_transmit_timer.expires_from_now(boost::posix_time::milliseconds(20));
    m_transmit_timer.async_wait(boost::bind(&hardware_io::read_driver, this, boost::asio::placeholders::error));
}

void rov::hardware_io::stop() {
    m_transmit_timer.cancel();
}

bool rov::hardware_io::is_connected() {
    return false;
}

void rov::hardware_io::read_driver(const boost::system::error_code &e) {

    if (e){
        //TODO SEE WHAT WRONG
        return;
    }

    std::vector<std::uint8_t> vec_to_ret;
    auto recv_str = m_driver->read(1024);
//    std::cout <<"hardware_io::read_driver" << std::endl;
    if (recv_str.size() != 0){
        try {
            vec_to_ret.resize(recv_str.size());
            std::cout << recv_str << std::endl;
            std::copy(std::begin(recv_str), std::end(recv_str), std::begin(vec_to_ret));
            m_on_read_callback(message_io_types::create_msg_io<message_io_types::hardware>(vec_to_ret));
        }
        catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
    start();

}
