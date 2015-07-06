extern uint8_t page_buffer[SPM_PAGESIZE];

void flash_program_page(uint16_t address);
void programming_busy_wait();