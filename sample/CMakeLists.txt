cmake_minimum_required(VERSION 2.8)

set(OPENMAOR_PATH "${CMAKE_CURRENT_SOURCE_DIR}/..")

set(MCU   atmega32)
set(F_CPU 16000000)
include(${OPENMAOR_PATH}/AVR.cmake)

project(OpenMAOR-sample-program C CXX)

include(ExternalProject)
ExternalProject_Add(
        OpenMAOR
        DOWNLOAD_COMMAND ""
        INSTALL_COMMAND ""
        SOURCE_DIR "${OPENMAOR_PATH}"
        CMAKE_ARGS "-DCMAKE_CXX_FLAGS=-mmcu=${MCU} -gstabs -g -ggdb -DF_CPU=${F_CPU} -Os -lm ${PRINTF_LIB} -funsigned-char -funsigned-bitfields -fshort-enums -ffunction-sections -fdata-sections -std=c++11" # TODO: why is this needed?
)
ExternalProject_Get_Property(OpenMAOR BINARY_DIR)
link_directories(${BINARY_DIR})
ExternalProject_Get_Property(OpenMAOR SOURCE_DIR)
include_directories(${SOURCE_DIR}/src)

set(SRC main.cpp)

add_executable(${PROJECT_NAME}.elf ${SRC})
add_dependencies(${PROJECT_NAME}.elf OpenMAOR)
target_link_libraries(${PROJECT_NAME}.elf OpenMAOR-framework)
avr_target_configure(${PROJECT_NAME}.elf)
