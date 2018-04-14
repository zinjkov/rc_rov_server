//
// Created by zinjkov on 28.03.18.
//

#ifndef RC_ROV_SERVER_EVENTS_TYPES_HPP
#define RC_ROV_SERVER_EVENTS_TYPES_HPP


namespace rov {
    using event_id_t = std::size_t;
    struct event_type {
        enum event_id : event_id_t {
            imu_config,
            imu_data_recieved,

            connectivity_transmit_request,
            rov_control_received,

            telimetry_updated,
            hardware_telemetry_updated
        };
    };

};

#endif //RC_ROV_SERVER_EVENTS_TYPES_HPP
