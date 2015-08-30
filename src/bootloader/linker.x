MEMORY
{
    bootloader_extra (rx) : ORIGIN = 0x6000, LENGTH = 0x1000
}

SECTIONS
{
    .bootloader_extra : {
        *(.text.bootloader_extra)
    }
}
INSERT BEFORE .text;
