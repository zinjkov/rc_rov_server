//
// Created by zinjkov on 19.05.18.
//

#ifndef RC_ROV_SERVER_CAMERAS_LOGIC_HPP
#define RC_ROV_SERVER_CAMERAS_LOGIC_HPP

#include "core/service_logic.hpp"
#include "core/eventable.hpp"
namespace rov {
    class cameras_logic : public service_logic, eventable {
    public:
        cameras_logic(const std::shared_ptr<service_io> &driver_, const std::shared_ptr<event_bus> &event_bus_);
        ~cameras_logic();

        virtual void on_read(const message_io &msg) override final;

    private:
        void on_camera_config_recvied(const std::string &config);

    };
}


#endif //RC_ROV_SERVER_CAMERAS_LOGIC_HPP
