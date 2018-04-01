//
// Created by zinjkov on 28.03.18.
//

#ifndef RC_ROV_SERVER_EVENTS_TYPES_HPP
#define RC_ROV_SERVER_EVENTS_TYPES_HPP

#include "../name_allias.hpp"
namespace rov {
    struct event_type {
        enum : event_id_t {
            imu_config,
            imu_data_recieved,

            connectivity_transmit_request,
            rov_control_received
        };
    };

};

#endif //RC_ROV_SERVER_EVENTS_TYPES_HPP
