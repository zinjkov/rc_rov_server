//
// Created by zinjkov on 19.05.18.
//

#include "camera_io.hpp"
#include "message_io_types.hpp"
#include <thread>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

inline void sleepFor(long long int time) {
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

rov::camera_io::camera_io(int camera_idx, const rov::io_service_ptr &service):
        service_io(service),
        m_cameras(std::make_shared<cv::VideoCapture>()),
        m_camera_idx(camera_idx),
        m_update_timer(*service)
{
    start();
}

bool rov::camera_io::is_connected() {
    return m_cameras->isOpened();
}

rov::camera_io::~camera_io() {

}

void rov::camera_io::write(const rov::message_io &write_data) {
    //std::string config = write_data.get<std::string>();
}

void rov::camera_io::start() {

    if (m_camera_idx == 1) {
        m_camera_idx = 2;
    } else {
        m_camera_idx = 0;
    }

    std::stringstream ss;
    ss << "/dev/video" << m_camera_idx;

    if (access(ss.str().c_str(), F_OK) != -1) {
        sleepFor(500);
        if (m_cameras->open(m_camera_idx)) {
            m_cameras->set(CV_CAP_PROP_FRAME_WIDTH, 800);
            m_cameras->set(CV_CAP_PROP_FRAME_HEIGHT, 600);
//            m_cameras->set(CV_CAP_PROP_FPS, 60);

            sleepFor(500);

            std::cout << "camera by path " << m_camera_idx << " opened" << std::endl;
            start_update();
            m_camera_idx++;
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
    m_update_timer.expires_from_now(boost::posix_time::milliseconds(15));
    m_update_timer.async_wait(boost::bind(&camera_io::update_frame, this, boost::asio::placeholders::error));
}

void rov::camera_io::update_frame(const boost::system::error_code &e) {

    static cv::Mat frame;
    if (m_cameras->isOpened()) {
        cv::Mat new_frame;
        if (m_cameras->read(new_frame)) {
            if (!new_frame.empty()) {
                if (frame.empty()) {
                    frame = new_frame;
                    start_update();
                    return;
                }
                cv::Mat diff;
                cv::Mat gray1, gray2;
                cv::cvtColor(frame, gray1, CV_BGR2GRAY);
                cv::cvtColor(new_frame, gray2, CV_BGR2GRAY);
                cv::compare(gray1, gray2, diff, cv::CMP_NE);

                if (cv::countNonZero(diff) != 0) {
                    m_on_read_callback(message_io_types::create_msg_io<message_io_types::camera>(new_frame));
                    frame = new_frame;
                } else {
                    stop();
                    start();
                    return;
                }
            }
        }
    }
    start_update();
}
