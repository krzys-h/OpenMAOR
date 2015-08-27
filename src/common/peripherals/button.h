#pragma once
#include "common/singleton.h"
#include "common/class_isr.h"

#include "common/extra_section.h"

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
    CButton() EXTRA;

public:
    bool Get() EXTRA;

    typedef void(*ButtonCallback)();
    void SetCallback(ButtonCallback callback) EXTRA;

    DECLARE_CLASS_ISR(INT1_vect) EXTRA;

protected:
    ButtonCallback m_callback;
};
