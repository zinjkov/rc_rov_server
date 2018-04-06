#include <boost/asio.hpp>
#include <thread>
#include "core/event_bus.hpp"

#include "imu_io.hpp"
#include "imu_service.hpp"

#include "connectivity_io.hpp"
#include "connectivity_logic.hpp"
#include "data_store.hpp"
#include "hardware_io.hpp"
#include "hardware_controller.hpp"


inline void sleepFor(long long int time) {
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

int main() {
    rov::io_service_ptr io_service = std::make_shared<boost::asio::io_service>(103);

    std::shared_ptr<rov::event_bus> event_bus_ = std::make_shared<rov::event_bus>(io_service);

    auto hardware_io =  std::make_shared<rov::hardware_io>(io_service);
    auto hardware_controller = std::make_shared<rov::hardware_controller>(hardware_io, event_bus_);

    std::shared_ptr<rov::service_io> imu_ = std::make_shared<rov::imu_io>(io_service);
    std::shared_ptr<rov::imu_service> imu_serivce_ = std::make_shared<rov::imu_service>(imu_, event_bus_);

    std::shared_ptr<rov::service_io> connectivity_ = std::make_shared<rov::connectivity_io>(io_service);
    std::shared_ptr<rov::connectivity_logic> connectivity_logic_ = std::make_shared<rov::connectivity_logic>(connectivity_, event_bus_);

    auto data_store = std::make_shared<rov::data_store>(event_bus_);

    io_service->run();
    return 0;
}