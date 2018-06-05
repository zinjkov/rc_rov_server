//
// Created by zinjkov on 03.06.18.
//

#include "leo_logic.hpp"
#include "rov_types/rov_data_types.hpp"
#include "message_io_types.hpp"

rov::leo_logic::leo_logic(const std::shared_ptr<rov::service_io> &driver_,
                          const std::shared_ptr<rov::event_bus> &event_bus_) :
        service_logic(driver_), eventable(event_bus_) {
    subscribe_to_events();
    driver_->register_on_read_handler(std::bind(&leo_logic::on_read, this, std::placeholders::_1));
}

rov::leo_logic::~leo_logic() {

}

void rov::leo_logic::on_read(const rov::message_io &msg) {
    auto binary = msg.get<message_io_types::hardware>().get();
    rov_types::rov_leo_telimetry rlt;
    auto e = rlt.deserialize(binary);

    if (rov_types::serializable::check_for_success(e)) {
        post(event_t::make_event_ptr(event_type::leo_telimetry_updated, rlt));
    } else {
        std::cerr << "leo_logic::on_read - deserialize error: " << rov_types::serializable::error_to_string(e) << std::endl;
    }
}

void rov::leo_logic::subscribe_to_events() {
    subscribe(event_type::rov_control_received,
             std::bind(&leo_logic::on_leo_control_updated,
                       this, std::placeholders::_1));
}

void rov::leo_logic::on_leo_control_updated(const rov::event_ptr &event) {
    rov_types::rov_leo_control rlc;
    rov_types::rov_control rc = event->get<rov_types::rov_control>();

    rlc.realease = rc.realese;
    rlc.flashlight = rc.flashlight;
    rlc.secondary_manipulator = rc.secondary_manipulator;

    auto binary = rlc.serialize();
    m_service->write(message_io_types::create_msg_io<message_io_types::hardware>(binary));
}
