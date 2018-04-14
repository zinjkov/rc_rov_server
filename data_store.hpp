//
// Created by zinjkov on 01.04.18.
//

#ifndef RC_ROV_SERVER_DATA_STORE_HPP
#define RC_ROV_SERVER_DATA_STORE_HPP

#include "core/service_logic.hpp"
#include "core/eventable.hpp"
#include "rov_types/rov_data_types.hpp"

namespace rov {
    class data_store : public eventable {
    public:
        data_store(const std::shared_ptr<event_bus> &event_bus_);

        virtual ~data_store();

    private:
        void subscribe_to_event();
        void on_control_updated(const rov::event_ptr &event);
        void on_imu_updated(const rov::event_ptr &event);
        void on_hardware_telimetry_updated(const rov::event_ptr &event);

        rov_types::rov_control m_control;
        rov_types::rov_telimetry m_telimetry;

    };
}


#endif //RC_ROV_SERVER_DATA_STORE_HPP
