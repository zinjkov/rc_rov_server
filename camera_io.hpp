//
// Created by zinjkov on 19.05.18.
//

#ifndef RC_ROV_SERVER_CAMERA_IO_HPP
#define RC_ROV_SERVER_CAMERA_IO_HPP

#include <boost/asio/deadline_timer.hpp>
#include <mutex>
#include "core/service_io.hpp"
#include "opencv2/opencv.hpp"

namespace rov {
    class camera_io : public service_io {
    public:
        camera_io(int camera_idx, const io_service_ptr &service);

        virtual ~camera_io();

        virtual void register_on_read_handler(on_read_callback read_function);

        virtual void write(const message_io &write_data) ;

        virtual void start();

        virtual void stop();

        virtual bool is_connected();

        void start_update();

        void update_frame(const boost::system::error_code &e);
    private:
        std::shared_ptr<cv::VideoCapture> m_cameras;
        boost::asio::deadline_timer m_update_timer;
        bool is_open;
        std::mutex m_camera_guard;
        struct config {
            int height = 600;
            int width = 800;
            int fps = 30;
            int quality = 50;
            int camera = 0;
        } m_config;
    };
}


#endif //RC_ROV_SERVER_CAMERA_IO_HPP
