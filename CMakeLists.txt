cmake_minimum_required(VERSION 3.16)

project(makerbase-servo42d-wrapper VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBMODBUS REQUIRED libmodbus)

add_library(servo42d_driver
src/motor.cpp
src/modbusbus.cpp
)

target_link_libraries(servo42d_driver PUBLIC ${LIBMODBUS_LIBRARIES})

target_include_directories(servo42d_driver
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${LIBMODBUS_INCLUDE_DIRS}
)

target_compile_options(servo42d_driver
    PRIVATE
        ${LIBMODBUS_CFLAGS_OTHER}
)

add_executable(servo42d_example
    main.cpp
)

target_link_libraries(servo42d_example
    PRIVATE
        servo42d_driver
)