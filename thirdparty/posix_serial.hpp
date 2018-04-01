//
// Created by zinjkov on 28.03.18.
//

#ifndef RC_ROV_SERVER_POSIX_SERIAL_HPP
#define RC_ROV_SERVER_POSIX_SERIAL_HPP

#include <string>
#include <vector>

class posix_serial {
public:
    posix_serial(const std::string &device_port);
    bool serial_open(const std::string &device_port);
    void serial_close();
    bool is_connected();
    ~posix_serial();
    std::size_t read_bytes(std::vector<uint8_t> &bytes);
    std::size_t write_bytes(std::vector<uint8_t> bytes);
private:
    int m_fd;
    bool m_is_opened;
};


#endif //RC_ROV_SERVER_POSIX_SERIAL_HPP
