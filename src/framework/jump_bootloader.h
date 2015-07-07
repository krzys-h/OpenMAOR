const static void (*jump_bootloader)(void) __attribute__((noreturn)) = (const void(*)())BOOTLOADER_START;

void exit_program();