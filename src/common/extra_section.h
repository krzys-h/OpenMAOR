#pragma once

// Functions defined in the .bootloader_extra section are inaccesible during programming!
#define EXTRA __attribute__((section(".text.bootloader_extra")))
