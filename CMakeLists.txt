cmake_minimum_required(VERSION 2.8)

set(MCU   atmega32)
set(F_CPU 16000000)
set(BOOTLOADER_START "0x7000")
include(AVR.cmake)

project(OpenMAOR C CXX)


# Pliki źródłowe
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB COMMON_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/common/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/common/stdlib/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/peripherals/*.cpp")
add_library(${PROJECT_NAME}-common OBJECT ${COMMON_SRC})

file(GLOB FRAMEWORK_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/framework/*.cpp")
add_library(${PROJECT_NAME}-framework STATIC $<TARGET_OBJECTS:${PROJECT_NAME}-common> ${FRAMEWORK_SRC})

# Lista symboli w bibliotece do frameworka
add_custom_target(
    framework-symbols
    ALL
    nm -gCS --defined-only lib${PROJECT_NAME}-framework.a | tee "lib${PROJECT_NAME}-framework.symbols.txt"
    DEPENDS ${PROJECT_NAME}-framework
)
add_custom_command(
    OUTPUT ${PROJECT_NAME}-framework.disasm.txt
    COMMAND ${AVR_OBJDUMP} -d -m avr5 "lib${PROJECT_NAME}-framework.a" > "${PROJECT_NAME}-framework.disasm.txt"
    DEPENDS ${PROJECT_NAME}-framework
)
add_custom_target(framework-disasm ALL DEPENDS ${PROJECT_NAME}-framework.disasm.txt)

file(GLOB BOOTLOADER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/bootloader/*.cpp")
add_executable(${PROJECT_NAME}-bootloader.elf $<TARGET_OBJECTS:${PROJECT_NAME}-common> ${BOOTLOADER_SRC})
avr_target_configure_as_bootloader(${PROJECT_NAME}-bootloader.elf)
avr_target_configure(${PROJECT_NAME}-bootloader.elf)
