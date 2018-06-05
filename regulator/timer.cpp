//
// Created by zinjkov on 19.05.18.
//

#include "timer.hpp"

timer::timer() :  m_is_started(false) {

}

timer::~timer() {

}

void timer::start() {
    m_start = std::chrono::steady_clock::now();
    m_is_started = true;
}

void timer::stop() {
    m_is_started = false;
}

void timer::restart() {
    stop();
    start();
}

long long int timer::elapsed() {
    return m_is_started ?
           std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_start).count()
                   : 0;

}

bool timer::is_started() {
    return m_is_started;
}
