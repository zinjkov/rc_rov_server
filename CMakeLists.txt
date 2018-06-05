cmake_minimum_required(VERSION 3.0)
project(rc_rov_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O3")

find_package(Boost COMPONENTS system filesystem REQUIRED)
if (Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
endif ()

find_package(Threads REQUIRED)
if (Threads_FOUND)
    message(STATUS "CMAKE_THREAD_LIBS_INIT: ${CMAKE_THREAD_LIBS_INIT}")
endif ()

find_package(serial REQUIRED)
if (serial_FOUND)
    message(STATUS "serial_INCLUDE_DIRS: ${serial_INCLUDE_DIRS}")
    message(STATUS "serial_LIBRARIES: ${serial_LIBRARIES}")
    message(STATUS "serial_VERSION: ${serial_VERSION}")
endif ()

find_package(OpenCV REQUIRED)
if (OpenCV_FOUND)
    message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
    message(STATUS "OpenCV_LIBRARIES: ${OpenCV_LIBRARIES}")
    message(STATUS "OpenCV_VERSION: ${OpenCV_VERSION}")
endif ()

include_directories(${PROJECT_SOURCE_DIR}/thirdparty)

set(SOURCE_FILES main.cpp core/event_t.cpp core/event_t.hpp core/event_bus.cpp core/event_bus.hpp
        name_allias.hpp imu_io.cpp imu_io.hpp
        core/service_io.cpp core/service_io.hpp imu_service.cpp imu_service.hpp
        core/service_logic.cpp core/service_logic.hpp
        core/eventable.cpp core/eventable.hpp
        thirdparty/imu_data_decode.c
        thirdparty/packet.c core/event_types.hpp thirdparty/posix_serial.cpp thirdparty/posix_serial.hpp
        connectivity_logic.cpp connectivity_logic.hpp connectivity_io.cpp connectivity_io.hpp connectivity/session.cpp
        connectivity/session.hpp message_io_types.hpp rov_types/serializable.cpp rov_types/serializable.hpp
        rov_types/rov_data_types.cpp rov_types/rov_data_types.hpp
        rov_types/binary_stream.cpp rov_types/binary_stream.hpp
        rov_types/crc.cpp rov_types/crc.hpp data_store.cpp data_store.hpp hardware_controller.cpp hardware_controller.hpp
        regulator/basic_regulator.cpp regulator/basic_regulator.hpp regulator/horizontal_regulator.cpp
        regulator/horizontal_regulator.hpp
        regulator/yaw_regulator.cpp regulator/yaw_regulator.hpp regulator/vertical_regulator.cpp
        regulator/vertical_regulator.hpp
        regulator/regulator_sequence.cpp regulator/regulator_sequence.hpp
        regulator/regulators.hpp regulator/depth_regulator.cpp
        regulator/depth_regulator.hpp hardware_io.cpp hardware_io.hpp regulator/timer.cpp regulator/timer.hpp
        regulator/roll_regulator.cpp regulator/roll_regulator.hpp regulator/pitch_regulator.cpp
        regulator/pitch_regulator.hpp camera_io.cpp camera_io.hpp
        cameras_logic.cpp cameras_logic.hpp
        optical_flow.cpp optical_flow.hpp websocket_io.cpp websocket_io.hpp
        websocket_logic.cpp websocket_logic.hpp esp_logic.cpp esp_logic.hpp leo_logic.cpp
        leo_logic.hpp info_logic.cpp info_logic.hpp)

add_executable(rc_rov_server ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${serial_LIBRARIES} ${OpenCV_LIBS})