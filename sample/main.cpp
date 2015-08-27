#include <avr/io.h>
#include "common/framework.h"

class CProgram : public CRobot
{
public:
    void Run()
    {
        while (true)
        {
            led[0].Set(true);
            Sleep(500);
            led[0].Set(false);
            Sleep(500);
        }
    }
};

int main()
{
    CProgram program;
    program.Run();
}
