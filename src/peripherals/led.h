#pragma once
#include "common/singleton.h"
#include "common/class_isr.h"

class CRobot;

/**
 * \class CLed
 * \brief A LED! :D
 */
class CLed {
protected:
    friend class CRobot;
    CLed(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t bit);

public:
    void Set(bool value);
    bool Get();
    void Toggle();

protected:
    volatile uint8_t* m_port;
    uint8_t m_bit;
};
