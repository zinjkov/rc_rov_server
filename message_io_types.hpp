//
// Created by zinjkov on 29.03.18.
//

#ifndef RC_ROV_SERVER_MESSAGE_IO_TYPES_HPP
#define RC_ROV_SERVER_MESSAGE_IO_TYPES_HPP

#include <vector>
#include <cstdint>
#include "core/service_io.hpp"
#include <opencv2/opencv.hpp>

namespace rov {
    namespace  message_io_types {
        template<class message_type, class payload_type = decltype(message_type::get())>
        message_io create_msg_io(const payload_type &payload) {
            message_type loc_msg;
            loc_msg.set(payload);
            message_io msg;
            msg.set(loc_msg);
            return msg;
        }

        template<typename payload_type>
        class base_message {
        public:
            const payload_type &get() const {
                return m_payload;
            };

            void set(const payload_type &payload){
                m_payload = payload;
            };

        protected:
            payload_type m_payload;
        };

        using imu = base_message<std::vector<std::uint8_t>>;

        using connectivity = base_message<std::vector<std::uint8_t>>;

        using hardware = base_message<std::vector<std::uint8_t>>;

        struct camera_frame {
            cv::Mat frame;
        };

        using camera = base_message<cv::Mat>;

        using camera_config = base_message<std::string>;

        using websocket = base_message<std::string>;
    }
}

#endif //RC_ROV_SERVER_MESSAGE_IO_TYPES_HPP
