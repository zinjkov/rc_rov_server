//
// Created by zinjkov on 18.03.18.
//

#ifndef RC_ROV_SERVER_IMU_SENSOR_HPP
#define RC_ROV_SERVER_IMU_SENSOR_HPP


#include "core/service_logic.hpp"
#include "core/eventable.hpp"

namespace rov {
    class imu_service : service_logic, eventable {
    public:
        imu_service(const std::shared_ptr<service_io> &driver_, const std::shared_ptr<event_bus> &event_bus_);
        ~imu_service();

        struct eular_angles {
            float yaw;
            float pitch;
            float roll;
        };

    private:
        void subscribe_to_events();
        void on_read(const message_io &msg) override;

        void on_imu_config_event(const event_ptr &ev);

        void packet_decoding(const std::vector<std::uint8_t> &bytes);
        eular_angles get_angles();

    };
}


#endif //RC_ROV_SERVER_IMU_SENSOR_HPP
