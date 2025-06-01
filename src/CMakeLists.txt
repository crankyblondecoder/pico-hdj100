cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(pico_hdj100 C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# Add extra source files here.
add_executable(pico_hdj100
	pico_hdj100.cpp
	PicoSwitch.cpp)

# Set to 1 to enable.
pico_enable_stdio_usb(pico_hdj100 1)
pico_enable_stdio_uart(pico_hdj100 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(pico_hdj100)

target_link_libraries(pico_hdj100
	pico_stdlib
	pico_time
	pico_platform
	hardware_gpio)
