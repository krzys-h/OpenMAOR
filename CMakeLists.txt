cmake_minimum_required(VERSION 2.8)

# Konfiguracja kompilatora, MUSI być przed project()
find_program(AVR_CC avr-gcc)
find_program(AVR_CXX avr-g++)
find_program(AVR_STRIP avr-strip)
find_program(AVR_OBJCOPY avr-objcopy)
find_program(AVR_OBJDUMP avr-objdump)
find_program(AVR_SIZE_TOOL avr-size)

message(STATUS "avr-gcc path: ${AVR_CC}")
message(STATUS "avr-g++ path: ${AVR_CXX}")
message(STATUS "avr-strip path: ${AVR_STRIP}")
message(STATUS "avr-objcopy path: ${AVR_OBJCOPY}")
message(STATUS "avr-objdump path: ${AVR_OBJDUMP}")
message(STATUS "avr-size path: ${AVR_SIZE_TOOL}")

#TODO: to nie działa
#if(NOT ${AVR_CC} OR NOT ${AVR_CXX} OR NOT ${AVR_STRIP} OR NOT ${AVR_OBJCOPY} OR NOT ${AVR_OBJDUMP} OR NOT ${AVR_SIZE_TOOL})
#    message(FATAL_ERROR "Some required tools from AVR toolchain could not be found!")
#endif()

set(CMAKE_SYSTEM_NAME         Generic)
set(CMAKE_C_COMPILER          ${AVR_CC})
set(CMAKE_C_COMPILER_FORCED   TRUE)
set(CMAKE_CXX_COMPILER        ${AVR_CXX})
set(CMAKE_CXX_COMPILER_FORCED TRUE)

project(OpenMAOR C CXX)

set(MCU   atmega32)
set(F_CPU 16000000)
set(BOOTLOADER_START "0x7000")
add_definitions(-DBOOTLOADER_START=${BOOTLOADER_START})

set(PRINTF_LIB "")
# Mozna sprobowac odkomentowac jesli bedzie nam brakowac miejsca
#set(PRINTF_LIB "-Wl,-u,vfprintf -lprintf_min")
# Mozna zmienic na printf_flt jesli bedzie trzeba, ale mam nadzieje ze nie

# https://github.com/acristoffers/CMake-AVR/blob/master/Blink/CMakeLists.txt#L66-80
# Flagi kompilatora
set(CSTANDARD "-std=gnu99")
set(CDEBUG    "-gstabs -g -ggdb")
set(CWARN     "-Wall -Wstrict-prototypes -Wl,--gc-sections -Wl,--relax")
set(CTUNING   "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
set(COPT      "-Os -lm ${PRINTF_LIB}")
set(CMCU      "-mmcu=${MCU}")
set(CDEFS     "-DF_CPU=${F_CPU}")

set(CFLAGS   "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CSTANDARD} ${CTUNING}")
set(CXXFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CTUNING}")
set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")

# Pliki źródłowe
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB COMMON_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/common/*.cpp")
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
add_executable(${PROJECT_NAME}-bootloader.debug.elf $<TARGET_OBJECTS:${PROJECT_NAME}-common> ${BOOTLOADER_SRC})
target_link_libraries(${PROJECT_NAME}-bootloader.debug.elf "-Wl,--section-start=.text=${BOOTLOADER_START}") # Przesuwamy sekcję .text do BOOTLOADER_START

# Generowanie pliku .hex dla bootloadera
# Stworzone na podstawie https://github.com/acristoffers/CMake-AVR/blob/master/Blink/CMakeLists.txt#L87-110

# Po utworzeniu pliku .elf, wykonaj strip
add_custom_command(
    OUTPUT ${PROJECT_NAME}-bootloader.elf
    COMMAND ${AVR_STRIP} "${PROJECT_NAME}-bootloader.debug.elf" -o "${PROJECT_NAME}-bootloader.elf"
    DEPENDS ${PROJECT_NAME}-bootloader.debug.elf
)
add_custom_target(strip ALL DEPENDS ${PROJECT_NAME}-bootloader.debug.elf)

# Tworzenie pliku .hex z .elf, zawsze
add_custom_command(
    OUTPUT ${PROJECT_NAME}-bootloader.hex
    COMMAND ${AVR_OBJCOPY} -R .eeprom -O ihex "${PROJECT_NAME}-bootloader.elf" "${PROJECT_NAME}-bootloader.hex"
    DEPENDS ${PROJECT_NAME}-bootloader.elf
)
add_custom_target(hex ALL DEPENDS ${PROJECT_NAME}-bootloader.hex)

# disasm, zawsze
add_custom_command(
    OUTPUT ${PROJECT_NAME}-bootloader.disasm.txt
    COMMAND ${AVR_OBJDUMP} -d -m avr5 "${PROJECT_NAME}-bootloader.debug.elf" > "${PROJECT_NAME}-bootloader.disasm.txt"
    DEPENDS ${PROJECT_NAME}-bootloader.debug.elf
)
add_custom_target(disasm ALL DEPENDS ${PROJECT_NAME}-bootloader.disasm.txt)

# Tworzenie pliku do EEPROM
# Wykomentowane bo tego nie potrzebujemy (przynajmniej na razie)
#add_custom_target(
#    eeprom
#    ${AVR_OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${PROJECT_NAME}-bootloader.elf" "${PROJECT_NAME}-bootloader.eeprom.hex"
#    DEPENDS ${PROJECT_NAME}-bootloader.elf
#)

# Wyswietl statystyki rozmiaru, zawsze
add_custom_target(
    size
    ALL
    ${AVR_SIZE_TOOL} --format=avr --mcu=${MCU} "${PROJECT_NAME}-bootloader.elf"
    DEPENDS ${PROJECT_NAME}-bootloader.elf framework-symbols
)