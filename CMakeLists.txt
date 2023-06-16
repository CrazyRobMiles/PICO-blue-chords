# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/pico-sdk")

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(PICO-blue-chords C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(PICO-blue-chords
 PICO-blue-chords.c
 picow_bt_example_common.c
 picow_bt_example_poll.c
 ssd1306.c
 keyscan.c
 messages.c
)

pico_set_program_name(PICO-blue-chords "PICO-blue-chords")
pico_set_program_version(PICO-blue-chords "0.1")

pico_enable_stdio_uart(PICO-blue-chords 1)
pico_enable_stdio_usb(PICO-blue-chords 0)

# Add the standard library to the build
target_link_libraries(PICO-blue-chords
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(PICO-blue-chords PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(PICO-blue-chords 
        pico_stdlib
        hardware_i2c
        pico_btstack_ble
        pico_btstack_classic
        pico_btstack_cyw43
        pico_cyw43_arch_lwip_poll
        )

pico_add_extra_outputs(PICO-blue-chords)

