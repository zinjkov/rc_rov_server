//
// Created by zinjkov on 19.05.18.
//

#ifndef RC_ROV_SERVER_TIMER_HPP
#define RC_ROV_SERVER_TIMER_HPP

#include <chrono>
#include <thread>
class timer {
public:
    timer();
    ~timer();

    void start();
    void stop();
    void restart();
    long long int elapsed();
    bool is_started();

private:
    std::chrono::steady_clock::time_point m_start;
    bool m_is_started;
};


#endif //RC_ROV_SERVER_TIMER_HPP
