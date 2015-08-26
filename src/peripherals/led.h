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
    CLed(int id);

public:
    void Set(bool value);
    bool Get();
    void Toggle();

protected:
    int m_id;
};
