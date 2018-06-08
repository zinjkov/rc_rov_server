//
// Created by zinjkov on 19.05.18.
//

#include "camera_io.hpp"
#include "message_io_types.hpp"
#include <thread>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#
inline void sleepFor(long long int time) {
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

rov::camera_io::camera_io(int camera_idx, const rov::io_service_ptr &service):
        service_io(service),
        m_cameras(std::make_shared<cv::VideoCapture>()),
        m_update_timer(*service)
{
    start();
    m_config.camera = camera_idx;
}

bool rov::camera_io::is_connected() {
    return m_cameras->isOpened();
}

rov::camera_io::~camera_io() {

}

void rov::camera_io::write(const rov::message_io &write_data) {
    nlohmann::json config = write_data.get<nlohmann::json>();

    auto setter = [&](int &toset, const std::string key) {
        try {
            toset = config[key];
        } catch (const std::exception &e) {
            std::cerr << "error on set key: " << key << std::endl;
            std::cerr << e.what() << std::endl;
        }
    };

//    setter(m_config.height, "height");
//    setter(m_config.height, "width");
//    setter(m_config.height, "");
//    setter(m_config.height, "height");

}

void rov::camera_io::start() {
    std::stringstream ss;
    ss << "/dev/video" << m_config.camera;
    if (access(ss.str().c_str(), F_OK) != -1) {
        sleepFor(500);
        if (m_cameras->open(m_camera_idx)) {
            m_cameras->set(CV_CAP_PROP_FRAME_WIDTH, m_config.width);
            m_cameras->set(CV_CAP_PROP_FRAME_HEIGHT, m_config.height);
            sleepFor(500);

            std::cout << "camera by path " << m_config.camera << " opened" << std::endl;
            start_update();
            return;
        }
    }
    std::cerr << "wrong camera path: " << m_camera_idx << std::endl;
}

void rov::camera_io::stop() {
    m_cameras->release();
    m_update_timer.cancel();
}

void rov::camera_io::register_on_read_handler(rov::on_read_callback read_function) {
    m_on_read_callback = read_function;
}

void rov::camera_io::start_update() {
    m_update_timer.expires_from_now(boost::posix_time::milliseconds(m_config.fps));
    m_update_timer.async_wait(boost::bind(&camera_io::update_frame, this, boost::asio::placeholders::error));
}

void rov::camera_io::update_frame(const boost::system::error_code &e) {

    if (m_cameras->isOpened()) {
        cv::Mat new_frame;
        if (m_cameras->read(new_frame)) {
            if (!new_frame.empty()) {
                std::vector<uchar> encoded;
                std::vector<int> params(2);
                params[0] = cv::IMWRITE_JPEG_QUALITY;
                params[1] = m_config.quality;
                cv::imencode(".jpg", new_frame, encoded, params);
                std::string out(encoded.size(), 0);
                std::copy(encoded.begin(), encoded.end(), out.begin());
                m_on_read_callback(message_io_types::create_msg_io<message_io_types::camera>(encoded));
            }
        }
    }
    start_update();
}
