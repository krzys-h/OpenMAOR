#include "common/robot.h"

class CProgram : public CRobot
{
public:
    void Run()
    {
        while (true)
        {
            led[0].Toggle();
            timer.Sleep(500);
        }
    }
};

int main()
{
    CProgram program;
    program.Run();
}
