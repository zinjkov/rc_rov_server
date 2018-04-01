#include <boost/asio.hpp>
#include "core/event_bus.hpp"

#include "imu_io.hpp"
#include "imu_service.hpp"

#include "connectivity_io.hpp"
#include "connectivity_logic.hpp"

int main() {
    rov::io_service_ptr io_service = std::make_shared<boost::asio::io_service>();

    std::shared_ptr<rov::event_bus> event_bus_ = std::make_shared<rov::event_bus>(io_service);

//    std::shared_ptr<rov::service_io> imu_ = std::make_shared<rov::imu_io>(io_service);
//    std::shared_ptr<rov::imu_service> imu_serivce_ = std::make_shared<rov::imu_service>(imu_, event_bus_);

    std::shared_ptr<rov::service_io> connectivity_ = std::make_shared<rov::connectivity_io>(io_service);
    std::shared_ptr<rov::connectivity_logic> connectivity_logic_ = std::make_shared<rov::connectivity_logic>(connectivity_, event_bus_);

    io_service->run();
    return 0;
}