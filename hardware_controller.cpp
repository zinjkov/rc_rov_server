//
// Created by zinjkov on 01.04.18.
//

#include "hardware_controller.hpp"
#include "message_io_types.hpp"
rov::hardware_controller::hardware_controller(const std::shared_ptr<rov::service_io> &driver_,
                                              const std::shared_ptr<rov::event_bus> &event_bus_) :
        service_logic(driver_), eventable(event_bus_) {

    m_regulators.add_horizontal_regulator<horizontal_regulator>();
    m_regulators.add_horizontal_regulator<yaw_regulator>();

    m_regulators.add_verctical_regulator<vertical_regulator>();
    m_regulators.add_verctical_regulator<depth_regulator>();
    m_regulators.add_verctical_regulator<pitch_roll_regulator>();

    m_service->register_on_read_handler(std::bind(&hardware_controller::on_read, this, std::placeholders::_1));

    subscribe_to_event();
}

rov::hardware_controller::~hardware_controller() {

}

void rov::hardware_controller::subscribe_to_event() {
    subscribe(event_type::telimetry_updated,
              std::bind(&hardware_controller::on_telimetry_updated,
                        this, std::placeholders::_1));

    subscribe(event_type::rov_control_received,
              std::bind(&hardware_controller::on_control_updated,
                        this, std::placeholders::_1));

}

void rov::hardware_controller::on_telimetry_updated(const rov::event_ptr &event) {
    m_telimetry = event->get<rov_types::rov_telimetry>();
    emit_control();
}

void rov::hardware_controller::on_control_updated(const rov::event_ptr &event) {

}

void rov::hardware_controller::emit_control() {
    rov_types::rov_hardware_control rhc;

    update_config();

    m_regulators.apply_horizontal(rhc, m_control, m_telimetry, m_config);
    m_regulators.apply_vertical(rhc, m_control, m_telimetry, m_config);

    std::cout << "horizontal_power = { ";
    for (auto p : rhc.horizontal_power) {
        std::cout << (int)p << ",\t";
    }
    std::cout << "}" << std::endl;

    std::cout << "vertical_power =   { ";
    for (auto p : rhc.vertical_power) {
        std::cout << (int)p << ",\t";
    }
    std::cout << "}" << std::endl;

    m_service->write(message_io_types::create_msg_io<message_io_types::hardware>(rhc.serialize()));
}

void rov::hardware_controller::update_config() {
    static bool first_axis_w = true;
    if (m_control.axis_w == 0) {
        if (first_axis_w) {
            m_config.yaw_to_set = m_telimetry.yaw;
            first_axis_w = false;
        }
    } else {
        first_axis_w = true;
    }
}

void rov::hardware_controller::on_read(const rov::message_io &msg) {
    std::cout << "hardware_controller::on_read" << std::endl;

}

void rov::hardware_controller::init_packet_handler() {

}

rov_types::serializable::error_code rov::hardware_controller::on_hardware_telimetry(const std::vector<std::uint8_t> &packet) {
    std::cout << "hardware_controller::on_hardware_telimetry" << std::endl;
    return rov_types::serializable::success;
}



