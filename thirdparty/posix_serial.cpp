//
// Created by zinjkov on 28.03.18.
//

extern "C" {
#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
}

#include <stdexcept>
#include <iostream>
#include "posix_serial.hpp"

posix_serial::posix_serial(const std::string &device_port) : m_is_opened(false) {
    m_is_opened = serial_open(device_port);
}

bool posix_serial::serial_open(const std::string &device_port) {
    if (m_is_opened){
        close(m_fd);
    }

    struct termios options;

    m_fd = open(device_port.data(), O_RDWR | O_NOCTTY | O_NONBLOCK);

    tcgetattr(m_fd, &options);

    if (m_fd == -1)
    {
        perror("open_port: Unable to open SerialPort");
        return false;
    }

    if(fcntl(m_fd, F_SETFL, 0) < 0)
    {
        printf("fcntl failed\n");
        return false;
    }
    else
    {
        fcntl(m_fd, F_SETFL, 0);
    }

    if(isatty(STDIN_FILENO) == 0)
    {
        printf("standard input is not a terminal device\n");
        return false;
    }
    else
    {
        printf("isatty success!\n");
    }

    bzero(&options, sizeof(options));

    options.c_cflag = B115200 | CS8 | CLOCAL |CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    options.c_cc[VTIME] = 0;
    options.c_cc[VMIN] = 0;
    tcflush(m_fd,TCIFLUSH);
    tcsetattr(m_fd, TCSANOW, &options);
    return true;
}

void posix_serial::serial_close() {
    if (m_is_opened) {
        close(m_fd);
    }
}

posix_serial::~posix_serial() {
    serial_close();
}

std::size_t posix_serial::read_bytes(std::vector<uint8_t> &bytes) {
    if (!m_is_opened) {
        bytes.resize(0);
        std::cerr << "Trying read from posix serial from closed port" << std::endl;
        return bytes.size();
    }
    if (bytes.size() == 0) {
        bytes.resize(1024);
    }

    ssize_t received_bytes = read(m_fd, bytes.data(), bytes.size());

    if (received_bytes != -1) {
        bytes.resize(static_cast<std::size_t>(received_bytes));
    } else {
        std::string err_msg = std::to_string(m_fd) + std::string(" file discriptor in posix serial error");
        throw std::runtime_error(err_msg);
    }

    return bytes.size();
}

std::size_t posix_serial::write_bytes(std::vector<uint8_t> bytes) {
    if (bytes.size() == 0) {
        return 0;
    }
    return static_cast<std::size_t>(write(m_fd, bytes.data(), bytes.size()));

}

bool posix_serial::is_connected() {
    return m_is_opened;
}

