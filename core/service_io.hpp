//
// Created by zinjkov on 18.03.18.
//

#ifndef RC_ROV_SERVER_DRIVER_IO_HPP
#define RC_ROV_SERVER_DRIVER_IO_HPP

#include "../name_allias.hpp"
#include <boost/any.hpp>
namespace rov {

    class message_io {
    public:
        message_io() : m_payload("empty payload") {};
        template<class T>
        const T &get() const{
            return boost::any_cast<const T &>(m_payload);
        }

        template<class T>
        void set(const T &payload) {
            m_payload = payload;
        }
    private:
        boost::any m_payload;
    };


    class service_io {
    public:

        service_io(const io_service_ptr &service);

        virtual ~service_io();

        virtual void register_on_read_handler(on_read_callback read_function);

        virtual void write(const message_io &write_data) = 0;

        virtual void start() = 0;

        virtual void stop() = 0;

        virtual bool is_connected() = 0;

    protected:
        io_service_ptr m_io_service;

        on_read_callback m_on_read_callback;

    private:
    };
}


#endif //RC_ROV_SERVER_DRIVER_IO_HPP
