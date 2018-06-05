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
#include "camera_io.hpp"
#include "cameras_logic.hpp"
#include "optical_flow.hpp"
#include "websocket_io.hpp"
#include "websocket_logic.hpp"
#include "esp_logic.hpp"
#include "leo_logic.hpp"

inline void sleepFor(long long int time) {
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}


#include <nlohmann_json/json.hpp>
int main() {

    rov::io_service_ptr io_service = std::make_shared<boost::asio::io_service>(4);

    std::shared_ptr<rov::event_bus> event_bus_ = std::make_shared<rov::event_bus>(io_service);

    auto camera_io = std::make_shared<rov::camera_io>(0, io_service);
    auto cameras_logic = std::make_shared<rov::cameras_logic>(camera_io, event_bus_);

    auto websocket_io = std::make_shared<rov::websocket_io>(3090, io_service);
    auto websocket_logic = std::make_shared<rov::websocket_logic>(websocket_io, event_bus_);

    //auto webinfo_io = std::make_shared<rov::websocket_io>(4090, io_service);

    auto optical_flow = std::make_shared<rov::optical_flow>(event_bus_);

//    auto esp_io = std::make_shared<rov::hardware_io>("/dev/ttyS2", io_service);
//    auto esp_logic = std::make_shared<rov::esp_logic>(esp_io, event_bus_);
//
//    auto leo_io =  std::make_shared<rov::hardware_io>("/dev/ttyACM0", io_service);
//    auto leo_logic = std::make_shared<rov::leo_logic>(leo_io, event_bus_);
//
//    auto hardware_io =  std::make_shared<rov::hardware_io>("/dev/ttyUSB0", io_service);
//    auto hardware_controller = std::make_shared<rov::hardware_controller>(hardware_io, event_bus_);
//
//    std::shared_ptr<rov::service_io> imu_ = std::make_shared<rov::imu_io>(io_service);
//    std::shared_ptr<rov::imu_service> imu_serivce_ = std::make_shared<rov::imu_service>(imu_, event_bus_);
//
//    std::shared_ptr<rov::service_io> connectivity_ = std::make_shared<rov::connectivity_io>(io_service);
//    std::shared_ptr<rov::connectivity_logic> connectivity_logic_ = std::make_shared<rov::connectivity_logic>(connectivity_, event_bus_);

    auto data_store = std::make_shared<rov::data_store>(event_bus_);

//    for (int i = 0; i < 3; i++) {
//        std::thread th3([&io_service]() { io_service->run(); });
//        th3.detach();
//    }

    std::cout << "started in 4 thread" << std::endl;

    io_service->run();
    return 0;
}