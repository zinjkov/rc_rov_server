//
// Created by zinjkov on 27.03.18.
//

#ifndef RC_ROV_SERVER_SERVICE_LOGIC_HPP
#define RC_ROV_SERVER_SERVICE_LOGIC_HPP

#include "../name_allias.hpp"
#include <map>
#include <vector>
#include "service_io.hpp"

namespace rov {
    class service_logic {
    public:
        service_logic(const std::shared_ptr<service_io> &driver_);

        virtual ~service_logic();

        virtual void on_read(const message_io &msg) = 0;

    protected:
        std::shared_ptr<service_io> m_service;

    };
}

#endif //RC_ROV_SERVER_SERVICE_LOGIC_HPP
