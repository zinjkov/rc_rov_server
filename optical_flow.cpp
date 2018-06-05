//
// Created by zinjkov on 19.05.18.
//

#include "optical_flow.hpp"
#include <opencv2/opencv.hpp>
#include <string>
#include <mutex>

rov::optical_flow::optical_flow(const std::shared_ptr<rov::event_bus> &event_bus_):
            eventable(event_bus_) {
    subscribe_to_event();
    std::stringstream ss;
    m_name = ss.str();
}

void rov::optical_flow::subscribe_to_event() {
    subscribe(event_type::camera_one_frame_updated, std::bind(&optical_flow::on_frame_updated,
                                                   this, std::placeholders::_1));
}

#include "regulator/timer.hpp"

static void drawOptFlowMap(const cv::Mat& flow, cv::Mat& cflowmap, int step,
                           double, const cv::Scalar& color)
{
    for(int y = 0; y < cflowmap.rows; y += step)
        for(int x = 0; x < cflowmap.cols; x += step)
        {
            const cv::Point2f& fxy = flow.at<cv::Point2f>(y, x);
            cv::line(cflowmap, cv::Point(x,y), cv::Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                 color);
            cv::circle(cflowmap, cv::Point(x,y), 2, color, -1);
        }
}

void rov::optical_flow::on_frame_updated(const rov::event_ptr &e) {
//    static std::mutex mutex;
//    if (!mutex.try_lock()) {
//        return;
//    }
//    static timer t;
//    static int fps = 0;
//    if (!t.is_started()) {
//        t.start();
//    }
//    fps++;
//    if (t.elapsed() > 1000LL) {
//        std::cout << "frame rate: " << fps << std::endl;
//        fps = 0;
//        t.restart();
//    }
//    std::cout << "frame rate: " << fps << std::endl;

    cv::Mat frame = e->get<cv::Mat>().clone();
    cv::imshow("flow", frame);
    cv::waitKey(1);
//    static cv::Mat flow, cflow;
//    static cv::Mat prevgray, uflow;
//    cv::namedWindow("flow", 1);
//
//    cv::Mat gray;
//    cv::Mat frame = e->get<cv::Mat>().clone();
//
//    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
//
//    if( !prevgray.empty() )
//    {
//        cv::calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);
//        cv::cvtColor(prevgray, cflow, CV_GRAY2BGR);
//        uflow.copyTo(flow);
//
//        drawOptFlowMap(flow, cflow, 16, 1.5, cv::Scalar(0, 255, 0));
//        cv::imshow("flow", cflow);
//        //std::cout << "CV IMSHOW" << std::endl;
//        cv::waitKey(1);
//    }
//
//    std::swap(prevgray, gray);
//    mutex.unlock();
}
