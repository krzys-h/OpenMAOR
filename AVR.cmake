if(NOT DEFINED MCU)
    message(FATAL_ERROR "MCU is not defined!")
endif()

if(NOT DEFINED F_CPU)
    message(FATAL_ERROR "F_CPU is not defined!")
endif()

if(DEFINED BOOTLOADER_START)
    add_definitions(-DBOOTLOADER_START=${BOOTLOADER_START})
endif()

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

set(PRINTF_LIB "")
# Mozna sprobowac odkomentowac jesli bedzie nam brakowac miejsca
#set(PRINTF_LIB "-Wl,-u,vfprintf -lprintf_min")
# Mozna zmienic na printf_flt jesli bedzie trzeba, ale mam nadzieje ze nie

# https://github.com/acristoffers/CMake-AVR/blob/master/Blink/CMakeLists.txt#L66-80
# Flagi kompilatora
set(CMAKE_CXX_FLAGS "-mmcu=${MCU} -gstabs -g -ggdb -DF_CPU=${F_CPU} -Os -lm ${PRINTF_LIB} -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=c++11")
set(CMAKE_C_FLAGS   "${CMAKE_CXX_FLAGS} -Wall -Wstrict-prototypes -Wl,--gc-sections -Wl,--relax -std=gnu99")

function(avr_target_configure_as_bootloader TARGET_FILE)
    target_link_libraries(${TARGET_FILE} "-Wl,--section-start=.text=${BOOTLOADER_START}") # Przesuwamy sekcję .text do BOOTLOADER_START
endfunction(avr_target_configure_as_bootloader)

function(avr_target_configure TARGET_FILE)
    string(REGEX REPLACE "\\.[^.]*$" "" TARGET ${TARGET_FILE})

    add_custom_command(
        OUTPUT ${TARGET}.stripped.elf
        COMMAND ${AVR_STRIP} ${TARGET_FILE} -o ${TARGET}.stripped.elf
        DEPENDS ${TARGET_FILE}
    )
    add_custom_target(${TARGET_FILE}-strip ALL DEPENDS ${TARGET}.stripped.elf)

    # Tworzenie pliku .hex z .elf, zawsze
    add_custom_command(
        OUTPUT ${TARGET}.hex
        COMMAND ${AVR_OBJCOPY} -R .eeprom -O ihex ${TARGET}.stripped.elf ${TARGET}.hex
        DEPENDS ${TARGET_FILE}-strip ${TARGET}.stripped.elf
    )
    add_custom_target(${TARGET_FILE}-hex ALL DEPENDS ${TARGET}.hex)

    # disasm
    add_custom_command(
        OUTPUT ${TARGET}.disasm.txt
        COMMAND ${AVR_OBJDUMP} -d -m avr5 -S ${TARGET_FILE} > ${TARGET}.disasm.txt
        DEPENDS ${TARGET_FILE}
    )
    add_custom_target(${TARGET_FILE}-disasm ALL DEPENDS ${TARGET}.disasm.txt)

    # Wyswietl statystyki rozmiaru
    add_custom_target(
        ${TARGET}-size
        ALL
        ${AVR_SIZE_TOOL} --format=avr --mcu=${MCU} "${TARGET}.stripped.elf"
        DEPENDS ${TARGET_FILE}-strip ${TARGET}.stripped.elf
    )
endfunction(avr_target_configure)
