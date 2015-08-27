#pragma once
#include "common/singleton.h"
#include "common/class_isr.h"

class CPeripherals;

// TODO: CSingleton jest tu tylko dla ISR, nie da sie tego zrobic inaczej?

/**
 * \class CButton
 * \brief A button! :D
 */
class CButton : public CSingleton<CButton>
{
protected:
    friend class CPeripherals;
    CButton();

public:
    bool Get();

    typedef void(*ButtonCallback)();
    void SetCallback(ButtonCallback callback);

    DECLARE_CLASS_ISR(INT1_vect);

protected:
    ButtonCallback m_callback;
};
