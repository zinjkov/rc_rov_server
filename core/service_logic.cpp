//
// Created by zinjkov on 27.03.18.
//

#include "service_logic.hpp"

rov::service_logic::service_logic(const std::shared_ptr<service_io> &service) : m_service(service) {

}

rov::service_logic::~service_logic() {

}

const std::shared_ptr<rov::service_io> rov::service_logic::get_asio() {
    return m_service;
}
