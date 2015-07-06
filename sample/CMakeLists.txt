cmake_minimum_required(VERSION 2.8)

set(MCU   atmega32)
set(F_CPU 16000000)
set(BOOTLOADER_START "0x7000")
include("${CMAKE_CURRENT_SOURCE_DIR}/../AVR.cmake")

project(OpenMAOR-sample-program C CXX)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src")

file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(${PROJECT_NAME}.elf ${SRC_FILES})
target_link_libraries(${PROJECT_NAME}.elf "${CMAKE_CURRENT_SOURCE_DIR}/../build/libOpenMAOR-framework.a")
avr_target_configure(${PROJECT_NAME}.elf)