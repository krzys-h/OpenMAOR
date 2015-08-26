#include "common/stdlib/assert.h"
#include "common/uart.h"

void assert_failed(const char* condition)
{
    if (CUart::IsCreated())
    {
        CUart::GetInstance().SendString("\r\n\r\nAssertion failed!\r\n    ");
        CUart::GetInstance().SendString(condition);
        CUart::GetInstance().SendString("\r\nProgram aborted.\r\n");
    }

    while (true);
}
