//
// Created by zinjkov on 18.03.18.
//

#include "service_io.hpp"

rov::service_io::service_io(const rov::io_service_ptr &service) : m_io_service(service) {

}

rov::service_io::~service_io() {

}

void rov::service_io::register_on_read_handler(rov::on_read_callback read_function) {
    m_on_read_callback = read_function;
}


