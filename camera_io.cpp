//
// Created by zinjkov on 19.05.18.
//

#include "camera_io.hpp"
#include "message_io_types.hpp"
#include <thread>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <mutex>

#
inline void sleepFor(long long int time) {
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

rov::camera_io::camera_io(int camera_idx, const rov::io_service_ptr &service):
        service_io(service),
        m_update_timer(*service)
{
    m_config.camera = camera_idx;
    is_open = false;
    start();

}

bool rov::camera_io::is_connected() {
    return m_cameras->isOpened();
}

rov::camera_io::~camera_io() {

}

void rov::camera_io::write(const rov::message_io &write_data) {
    std::lock_guard<std::mutex> lock(m_camera_guard);
    nlohmann::json config = write_data.get<message_io_types::camera_config>().get();
    bool reset = false;
    auto setter = [&](int &toset, const std::string key) {
        try {
            toset = config[key];
            if (toset != config[key]) {
                if (key != "fps"){
                    reset = true;
                } else {
                    toset = toset > 20 ? toset : 20;
                }
            }

            std::cout << "camera setting change " << key << " set to " << toset << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "error on set key: " << key << std::endl;
            std::cerr << e.what() << std::endl;
        }
    };

    setter(m_config.height, "height");
    setter(m_config.width, "width");
    setter(m_config.quality, "quality");
    setter(m_config.fps, "fps");
    setter(m_config.camera, "camera");

//    if (reset) {
        stop();
        sleepFor(100);
        start();
//    }

}

void rov::camera_io::start() {
    if (is_open) return;
    std::stringstream ss;
    ss << "/dev/video" << m_config.camera;
    if (access(ss.str().c_str(), F_OK) != -1) {
        cv::waitKey(10);
        m_cameras = std::make_shared<cv::VideoCapture>();
        cv::waitKey(10);
        sleepFor(50);
        if (m_cameras->open(m_config.camera)) {
            is_open = true;
            m_cameras->set(CV_CAP_PROP_FRAME_WIDTH, m_config.width);
            m_cameras->set(CV_CAP_PROP_FRAME_HEIGHT, m_config.height);
            sleepFor(100);
            std::cout << "camera by path " << m_config.camera << " opened" << std::endl;
            start_update();
            return;
        }
    }
    std::cerr << "wrong camera path: " << m_config.camera << std::endl;
}

void rov::camera_io::stop() {
    is_open = false;
    if(m_cameras == nullptr) return;
    cv::waitKey(10);
    m_cameras->release();
    cv::waitKey(10);
    m_cameras.reset();
    cv::waitKey(10);
    m_update_timer.cancel();
    std::cout << "STOPED" << std::endl;
}

void rov::camera_io::register_on_read_handler(rov::on_read_callback read_function) {
    m_on_read_callback = read_function;
}

void rov::camera_io::start_update() {
    m_update_timer.expires_from_now(boost::posix_time::milliseconds(m_config.fps));
    m_update_timer.async_wait(boost::bind(&camera_io::update_frame, this, boost::asio::placeholders::error));
}

void rov::camera_io::update_frame(const boost::system::error_code &e) {
    std::lock_guard<std::mutex> lock(m_camera_guard);
    if (is_open) {
//        std::cout << "new_Frame" << std::endl;
        if (m_cameras->isOpened()) {
            cv::Mat new_frame;
            if (m_cameras->read(new_frame)) {
                if (!new_frame.empty()) {
                    m_on_read_callback(message_io_types::create_msg_io<message_io_types::camera>(new_frame));
                }
            }
        }
    }
    start_update();
}
