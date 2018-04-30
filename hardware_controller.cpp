//
// Created by zinjkov on 01.04.18.
//

#include <fstream>
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

    init_packet_handler();
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

    subscribe(event_type::rov_debug_received,
              std::bind(&hardware_controller::on_debug,
                        this, std::placeholders::_1));

    subscribe(event_type::rov_firmware_received,
              std::bind(&hardware_controller::on_firmware_updated,
                        this, std::placeholders::_1));

    subscribe(event_type::rov_pd_updated,
              std::bind(&hardware_controller::on_pd_updated,
                        this, std::placeholders::_1));

    subscribe(event_type::rov_pd_enable_updated,
              std::bind(&hardware_controller::on_enable_pd_updated,
                        this, std::placeholders::_1));

}

void rov::hardware_controller::on_telimetry_updated(const rov::event_ptr &event) {
    m_telimetry = event->get<rov_types::rov_telimetry>();
    //emit_control();
}

void rov::hardware_controller::on_control_updated(const rov::event_ptr &event) {
    m_control = event->get<rov_types::rov_control>();
    emit_control();
}

void rov::hardware_controller::emit_control() {
    rov_types::rov_hardware_control rhc;

    update_config();

    m_regulators.apply_horizontal(rhc, m_control, m_telimetry, m_config);
    m_regulators.apply_vertical(rhc, m_control, m_telimetry, m_config);

    rhc.manipulator_open_close = m_control.manipulator_open_close;
    rhc.manipulator_rotate = m_control.manipulator_rotate;
    for(int i = 0; i < 2; i++) {
        rhc.camera_rotate[i] = m_control.camera_rotate[i];
    }
    rhc.acoustic = m_control.acoustic;
    rhc.magnet = m_control.magnet;

    for (int i = 0; i < 4; i++) {
        rhc.twisting_motors[i] = m_control.twisting_motors[i];
    }

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

    auto m = msg.get<message_io_types::hardware>().get();

    try {
       // std::cout << std::hex << 0x2A << " " << (int)m[0] << std::endl;
        if (m_packet_handler.find(m[0]) != m_packet_handler.end()){
            auto err = m_packet_handler.at(m[0])(m);
        }

    }catch(std::exception &e){
        std::cerr << e.what() << std::endl;
    }


}

void rov::hardware_controller::init_packet_handler() {
    m_packet_handler[rov_types::rov_hardware_telimetry::meta().packet_id] =
            std::bind(&hardware_controller::on_hardware_telimetry, this, std::placeholders::_1);
}

rov_types::serializable::error_code rov::hardware_controller::on_hardware_telimetry(const std::vector<std::uint8_t> &packet) {

    rov_types::rov_hardware_telimetry ht;
    auto err = ht.deserialize(packet);
    if (rov_types::serializable::check_for_success(err)) {
        post(event_t::make_event_ptr(event_type::hardware_telemetry_updated, ht));
    }
    return err;
}

void rov::hardware_controller::on_debug(const rov::event_ptr &event) {
    auto debug = event->get<rov_types::rov_debug>();
    rov_types::rov_hardware_control hc;

    for (int i = 0; i < 4; i++) {
        hc.horizontal_power[i] = debug.thruster_power[i];
        hc.vertical_power[i] = debug.thruster_power[i + 4];
    }

    m_service->write(message_io_types::create_msg_io<message_io_types::hardware>(hc.serialize()));

}

void rov::hardware_controller::on_firmware_updated(const rov::event_ptr &event) {
    auto firmware = event->get<rov_types::rov_hardware_firmware>();
    std::ofstream bin("firmware.hex");

    bin << firmware.firmware;

    m_service->stop();
    int ret = system("flashing.sh ./firmware.hex");
    if (ret < 0) {
        std::cerr << "error on flashing" << std::endl;
    }
    if (ret >= 0) {
        std::cerr << "flashing success" << std::endl;
    }
    m_service->start();
}

void rov::hardware_controller::on_pd_updated(const rov::event_ptr &event) {
    m_config.pd = event->get<rov_types::rov_pd>();
}

void rov::hardware_controller::on_enable_pd_updated(const rov::event_ptr &event) {
    m_config.enabled_pd = event->get<rov_types::rov_enable_pd>();
}



