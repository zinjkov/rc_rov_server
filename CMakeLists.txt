cmake_minimum_required(VERSION 3.0)
project(rc_rov_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

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


set(SOURCE_FILES main.cpp core/event_t.cpp core/event_t.hpp core/event_bus.cpp core/event_bus.hpp name_allias.hpp imu_io.cpp imu_io.hpp
        core/service_io.cpp core/service_io.hpp imu_service.cpp imu_service.hpp core/service_logic.cpp core/service_logic.hpp
        core/eventable.cpp core/eventable.hpp
        thirdparty/imu_data_decode.c
        thirdparty/packet.c core/event_types.hpp thirdparty/posix_serial.cpp thirdparty/posix_serial.hpp connectivity_logic.cpp connectivity_logic.hpp connectivity_io.cpp connectivity_io.hpp connectivity/session.cpp connectivity/session.hpp message_io_types.hpp rov_types/serializable.cpp rov_types/serializable.hpp rov_types/rov_data_types.cpp rov_types/rov_data_types.hpp rov_types/binary_stream.cpp rov_types/binary_stream.hpp rov_types/crc.cpp rov_types/crc.hpp)

add_executable(rc_rov_server ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})